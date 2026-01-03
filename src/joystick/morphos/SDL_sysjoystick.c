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

#include "SDL_internal.h"

#ifdef SDL_JOYSTICK_MORPHOS

#include "../SDL_sysjoystick.h"
#include "../SDL_joystick_c.h"
#include "SDL_joystick.h"

// Define that to use new GUID (not compatible with mapping SDL2)
// #define USE_NEW_GUID
#ifndef USE_NEW_GUID
#include "SDL_endian.h"
#endif

#include <dos/dos.h>
#include <exec/execbase.h>
#include <libraries/sensors.h>
#include <libraries/sensors_hid.h>
#include <proto/exec.h>
#define USE_INLINE_STDARG
#include <proto/sensors.h>
#undef USE_INLINE_STDARG
#include <proto/threadpool.h>
#include <proto/utility.h>

// SDL2 deadzone is around 409, we need 1638
#define DEADZONE_MIN (-0.05)
#define DEADZONE_MAX (0.05)

#define JOYSTICK_MIN -1.0
#define JOYSTICK_MAX 1.0

#define CLAMP(val) \
			(((val) <= (DEADZONE_MAX) && (val) >= (DEADZONE_MIN)) ? (0) : \
			((val) > (JOYSTICK_MAX)) ? (JOYSTICK_MAX) : (((val) < (JOYSTICK_MIN)) ? (JOYSTICK_MIN) : (val)))

#define MAX_JOYSTICKS 	4
#define MAX_BUTTONS     16
#define MAX_HATS        8
#define MAX_STICKS      8
#define MAX_RUMBLE		4
#define AXIS_EPS 64  /* à ajuster: 16 plus sensible, 64 réduit bien le jitter */

#ifndef SENSORS_HIDInput_Rumble_Type
#define SENSORS_HIDInput_Rumble_Type (SENSORS_Dummy + 2403)
#endif

struct joystick_hwdata
{
	APTR child_sensors; 		// List of specific sub-sensor entries
	APTR button[MAX_BUTTONS]; 	// SensorType_HIDInput_Trigger
	APTR hat[MAX_HATS]; 		// SensorType_HIDInput_Stick
	APTR stick[MAX_STICKS]; 	// SensorType_HIDInput_Analog, SensorType_HIDInput_AnalogStick, SensorType_HIDInput_3DStick
	APTR battery; 				// SensorType_HIDInput_Battery 
	APTR rumble[MAX_RUMBLE]; 	// SensorType_HIDInput_Rumble

	ULONG stickType[MAX_STICKS];
	int numSticks;
	int numRumbles;
	Uint8 last_hat[MAX_HATS];
	bool hat_inited;
	Sint16 last_axis[MAX_STICKS * 4];
	bool axis_inited;
	
	Uint64 last_battery_ts;
	int last_battery_level;
	bool battery_inited;
};

APTR sensorlist;
APTR JoySensor[MAX_JOYSTICKS];
int joystick_count = 0;

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
static volatile bool g_hidListDirty = true;
static APTR g_classListNotify = NULL;

static int MORPHOS_ScanJoysticks(JoyEntry out[MAX_JOYSTICKS], APTR list)
{
    APTR sensor = NULL;
    int count = 0;

    while ((sensor = NextSensor(sensor, list, NULL)) && count < MAX_JOYSTICKS) {
        const char *name = "<unknown>";
        const char *serial = "";
        ULONG vendor = 0, product = 0;

        GetSensorAttrTags(sensor,
                          SENSORS_HID_Name,   (IPTR)&name,
                          SENSORS_HID_Serial, (IPTR)&serial,
                          SENSORS_HID_Product,(IPTR)&product,
                          SENSORS_HID_Vendor, (IPTR)&vendor,
                          TAG_DONE);

        out[count].sensor = sensor;
        out[count].instance_id = 0;
        out[count].vendor = vendor;
        out[count].product = product;
        SDL_strlcpy(out[count].name,   name ? name : "<unknown>", sizeof(out[count].name));
        SDL_strlcpy(out[count].serial, serial ? serial : "",      sizeof(out[count].serial));
        count++;
    }
    return count;
}

static bool MORPHOS_SameDevice(const JoyEntry *a, const JoyEntry *b)
{
    if (a->vendor != b->vendor || a->product != b->product) {
        return false;
    }

    /* Si on a un serial non vide des deux côtés, c'est la clé */
    if (a->serial[0] && b->serial[0]) {
        return (SDL_strcmp(a->serial, b->serial) == 0);
    }

    /* Fallback: name */
    return (SDL_strcmp(a->name, b->name) == 0);
}

