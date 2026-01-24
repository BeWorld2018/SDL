/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2026 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include "../../SDL_internal.h"

#ifdef SDL_JOYSTICK_MORPHOS

#include "../SDL_sysjoystick.h"
#include "../SDL_joystick_c.h"
#include "SDL_endian.h"
#include "SDL_joystick.h"
#include "SDL_timer.h"

#include <exec/execbase.h>
#include <proto/exec.h>
#include <libraries/sensors.h>
#include <libraries/sensors_hid.h>
#define USE_INLINE_STDARG
#include <proto/sensors.h>
#undef USE_INLINE_STDARG

#ifndef SENSORS_HIDInput_Rumble_Type
#define SENSORS_HIDInput_Rumble_Type (SENSORS_Dummy + 2403)
#endif

/* SDL2 deadzone is around 409, we need 1638 */
#define DEADZONE_MIN (-0.05)
#define DEADZONE_MAX (0.05)

#define JOYSTICK_MIN -1.0
#define JOYSTICK_MAX  1.0

#define CLAMP(val) \
    ((((val) <= (DEADZONE_MAX) && (val) >= (DEADZONE_MIN)) ? (0) : \
     ((val) > (JOYSTICK_MAX)) ? (JOYSTICK_MAX) : (((val) < (JOYSTICK_MIN)) ? (JOYSTICK_MIN) : (val))))

/* ---- Tunables ---- */
#ifndef MAX_JOYSTICKS
#define MAX_JOYSTICKS 4
#endif

#ifndef MAX_BUTTONS
#define MAX_BUTTONS 16
#endif
#ifndef MAX_HATS
#define MAX_HATS 8
#endif
#ifndef MAX_STICKS
#define MAX_STICKS 8
#endif
#ifndef MAX_RUMBLE
#define MAX_RUMBLE 4
#endif

#ifndef AXIS_EPS
#define AXIS_EPS 64 /* 16 = plus sensible, 64 réduit bien le jitter */
#endif

/* ---- Globals inventory ---- */
APTR sensorlist;
APTR JoySensor[MAX_JOYSTICKS];
int joystick_count;

typedef struct JoyEntry {
    APTR sensor;
    SDL_JoystickID instance_id;
    ULONG vendor;
    ULONG product;
    char name[128];
    char serial[128];
} JoyEntry;

static JoyEntry g_joy[MAX_JOYSTICKS];
static SDL_JoystickID g_next_instance_id = 1;

static struct MsgPort *g_hidNotifyPort = NULL;
static SDL_bool g_hidListDirty = SDL_TRUE;
static APTR g_classListNotify = NULL;

struct joystick_hwdata
{
    APTR main_sensor;
    APTR child_sensors;

    APTR button[MAX_BUTTONS];
    APTR hat[MAX_HATS];
    APTR stick[MAX_STICKS];
    APTR battery;
    APTR rumble[MAX_RUMBLE];

    ULONG stickType[MAX_STICKS];
    int numSticks;
    int numRumbles;

    Uint8 last_hat[MAX_HATS];
    SDL_bool hat_inited;

    Sint16 last_axis[MAX_STICKS * 4];
    SDL_bool axis_inited;

    Uint32 last_battery_ts;
    int last_battery_level;
    SDL_bool battery_inited;
};
static int MOS_ScanJoysticks(JoyEntry out[MAX_JOYSTICKS], APTR list)
{
    APTR sensor = NULL;
    int count = 0;

    while ((sensor = NextSensor(sensor, list, NULL)) && count < MAX_JOYSTICKS) {
        const char *name = NULL;
        const char *serial = NULL;
        ULONG vendor = 0, product = 0;

        GetSensorAttrTags(sensor,
                          SENSORS_HID_Name,    (IPTR)&name,
                          SENSORS_HID_Serial,  (IPTR)&serial,
                          SENSORS_HID_Product, (IPTR)&product,
                          SENSORS_HID_Vendor,  (IPTR)&vendor,
                          TAG_DONE);

        out[count].sensor = sensor;
        out[count].instance_id = 0;
        out[count].vendor = vendor;
        out[count].product = product;
        SDL_strlcpy(out[count].name,   (name && *name) ? name : "<unknown>", sizeof(out[count].name));
        SDL_strlcpy(out[count].serial, (serial) ? serial : "",               sizeof(out[count].serial));
        count++;
    }

    return count;
}