static bool MORPHOS_JoystickInit(void)
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
        D("Failed to ObtainSensorsListTags");
        return true;
    }

	if (!g_hidNotifyPort) {
		g_hidNotifyPort = CreateMsgPort();
		if (!g_hidNotifyPort) {
			D("Failed to create HID notify port (fallback to polling)");
		} else {
			struct TagItem nottags[] =
				{
					{SENSORS_Notification_Destination, (IPTR)g_hidNotifyPort},
					{SENSORS_Notification_ClassListChanged, TRUE},
					{SENSORS_Class, SensorClass_HID},
					{TAG_DONE}
				};
				
			g_classListNotify = StartSensorNotify(NULL, nottags);
			if (!g_classListNotify) {
				D("StartSensorNotify failed (fallback to polling)");
				DeleteMsgPort(g_hidNotifyPort);
				g_hidNotifyPort = NULL;
			} else {
				g_hidListDirty = true;
			}
		}
	}

    JoyEntry now[MAX_JOYSTICKS];
    SDL_zero(now);

    int now_count = MORPHOS_ScanJoysticks(now, sensorlist);

    joystick_count = now_count;
    for (int i = 0; i < now_count; i++) {
        now[i].instance_id = g_next_instance_id++;
        g_joy[i] = now[i];
        JoySensor[i] = now[i].sensor;
        SDL_PrivateJoystickAdded(g_joy[i].instance_id);
    }

    D("Found %ld joysticks...", joystick_count);
    return true;
}

static int MORPHOS_JoystickGetCount(void)
{
	return joystick_count;
}

static void MORPHOS_JoystickDetect(void)
{

	if (g_hidNotifyPort) {
		struct SensorsNotificationMessage *msg;
		bool got_any = false;

		while ((msg = (struct SensorsNotificationMessage *)GetMsg(g_hidNotifyPort)) != NULL) {
			got_any = true;
			ReplyMsg(&msg->Msg);
		}

		if (got_any) {
			g_hidListDirty = true;
		}
	}

	if (!g_hidListDirty) {
		return;
	}
	g_hidListDirty = false;

    APTR new_list = ObtainSensorsListTags(SENSORS_Class, SensorClass_HID, TAG_DONE);
    if (!new_list) {
		g_hidListDirty = true;
        return;
    }

    JoyEntry now[MAX_JOYSTICKS];
    SDL_zero(now);
    const int now_count = MORPHOS_ScanJoysticks(now, new_list);

    bool old_matched[MAX_JOYSTICKS];
    SDL_zero(old_matched);

    SDL_JoystickID added_ids[MAX_JOYSTICKS];
    int added_count = 0;

    SDL_JoystickID removed_ids[MAX_JOYSTICKS];
    int removed_count = 0;

    for (int i = 0; i < now_count; i++) {
        SDL_JoystickID kept = 0;

        for (int j = 0; j < joystick_count; j++) {
            if (!old_matched[j] && MORPHOS_SameDevice(&now[i], &g_joy[j])) {
                kept = g_joy[j].instance_id;
                old_matched[j] = true;
                break;
            }
        }

        if (kept == 0) {
            kept = g_next_instance_id++;
            if (added_count < MAX_JOYSTICKS) {
                added_ids[added_count++] = kept;
            }
        }

        now[i].instance_id = kept;
    }

    for (int j = 0; j < joystick_count; j++) {
        if (!old_matched[j]) {
            if (removed_count < MAX_JOYSTICKS) {
                removed_ids[removed_count++] = g_joy[j].instance_id;
            }
        }
    }

    if (sensorlist) {
        ReleaseSensorsList(sensorlist, NULL);
    }
    sensorlist = new_list;

    joystick_count = now_count;
    for (int i = 0; i < now_count; i++) {
        g_joy[i] = now[i];
        JoySensor[i] = now[i].sensor;
    }
    for (int i = now_count; i < MAX_JOYSTICKS; i++) {
        SDL_zero(g_joy[i]);
        JoySensor[i] = NULL;
    }

    for (int i = 0; i < removed_count; i++) {
        SDL_PrivateJoystickRemoved(removed_ids[i]);
    }
    for (int i = 0; i < added_count; i++) {
        SDL_PrivateJoystickAdded(added_ids[i]);
    }
}