static SDL_bool MOS_SameDevice(const JoyEntry *a, const JoyEntry *b)
{
    if (a->sensor && b->sensor && a->sensor == b->sensor) {
        return SDL_TRUE;
    }

    if (a->vendor != b->vendor || a->product != b->product) {
        return SDL_FALSE;
    }

    if (a->serial[0] && b->serial[0]) {
        return (SDL_strcmp(a->serial, b->serial) == 0) ? SDL_TRUE : SDL_FALSE;
    }

    return (SDL_strcmp(a->name, b->name) == 0) ? SDL_TRUE : SDL_FALSE;
}


static int MOS_JoystickGetCount(void);
static const char *MOS_JoystickGetDeviceName(int device_index);
static SDL_JoystickID MOS_JoystickGetDeviceInstanceID(int device_index);
static SDL_JoystickGUID MOS_JoystickGetDeviceGUID(int device_index);

static int MOS_JoystickInit(void)
{
    SDL_zero(g_joy);
    SDL_zero(JoySensor);
    joystick_count = 0;

    if (sensorlist) {
        ReleaseSensorsList(sensorlist, NULL);
        sensorlist = NULL;
    }

    sensorlist = ObtainSensorsListTags(SENSORS_Class, SensorClass_HID, TAG_DONE);
    if (!sensorlist) {
        return 0;
    }

    if (!g_hidNotifyPort) {
        g_hidNotifyPort = CreateMsgPort();
        if (g_hidNotifyPort) {
            struct TagItem nottags[] = {
                { SENSORS_Notification_Destination, (IPTR)g_hidNotifyPort },
                { SENSORS_Notification_ClassListChanged, TRUE },
                { SENSORS_Class, SensorClass_HID },
                { TAG_DONE }
            };
            g_classListNotify = StartSensorNotify(NULL, nottags);
            if (!g_classListNotify) {
                DeleteMsgPort(g_hidNotifyPort);
                g_hidNotifyPort = NULL;
            } else {
                g_hidListDirty = SDL_TRUE;
            }
        }
    }

    JoyEntry now[MAX_JOYSTICKS];
    SDL_zero(now);
    const int now_count = MOS_ScanJoysticks(now, sensorlist);

    joystick_count = now_count;
    for (int i = 0; i < now_count; i++) {
        now[i].instance_id = g_next_instance_id++;
        g_joy[i] = now[i];
        JoySensor[i] = now[i].sensor;
    }

    g_hidListDirty = SDL_FALSE;
    return 0;
}

static int MOS_JoystickGetCount(void)
{
    return joystick_count;
}