static bool MORPHOS_JoystickIsDevicePresent(Uint16 vendor_id, Uint16 product_id, Uint16 version, const char *name)
{
    return true;
}

static const char *MORPHOS_JoystickGetDeviceName(int device_index)
{
	if (device_index < 0 || device_index >= joystick_count) return NULL;
	
	const char *name = g_joy[device_index].name;
	return name;
}

static const char *MORPHOS_JoystickGetDevicePath(int device_index)
{
    return NULL;
}

static int MORPHOS_JoystickGetDeviceSteamVirtualGamepadSlot(int device_index)
{
    return -1;
}

static int MORPHOS_JoystickGetDevicePlayerIndex(int device_index)
{
    return -1;
}

static void MORPHOS_JoystickSetDevicePlayerIndex(int device_index, int player_index)
{
}

static SDL_GUID MORPHOS_JoystickGetDeviceGUID(int device_index)
{
	D("");
	if (device_index < 0 || device_index >= joystick_count) {
        SDL_GUID g; SDL_zero(g); return g;
    }
	
#ifdef USE_NEW_GUID
    const JoyEntry *e = &g_joy[device_index];

    return SDL_CreateJoystickGUID(SDL_HARDWARE_BUS_USB,
                                  (Uint16)e->vendor,
                                  (Uint16)e->product,
                                  0,              /* version */
                                  NULL,           /* vendor name */
                                  e->name,
                                  0, 0);
#else
    SDL_GUID guid;
    SDL_zero(guid);

    const JoyEntry *e = &g_joy[device_index];
    Uint16 *guid16 = (Uint16 *)guid.data;

    *guid16++ = SDL_Swap16LE(SDL_HARDWARE_BUS_USB);
    *guid16++ = 0;

    if (e->vendor && e->product) {
        *guid16++ = SDL_Swap16LE((Uint16)e->vendor);
        *guid16++ = 0;
        *guid16++ = SDL_Swap16LE((Uint16)e->product);
        *guid16++ = 0;
        *guid16++ = 0;
        *guid16++ = 0;
    } else {
        SDL_strlcpy((char*)guid16, e->name, sizeof(guid.data) - 4);
    }

    return guid;
#endif	
}

static SDL_JoystickID MORPHOS_JoystickGetDeviceInstanceID(int device_index)
{
    if (device_index < 0 || device_index >= joystick_count) {
        return 0;
    }
    return g_joy[device_index].instance_id;
}