static void MOS_JoystickDetect(void)
{
    SDL_bool got_any = SDL_FALSE;

    if (g_hidNotifyPort) {
        struct SensorsNotificationMessage *msg;
        while ((msg = (struct SensorsNotificationMessage *)GetMsg(g_hidNotifyPort)) != NULL) {
            got_any = SDL_TRUE;
            ReplyMsg(&msg->Msg);
        }
        if (got_any) {
            g_hidListDirty = SDL_TRUE;
        }
    }

    if (!g_hidListDirty) {
        return;
    }
    g_hidListDirty = SDL_FALSE;

    APTR new_list = ObtainSensorsListTags(SENSORS_Class, SensorClass_HID, TAG_DONE);
    if (!new_list) {
        g_hidListDirty = SDL_TRUE;
        return;
    }

    JoyEntry now[MAX_JOYSTICKS];
    SDL_zero(now);
    const int now_count = MOS_ScanJoysticks(now, new_list);

    SDL_bool now_used[MAX_JOYSTICKS];
    SDL_zero(now_used);

    SDL_bool old_present[MAX_JOYSTICKS];
    SDL_zero(old_present);

    SDL_JoystickID removed_ids[MAX_JOYSTICKS];
    int removed_count = 0;

    SDL_JoystickID added_instances[MAX_JOYSTICKS];
    int added_count = 0;

    JoyEntry next[MAX_JOYSTICKS];
    SDL_zero(next);
    int next_count = 0;

    /* 1) Survivors en gardant l'ordre OLD */
    for (int j = 0; j < joystick_count && next_count < MAX_JOYSTICKS; j++) {
        for (int i = 0; i < now_count; i++) {
            if (now_used[i]) continue;
            if (MOS_SameDevice(&now[i], &g_joy[j])) {
                now_used[i] = SDL_TRUE;
                old_present[j] = SDL_TRUE;

                now[i].instance_id = g_joy[j].instance_id; /* reste stable tant que c'est le "même" device */
                next[next_count++] = now[i];
                break;
            }
        }
    }

    /* 2) Tout ce qui reste est "nouveau/replug" => nouvel instance_id, et on note le *device_index* futur */
    for (int i = 0; i < now_count && next_count < MAX_JOYSTICKS; i++) {
        if (now_used[i]) continue;

        now_used[i] = SDL_TRUE;
        now[i].instance_id = g_next_instance_id++;
        next[next_count++] = now[i];

        if (added_count < MAX_JOYSTICKS) {
            added_instances[added_count++] = now[i].instance_id; 
        }
    }

    for (int j = 0; j < joystick_count; j++) {
        if (!old_present[j] && removed_count < MAX_JOYSTICKS) {
            removed_ids[removed_count++] = g_joy[j].instance_id;
        }
    }

    if (sensorlist) {
        ReleaseSensorsList(sensorlist, NULL);
    }
    sensorlist = new_list;
	
	SDL_LockJoysticks();
		
    joystick_count = next_count;
    for (int i = 0; i < next_count; i++) {
        g_joy[i] = next[i];
        JoySensor[i] = next[i].sensor;
    }
	
    for (int i = next_count; i < MAX_JOYSTICKS; i++) {
       SDL_zero(g_joy[i]);
   	   JoySensor[i] = NULL;
    }

    for (int k = 0; k < removed_count; k++) {
        SDL_JoystickID inst = removed_ids[k];
        D("[%s] Removed inst=%d\n", __FUNCTION__, (int)inst);
        SDL_PrivateJoystickRemoved(inst);
    }
	
	for (int k = 0; k < added_count; k++) {
		SDL_JoystickID inst = added_instances[k];
		D("[%s] Added inst=%d\n", __FUNCTION__, (int)inst);
		SDL_PrivateJoystickAdded(inst);
	}
	
	SDL_UnlockJoysticks();
}

static const char *MOS_JoystickGetDeviceName(int device_index)
{
    if (device_index < 0 || device_index >= joystick_count) return NULL;
    return g_joy[device_index].name;
}

static const char *MOS_JoystickGetDevicePath(int device_index)
{
    (void)device_index;
    return NULL;
}

static int MOS_JoystickGetDeviceSteamVirtualGamepadSlot(int device_index)
{
    (void)device_index;
    return -1;
}

static int MOS_JoystickGetDevicePlayerIndex(int device_index)
{
    return device_index;
}

static void MOS_JoystickSetDevicePlayerIndex(int device_index, int player_index)
{
    (void)device_index; (void)player_index;
}

static SDL_JoystickGUID MOS_JoystickGetDeviceGUID(int device_index)
{
    SDL_JoystickGUID guid;
    Uint16 *guid16 = (Uint16 *)guid.data;

    SDL_zero(guid);

    if (device_index < 0 || device_index >= joystick_count) {
        return guid;
    }

    const ULONG vendor  = g_joy[device_index].vendor;
    const ULONG product = g_joy[device_index].product;
    const char *name    = g_joy[device_index].name;

    *guid16++ = SDL_SwapLE16(SDL_HARDWARE_BUS_USB);
    *guid16++ = 0;

    if (vendor && product) {
        *guid16++ = SDL_SwapLE16((Uint16)vendor);
        *guid16++ = 0;
        *guid16++ = SDL_SwapLE16((Uint16)product);
        *guid16++ = 0;
        *guid16++ = 0;
        *guid16++ = 0;
    } else {
        SDL_strlcpy((char *)guid16, name ? name : "<unknown>", sizeof(guid.data) - 4);
    }

    return guid;
}

static SDL_JoystickID MOS_JoystickGetDeviceInstanceID(int device_index)
{
    if (device_index < 0 || device_index >= joystick_count) return 0;
    return g_joy[device_index].instance_id;
}

static int MOS_JoystickOpen(SDL_Joystick *joystick, int device_index)
{
    APTR sensor = (device_index >= 0 && device_index < joystick_count) ? g_joy[device_index].sensor : NULL;
    int rc = -1;

    if (sensor) {
        size_t buttons = 0, naxes = 0, nhats = 0, nsticks = 0;
        CONST_STRPTR name = "<unknown>";

        struct joystick_hwdata *hwdata = (struct joystick_hwdata *)SDL_calloc(1, sizeof(*hwdata));
        if (!hwdata) {
            return SDL_OutOfMemory();
        }

        hwdata->main_sensor = sensor;
        hwdata->hat_inited = SDL_FALSE;
        hwdata->axis_inited = SDL_FALSE;
        hwdata->battery_inited = SDL_FALSE;
        hwdata->last_battery_ts = 0;
        hwdata->last_battery_level = -1;

        hwdata->child_sensors = ObtainSensorsListTags(
            SENSORS_Parent, (IPTR)hwdata->main_sensor,
            SENSORS_Class, SensorClass_HID,
            TAG_DONE);

        sensor = NULL;
        while ((sensor = NextSensor(sensor, hwdata->child_sensors, NULL))) {
            ULONG type = SensorType_HIDInput_Unknown;

            if (GetSensorAttrTags(sensor, SENSORS_Type, (IPTR)&type, TAG_DONE)) {
                switch (type) {
                    case SensorType_HIDInput_Trigger:
                        if (buttons < MAX_BUTTONS) hwdata->button[buttons++] = sensor;
                        break;

                    case SensorType_HIDInput_Stick:
                        if (nhats < MAX_HATS) hwdata->hat[nhats++] = sensor;
                        break;

                    case SensorType_HIDInput_Analog:
                    case SensorType_HIDInput_AnalogStick:
                    case SensorType_HIDInput_3DStick:
                        if (nsticks < MAX_STICKS) {
                            hwdata->stick[nsticks] = sensor;
                            hwdata->stickType[nsticks] = type;
                            nsticks++;
                            if (type == SensorType_HIDInput_AnalogStick) naxes += 2;
                            else if (type == SensorType_HIDInput_3DStick) naxes += 4;
                            else naxes++;
                        }
                        break;

                    case SensorType_HIDInput_Rumble:
                    {
                        ULONG limb = 0, rtype = 0;
                        GetSensorAttrTags(sensor,
                            SENSORS_HIDInput_Limb, (IPTR)&limb,
                            SENSORS_HIDInput_Rumble_Type, (IPTR)&rtype,
                            TAG_DONE);

                        if (hwdata->numRumbles < 4) hwdata->numRumbles++;

                        if (rtype == 0) hwdata->rumble[(limb == 0) ? 0 : 1] = sensor;
                        else if (rtype == 1) hwdata->rumble[(limb == 0) ? 2 : 3] = sensor;
                        break;
                    }

                    case SensorType_HIDInput_Battery:
                        GetSensorAttrTags(sensor, SENSORS_HID_Name, (IPTR)&name, TAG_DONE);
                        if (SDL_strcmp(MOS_JoystickGetDeviceName(device_index), "Xbox360 Controller") == 0) {
                            SDL_PrivateJoystickBatteryLevel(joystick, SDL_JOYSTICK_POWER_WIRED);
                            hwdata->battery = NULL;
                        } else {
                            hwdata->battery = sensor;
                        }
                        break;

                    default:
                        break;
                }
            }
        }

        joystick->naxes = (int)naxes;
        joystick->nhats = (int)nhats;
        joystick->nbuttons = (int)buttons;
        hwdata->numSticks = (int)nsticks;
        joystick->hwdata = hwdata;
        rc = 0;
    } else {
        SDL_SetError("Failed to open device");
    }

    return rc;
}

static SDL_bool MOS_SetRumbleMotor(APTR motor, DOUBLE power, ULONG duration_ms)
{
    if (!motor) return SDL_TRUE;
    if (power < 0.0) power = 0.0;
    if (power > 1.0) power = 1.0;

    return (SetSensorAttrTags(motor,
        SENSORS_HIDInput_Rumble_Power, (IPTR)&power,
        SENSORS_HIDInput_Rumble_Duration, duration_ms,
        TAG_DONE) != 0) ? SDL_TRUE : SDL_FALSE;
}