static bool MORPHOS_JoystickOpen(SDL_Joystick *joystick, int device_index)
{
	D("");
    APTR sensor = (device_index >= 0 && device_index < joystick_count) ? g_joy[device_index].sensor : NULL;

	if (sensor)
	{
		size_t buttons = 0, naxes = 0, nhats = 0, nsticks = 0;
		CONST_STRPTR name = "<unknown>";
		struct joystick_hwdata *hwdata = SDL_calloc(1, sizeof(*hwdata));
		if (hwdata)
		{
			hwdata->child_sensors = ObtainSensorsListTags(
				SENSORS_Parent, (IPTR)sensor,
				SENSORS_Class, SensorClass_HID,
				TAG_DONE);

			sensor = NULL;
			hwdata->hat_inited = false;
			while ((sensor = NextSensor(sensor, hwdata->child_sensors, NULL))) 
			{
				ULONG type = SensorType_HIDInput_Unknown/*, Limb, color, id*/;
			
				if (GetSensorAttrTags(sensor, SENSORS_Type, (IPTR)&type, TAG_DONE))
				{
					switch (type) 
					{
						case SensorType_HIDInput_Trigger:
							if (buttons < MAX_BUTTONS) 
							{
								hwdata->button[buttons++] = sensor;
							}
							break;
						case SensorType_HIDInput_Stick:
							if (nhats < MAX_HATS)
							{
								hwdata->hat[nhats++] = sensor;
							}
							break;
						case SensorType_HIDInput_Analog:
						case SensorType_HIDInput_AnalogStick:
						case SensorType_HIDInput_3DStick:
							if (nsticks < MAX_STICKS) 
							{
								hwdata->stick[nsticks] = sensor;
								hwdata->stickType[nsticks] = type;
								nsticks++;
								if (type == SensorType_HIDInput_AnalogStick)
									naxes += 2;
								else if (type == SensorType_HIDInput_3DStick)
									naxes += 4;
								else
									naxes++;
							}
							break;
							case SensorType_HIDInput_Rumble:
							{
								hwdata->numRumbles++;

								ULONG limb = 0;
								ULONG rtype = 0;

								bool ok = GetSensorAttrTags(sensor,
									SENSORS_HIDInput_Limb, (IPTR)&limb,
									SENSORS_HIDInput_Rumble_Type, (IPTR)&rtype,
									TAG_DONE);

								if (ok) {
									D("Rumble detect limb=%lu rtype=%lu", limb, rtype);

									const bool is_left  = (limb == 0);
									const bool is_right = (limb == 1);

									const bool is_vibe    = (rtype == 0);
									const bool is_impulse = (rtype == 1);

									if (is_vibe && is_left) {
										hwdata->rumble[0] = sensor;
									} else if (is_vibe && is_right) {
										hwdata->rumble[1] = sensor;
									} else if (is_impulse && is_left) {
										hwdata->rumble[2] = sensor;
									} else if (is_impulse && is_right) {
										hwdata->rumble[3] = sensor;
									} else {
										D("Unknown rumble mapping limb=%lu rtype=%lu", limb, rtype);
									}
								}
								break;
							}

						case SensorType_HIDInput_Battery:
							GetSensorAttrTags(sensor, SENSORS_HID_Name, (IPTR)&name, TAG_DONE);
							// Force "Xbox360 Controller" (WIRED) to use SDL_JOYSTICK_POWER_WIRED
							const char *devname = g_joy[device_index].name;
							if (devname && SDL_strcmp(devname, "Xbox360 Controller") == 0) {
								SDL_SendJoystickPowerInfo(joystick, SDL_POWERSTATE_UNKNOWN, 100);
								hwdata->battery = sensor;
							} else {							
								hwdata->battery = NULL;
								SDL_SendJoystickPowerInfo(joystick, SDL_POWERSTATE_NO_BATTERY, -1);
							}
							break;
						case SensorType_HIDInput_Knob:
							GetSensorAttrTags(sensor, SENSORS_HID_Name, (IPTR)&name, TAG_DONE);
							D("Knob SensorType: %d name: %s", type, name);
							break;
						case SensorType_HIDInput_Wheel:
							GetSensorAttrTags(sensor, SENSORS_HID_Name, (IPTR)&name, TAG_DONE);
							D("Wheel SensorType: %d name: %s", type, name);
							break;
						default:
							GetSensorAttrTags(sensor, SENSORS_HID_Name, (IPTR)&name, TAG_DONE);
							D("Unknow SensorType: %d name: %s", type, name);
							break;
					}
				}
			}

			joystick->naxes = naxes;
			joystick->nhats = nhats;
			joystick->nbuttons = buttons;
			hwdata->numSticks = nsticks;
			joystick->hwdata = hwdata;
			joystick->name = SDL_strdup(g_joy[device_index].name);
			return true;
		}
	}
	SDL_SetError("Failed to open device\n");
	return false;
}

static bool MOS_SetRumbleMotor(APTR motor, DOUBLE power, ULONG duration_ms)
{
    if (!motor) return true;
    if (power < 0.0) power = 0.0;
    if (power > 1.0) power = 1.0;

    return SetSensorAttrTags(motor,
        SENSORS_HIDInput_Rumble_Power, (IPTR)&power,
        SENSORS_HIDInput_Rumble_Duration, duration_ms,
        TAG_DONE) != 0;
}

static bool MORPHOS_JoystickRumble(SDL_Joystick *joystick, Uint16 low, Uint16 high)
{
    struct joystick_hwdata *hw = joystick->hwdata;
    if (!hw) return SDL_Unsupported();

    const bool stop = (low == 0 && high == 0);
    const ULONG dur = stop ? 0 : 1200;

    DOUBLE l = (DOUBLE)low / 65535.0;
    DOUBLE r = (DOUBLE)high / 65535.0;

    if (!hw->rumble[0] && !hw->rumble[1]) {
        return SDL_Unsupported();
    }
    if (!hw->rumble[0] && hw->rumble[1]) l = r;
    if (!hw->rumble[1] && hw->rumble[0]) r = l;

    if (!MOS_SetRumbleMotor(hw->rumble[0], stop ? 0.0 : l, dur)) return false;
    if (!MOS_SetRumbleMotor(hw->rumble[1], stop ? 0.0 : r, dur)) return false;

    return true;
}