static int MOS_JoystickRumble(SDL_Joystick *joystick, Uint16 low, Uint16 high)
{
    if (!joystick || !joystick->attached) return 0;

    struct joystick_hwdata *hw = (struct joystick_hwdata *)joystick->hwdata;
    if (!hw) return SDL_Unsupported();
    if (!hw->rumble[0] && !hw->rumble[1]) return SDL_Unsupported();

    const SDL_bool stop = (low == 0 && high == 0) ? SDL_TRUE : SDL_FALSE;
    const ULONG dur = stop ? 0 : 1200;

    DOUBLE l = (DOUBLE)low  / 65535.0;
    DOUBLE r = (DOUBLE)high / 65535.0;

    if (!hw->rumble[0] && hw->rumble[1]) l = r;
    if (!hw->rumble[1] && hw->rumble[0]) r = l;

    if (!MOS_SetRumbleMotor(hw->rumble[0], stop ? 0.0 : l, dur)) return -1;
    if (!MOS_SetRumbleMotor(hw->rumble[1], stop ? 0.0 : r, dur)) return -1;
    return 0;
}

static int MOS_JoystickRumbleTriggers(SDL_Joystick *joystick, Uint16 left, Uint16 right)
{
    if (!joystick || !joystick->attached) return 0;

    struct joystick_hwdata *hw = (struct joystick_hwdata *)joystick->hwdata;
    if (!hw) return SDL_Unsupported();
    if (!hw->rumble[2] && !hw->rumble[3]) return SDL_Unsupported();

    const SDL_bool stop = (left == 0 && right == 0) ? SDL_TRUE : SDL_FALSE;
    const ULONG dur = stop ? 0 : 1200;

    DOUBLE l = (DOUBLE)left  / 65535.0;
    DOUBLE r = (DOUBLE)right / 65535.0;

    if (!hw->rumble[2] && hw->rumble[3]) l = r;
    if (!hw->rumble[3] && hw->rumble[2]) r = l;

    if (!MOS_SetRumbleMotor(hw->rumble[2], stop ? 0.0 : l, dur)) return -1;
    if (!MOS_SetRumbleMotor(hw->rumble[3], stop ? 0.0 : r, dur)) return -1;
    return 0;
}

static Uint32 MOS_JoystickGetCapabilities(SDL_Joystick *joystick)
{
    Uint32 result = 0;
    struct joystick_hwdata *hw = (struct joystick_hwdata *)joystick->hwdata;
    if (hw) {
        if (hw->rumble[0] || hw->rumble[1]) result |= SDL_JOYCAP_RUMBLE;
        if (hw->rumble[2] || hw->rumble[3]) result |= SDL_JOYCAP_RUMBLE_TRIGGERS;
    }
    return result;
}

static int MOS_JoystickSetLED(SDL_Joystick *joystick, Uint8 red, Uint8 green, Uint8 blue)
{
    (void)joystick; (void)red; (void)green; (void)blue;
    return SDL_Unsupported();
}

static int MOS_JoystickSendEffect(SDL_Joystick *joystick, const void *data, int size)
{
    (void)joystick; (void)data; (void)size;
    return SDL_Unsupported();
}

static int MOS_JoystickSetSensorsEnabled(SDL_Joystick *joystick, SDL_bool enabled)
{
    (void)joystick; (void)enabled;
    return SDL_Unsupported();
}