static bool MORPHOS_JoystickRumbleTriggers(SDL_Joystick *joystick, Uint16 left_rumble, Uint16 right_rumble)
{
    struct joystick_hwdata *hw = joystick->hwdata;
    if (!hw) return SDL_Unsupported();

    if (!hw->rumble[2] && !hw->rumble[3]) {
        return SDL_Unsupported();
    }

    const bool stop = (left_rumble == 0 && right_rumble == 0);
    const ULONG dur = stop ? 0 : 1200;

    DOUBLE l = (DOUBLE)left_rumble / 65535.0;
    DOUBLE r = (DOUBLE)right_rumble / 65535.0;

    if (!hw->rumble[2] && hw->rumble[3]) l = r;
    if (!hw->rumble[3] && hw->rumble[2]) r = l;

    if (!MOS_SetRumbleMotor(hw->rumble[2],  stop ? 0.0 : l, dur)) return false;
    if (!MOS_SetRumbleMotor(hw->rumble[3], stop ? 0.0 : r, dur)) return false;

    return true;
}

static bool MORPHOS_JoystickSetLED(SDL_Joystick *joystick, Uint8 red, Uint8 green, Uint8 blue)
{
    return SDL_Unsupported();
}

static bool MORPHOS_JoystickSendEffect(SDL_Joystick *joystick, const void *data, int size)
{
    return SDL_Unsupported();
}

static bool MORPHOS_JoystickSetSensorsEnabled(SDL_Joystick *joystick, bool enabled)
{
    return SDL_Unsupported();
}

static inline void SendAxisFiltered(struct joystick_hwdata *hw, Uint64 ts, SDL_Joystick *joy,
                                    int axis, Sint16 v)
{
    if (!hw->axis_inited || SDL_abs((int)v - (int)hw->last_axis[axis]) >= AXIS_EPS) {
        hw->last_axis[axis] = v;
        SDL_SendJoystickAxis(ts, joy, axis, v);
    }
}

static void MORPHOS_JoystickUpdate(SDL_Joystick *joystick)
{
	struct joystick_hwdata *hwdata = joystick->hwdata;
	if (hwdata) 
	{
		int i, j;
		double btn_value, bt_value, x_value, y_value, z_value, ns_value, ew_value, z_rotation;
		Uint64 timestamp = SDL_GetTicksNS();
		
		for (i = 0; i < joystick->nbuttons; i++) 
		{
			if (!GetSensorAttrTags(hwdata->button[i], SENSORS_HIDInput_Value, (IPTR)&btn_value, TAG_DONE)) {
				continue;
			}
			if ((joystick->buttons[i] && btn_value == 0.0) || (joystick->buttons[i] == 0 && btn_value > 0.0)) 
			{
				SDL_SendJoystickButton(timestamp, joystick, i, btn_value == 0.0 ? 0 : 1);
			}
		}

		for (i = 0; i < joystick->nhats; i++) 
		{
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
				SDL_SendJoystickHat(timestamp, joystick, i, value_hat);
			}
		}
		hwdata->hat_inited = true;

		j = 0;
		for (i = 0; i < hwdata->numSticks; i++) {
			switch (hwdata->stickType[i]) {

			case SensorType_HIDInput_3DStick: {
				if (!GetSensorAttrTags(hwdata->stick[i],
						SENSORS_HIDInput_X_Index, (IPTR)&x_value,
						SENSORS_HIDInput_Y_Index, (IPTR)&y_value,
						SENSORS_HIDInput_Z_Index, (IPTR)&z_value,
						SENSORS_HIDInput_Z_Rotation, (IPTR)&z_rotation,
						TAG_DONE)) {
					j += 4;
					break;
				}

				Sint16 v0 = (Sint16)(CLAMP(x_value) * SDL_JOYSTICK_AXIS_MAX);
				Sint16 v1 = (Sint16)(CLAMP(y_value) * SDL_JOYSTICK_AXIS_MAX);
				Sint16 v2 = (Sint16)(CLAMP(z_value) * SDL_JOYSTICK_AXIS_MAX);
				Sint16 v3 = (Sint16)(CLAMP(z_rotation) * SDL_JOYSTICK_AXIS_MAX);

				SendAxisFiltered(hwdata, timestamp, joystick, j+0, v0);
				SendAxisFiltered(hwdata, timestamp, joystick, j+1, v1);
				SendAxisFiltered(hwdata, timestamp, joystick, j+2, v2);
				SendAxisFiltered(hwdata, timestamp, joystick, j+3, v3);

				j += 4;
				break;
			}

			case SensorType_HIDInput_Analog: {
				if (!GetSensorAttrTags(hwdata->stick[i],
						SENSORS_HIDInput_Value, (IPTR)&btn_value,
						TAG_DONE)) {
					j += 1;
					break;
				}

				Sint16 v = (Sint16)(btn_value * SDL_JOYSTICK_AXIS_MAX);
				SendAxisFiltered(hwdata, timestamp, joystick, j, v);
				j += 1;
				break;
			}

			case SensorType_HIDInput_AnalogStick: {
				if (!GetSensorAttrTags(hwdata->stick[i],
						SENSORS_HIDInput_EW_Value, (IPTR)&ew_value,
						SENSORS_HIDInput_NS_Value, (IPTR)&ns_value,
						TAG_DONE)) {
					j += 2;
					break;
				}

				Sint16 v0 = (Sint16)(CLAMP(ew_value) * SDL_JOYSTICK_AXIS_MAX);
				Sint16 v1 = (Sint16)(CLAMP(ns_value) * SDL_JOYSTICK_AXIS_MAX);

				SendAxisFiltered(hwdata, timestamp, joystick, j+0, v0);
				SendAxisFiltered(hwdata, timestamp, joystick, j+1, v1);

				j += 2;
				break;
			}

			default:
				/* Si jamais un type inconnu arrive, ne désynchronise pas j.
				   Ici, tu ne sais pas combien d’axes il représente, donc mieux: break. */
				break;
			}
		}

		hwdata->axis_inited = true;

		if (hwdata->battery) {
			const Uint64 BATTERY_PERIOD = 1000000000ULL; /* 1 seconde */
			if (!hwdata->battery_inited || (timestamp - hwdata->last_battery_ts) >= BATTERY_PERIOD) {
				hwdata->last_battery_ts = timestamp;
				hwdata->battery_inited = true;

				if (GetSensorAttrTags(hwdata->battery, SENSORS_HIDInput_Value, (IPTR)&bt_value, TAG_DONE)) {
					int level = (int)(bt_value * 100.0);
					if (level < 0) level = 0;
					if (level > 100) level = 100;

					if (level != hwdata->last_battery_level) {
						hwdata->last_battery_level = level;
						SDL_SendJoystickPowerInfo(joystick, SDL_POWERSTATE_UNKNOWN, level);
					}
				}
			}
		}

	}
}