static void MOS_JoystickUpdate(SDL_Joystick *joystick)
{
    if (!joystick || !joystick->attached) return;

    struct joystick_hwdata *hwdata = (struct joystick_hwdata *)joystick->hwdata;
    if (!hwdata) return;

    int i, j;
    Sint16 sval;
    double btn_value, bt_value, x_value, y_value, z_value, ns_value, ew_value, z_rotation;

    for (i = 0; i < joystick->nbuttons; i++) {
        GetSensorAttrTags(hwdata->button[i], SENSORS_HIDInput_Value, (IPTR)&btn_value, TAG_DONE);
        if ((joystick->buttons[i] && btn_value == 0.0) || (joystick->buttons[i] == 0 && btn_value > 0.0)) {
            SDL_PrivateJoystickButton(joystick, i, btn_value == 0.0 ? 0 : 1);
        }
    }

    for (i = 0; i < joystick->nhats; i++) {
        if (!GetSensorAttrTags(hwdata->hat[i],
                SENSORS_HIDInput_EW_Value, (IPTR)&ew_value,
                SENSORS_HIDInput_NS_Value, (IPTR)&ns_value,
                TAG_DONE)) {
            continue;
        }
        int ns = (ns_value > 0.5) ? 1 : (ns_value < -0.5) ? -1 : 0;
        int ew = (ew_value > 0.5) ? 1 : (ew_value < -0.5) ? -1 : 0;

        Uint8 value_hat = SDL_HAT_CENTERED;
        if (ns == 1) value_hat |= SDL_HAT_DOWN;
        else if (ns == -1) value_hat |= SDL_HAT_UP;
        if (ew == 1) value_hat |= SDL_HAT_RIGHT;
        else if (ew == -1) value_hat |= SDL_HAT_LEFT;

        if (!hwdata->hat_inited || hwdata->last_hat[i] != value_hat) {
            hwdata->last_hat[i] = value_hat;
            SDL_PrivateJoystickHat(joystick, i, value_hat);
        }
    }
    hwdata->hat_inited = SDL_TRUE;

    j = 0;
    for (i = 0; i < hwdata->numSticks; i++) {
        switch (hwdata->stickType[i]) {
            case SensorType_HIDInput_3DStick:
                GetSensorAttrTags(hwdata->stick[i],
                    SENSORS_HIDInput_X_Index, (IPTR)&x_value,
                    SENSORS_HIDInput_Y_Index, (IPTR)&y_value,
                    SENSORS_HIDInput_Z_Index, (IPTR)&z_value,
                    SENSORS_HIDInput_Z_Rotation, (IPTR)&z_rotation,
                    TAG_DONE);

                sval = (Sint16)(CLAMP(x_value) * SDL_JOYSTICK_AXIS_MAX);
                if (!hwdata->axis_inited || SDL_abs((int)sval - (int)hwdata->last_axis[j]) >= AXIS_EPS) {
                    hwdata->last_axis[j] = sval;
                    SDL_PrivateJoystickAxis(joystick, j, sval);
                }

                sval = (Sint16)(CLAMP(y_value) * SDL_JOYSTICK_AXIS_MAX);
                if (!hwdata->axis_inited || SDL_abs((int)sval - (int)hwdata->last_axis[j+1]) >= AXIS_EPS) {
                    hwdata->last_axis[j+1] = sval;
                    SDL_PrivateJoystickAxis(joystick, j+1, sval);
                }

                sval = (Sint16)(CLAMP(z_value) * SDL_JOYSTICK_AXIS_MAX);
                if (!hwdata->axis_inited || SDL_abs((int)sval - (int)hwdata->last_axis[j+2]) >= AXIS_EPS) {
                    hwdata->last_axis[j+2] = sval;
                    SDL_PrivateJoystickAxis(joystick, j+2, sval);
                }

                sval = (Sint16)(CLAMP(z_rotation) * SDL_JOYSTICK_AXIS_MAX);
                if (!hwdata->axis_inited || SDL_abs((int)sval - (int)hwdata->last_axis[j+3]) >= AXIS_EPS) {
                    hwdata->last_axis[j+3] = sval;
                    SDL_PrivateJoystickAxis(joystick, j+3, sval);
                }

                j += 4;
                break;

            case SensorType_HIDInput_Analog:
                GetSensorAttrTags(hwdata->stick[i], SENSORS_HIDInput_Value, (IPTR)&btn_value, TAG_DONE);
                sval = (Sint16)(btn_value * SDL_JOYSTICK_AXIS_MAX);
                if (!hwdata->axis_inited || SDL_abs((int)sval - (int)hwdata->last_axis[j]) >= AXIS_EPS) {
                    hwdata->last_axis[j] = sval;
                    SDL_PrivateJoystickAxis(joystick, j, sval);
                }
                j++;
                break;

            case SensorType_HIDInput_AnalogStick:
                GetSensorAttrTags(hwdata->stick[i],
                    SENSORS_HIDInput_EW_Value, (IPTR)&ew_value,
                    SENSORS_HIDInput_NS_Value, (IPTR)&ns_value,
                    TAG_DONE);

                sval = (Sint16)(CLAMP(ew_value) * SDL_JOYSTICK_AXIS_MAX);
                if (!hwdata->axis_inited || SDL_abs((int)sval - (int)hwdata->last_axis[j]) >= AXIS_EPS) {
                    hwdata->last_axis[j] = sval;
                    SDL_PrivateJoystickAxis(joystick, j, sval);
                }

                sval = (Sint16)(CLAMP(ns_value) * SDL_JOYSTICK_AXIS_MAX);
                if (!hwdata->axis_inited || SDL_abs((int)sval - (int)hwdata->last_axis[j+1]) >= AXIS_EPS) {
                    hwdata->last_axis[j+1] = sval;
                    SDL_PrivateJoystickAxis(joystick, j+1, sval);
                }

                j += 2;
                break;

            default:
                break;
        }
    }
    hwdata->axis_inited = SDL_TRUE;

    if (hwdata->battery) {
        Uint32 now = SDL_GetTicks();
        if (!hwdata->battery_inited || (now - hwdata->last_battery_ts) >= 1000) {
            hwdata->last_battery_ts = now;
            hwdata->battery_inited = SDL_TRUE;

            if (GetSensorAttrTags(hwdata->battery, SENSORS_HIDInput_Value, (IPTR)&bt_value, TAG_DONE)) {
                int level = (int)(bt_value * 100.0);
                if (level < 0) level = 0;
                if (level > 100) level = 100;

                if (level != hwdata->last_battery_level) {
                    hwdata->last_battery_level = level;
                    SDL_JoystickPowerLevel e = SDL_JOYSTICK_POWER_UNKNOWN;
                    if (level <= 5) e = SDL_JOYSTICK_POWER_EMPTY;
                    else if (level <= 20) e = SDL_JOYSTICK_POWER_LOW;
                    else if (level <= 70) e = SDL_JOYSTICK_POWER_MEDIUM;
                    else e = SDL_JOYSTICK_POWER_FULL;
                    SDL_PrivateJoystickBatteryLevel(joystick, e);
                }
            }
        }
    }
}