void MORPHOS_JoystickClose(SDL_Joystick *joystick)
{
	D("");
	struct joystick_hwdata *hwdata = joystick->hwdata;
	if (hwdata) 
	{
		(void)MORPHOS_JoystickRumble(joystick, 0, 0);
		(void)MORPHOS_JoystickRumbleTriggers(joystick, 0, 0);
		
		if (hwdata->child_sensors) 
		{
			ReleaseSensorsList(hwdata->child_sensors, NULL);
		}
		SDL_free(hwdata);
		joystick->hwdata = NULL;
	}
}

static void MORPHOS_JoystickQuit(void)
{
	D("");
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
	if (sensorlist) { ReleaseSensorsList(sensorlist, NULL); sensorlist = NULL; }
	SDL_zero(g_joy);
	SDL_zero(JoySensor);
	g_hidListDirty = true;
	joystick_count = 0;
}

static bool MORPHOS_JoystickGetGamepadMapping(int device_index, SDL_GamepadMapping *out)
{
	D("");
    return SDL_Unsupported();
}

SDL_JoystickDriver SDL_MORPHOS_JoystickDriver = {
    MORPHOS_JoystickInit,
    MORPHOS_JoystickGetCount,
    MORPHOS_JoystickDetect,
	MORPHOS_JoystickIsDevicePresent,
    MORPHOS_JoystickGetDeviceName,
	MORPHOS_JoystickGetDevicePath,
	MORPHOS_JoystickGetDeviceSteamVirtualGamepadSlot,
    MORPHOS_JoystickGetDevicePlayerIndex,
    MORPHOS_JoystickSetDevicePlayerIndex,
    MORPHOS_JoystickGetDeviceGUID,
    MORPHOS_JoystickGetDeviceInstanceID,
    MORPHOS_JoystickOpen,
    MORPHOS_JoystickRumble,
    MORPHOS_JoystickRumbleTriggers,
    MORPHOS_JoystickSetLED,
	MORPHOS_JoystickSendEffect,
	MORPHOS_JoystickSetSensorsEnabled,
    MORPHOS_JoystickUpdate,
    MORPHOS_JoystickClose,
    MORPHOS_JoystickQuit,
	MORPHOS_JoystickGetGamepadMapping
};

#endif