void MOS_JoystickClose(SDL_Joystick *joystick)
{
    struct joystick_hwdata *hwdata = (struct joystick_hwdata *)joystick->hwdata;
    if (hwdata) {
        if (joystick->attached) {
            (void)MOS_JoystickRumble(joystick, 0, 0);
            (void)MOS_JoystickRumbleTriggers(joystick, 0, 0);
        }
        if (hwdata->child_sensors) {
            ReleaseSensorsList(hwdata->child_sensors, NULL);
            hwdata->child_sensors = NULL;
        }
        SDL_free(hwdata);
        joystick->hwdata = NULL;
    }
}

void MOS_JoystickQuit(void)
{
    if (g_hidNotifyPort) {
        if (g_classListNotify) {
            EndSensorNotify(g_classListNotify, NULL);
            g_classListNotify = NULL;
        }
        struct SensorsNotificationMessage *msg;
        while ((msg = (struct SensorsNotificationMessage *)GetMsg(g_hidNotifyPort)) != NULL) {
            ReplyMsg(&msg->Msg);
        }
        DeleteMsgPort(g_hidNotifyPort);
        g_hidNotifyPort = NULL;
    }

    if (sensorlist) {
        ReleaseSensorsList(sensorlist, NULL);
        sensorlist = NULL;
    }

    SDL_zero(g_joy);
    SDL_zero(JoySensor);
    g_hidListDirty = SDL_TRUE;
    joystick_count = 0;
}

static SDL_bool MOS_JoystickGetGamepadMapping(int device_index, SDL_GamepadMapping *out)
{
    (void)device_index; (void)out;
    return SDL_FALSE;
}

SDL_JoystickDriver SDL_MOS_JoystickDriver =
{
    MOS_JoystickInit,
    MOS_JoystickGetCount,
    MOS_JoystickDetect,
    MOS_JoystickGetDeviceName,
    MOS_JoystickGetDevicePath,
    MOS_JoystickGetDeviceSteamVirtualGamepadSlot,
    MOS_JoystickGetDevicePlayerIndex,
    MOS_JoystickSetDevicePlayerIndex,
    MOS_JoystickGetDeviceGUID,
    MOS_JoystickGetDeviceInstanceID,
    MOS_JoystickOpen,
    MOS_JoystickRumble,
    MOS_JoystickRumbleTriggers,
    MOS_JoystickGetCapabilities,
    MOS_JoystickSetLED,
    MOS_JoystickSendEffect,
    MOS_JoystickSetSensorsEnabled,
    MOS_JoystickUpdate,
    MOS_JoystickClose,
    MOS_JoystickQuit,
    MOS_JoystickGetGamepadMapping
};

#endif