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
#include "../SDL_syslocale.h"

#include <proto/dos.h>
#include <proto/locale.h>

/* '?' in a name: accented letter (Latin-1 or UTF-8) */
static const struct
{
    const char *name;
    char lang[3];
    char country[3];
} mos_languages[] = {
    { "english",    "en", "GB" },
    { "deutsch",    "de", "DE" },
    { "fran?ais",   "fr", "FR" },
    { "italiano",   "it", "IT" },
    { "espa?ol",    "es", "ES" },
    { "catal?",     "ca", "ES" },
    { "dansk",      "da", "DK" },
    { "greek",      "el", "GR" },
    { "hrvatski",   "hr", "HR" },
    { "magyar",     "hu", "HU" },
    { "malti",      "mt", "MT" },
    { "nederlands", "nl", "NL" },
    { "norsk",      "nb", "NO" },
    { "polski",     "pl", "PL" },
    { "portugu?s",  "pt", "PT" },
    { "shqipja",    "sq", "AL" },
    { "suomi",      "fi", "FI" },
    { "svenska",    "sv", "SE" },
    { "t?rk?e",     "tr", "TR" },
    { "?e?tina",    "cs", "CZ" },
    { "?slenska",   "is", "IS" },
};

/* loc_CountryCode: vehicle registration code, left justified */
#define IVS(a, b, c) ((ULONG)(a) << 24 | (ULONG)(b) << 16 | (ULONG)(c) << 8)

static const struct
{
    ULONG ivs;
    char iso[3];
    const char *langs;  /* languages above spoken there, 2 letters each */
} mos_countries[] = {
    { IVS('F', 0, 0),     "FR", "fr" },
    { IVS('D', 0, 0),     "DE", "de" },
    { IVS('G', 'B', 0),   "GB", "en" },
    { IVS('U', 'K', 0),   "GB", "en" },
    { IVS('U', 'S', 'A'), "US", "en" },
    { IVS('I', 0, 0),     "IT", "it" },
    { IVS('E', 0, 0),     "ES", "esca" },
    { IVS('C', 'H', 0),   "CH", "defrit" },
    { IVS('A', 0, 0),     "AT", "de" },
    { IVS('B', 0, 0),     "BE", "nlfrde" },
    { IVS('L', 0, 0),     "LU", "frde" },
    { IVS('N', 'L', 0),   "NL", "nl" },
    { IVS('C', 'D', 'N'), "CA", "enfr" },
    { IVS('A', 'U', 'S'), "AU", "en" },
    { IVS('N', 'Z', 0),   "NZ", "en" },
    { IVS('I', 'R', 'L'), "IE", "en" },
    { IVS('A', 'L', 0),   "AL", "sq" },
    { IVS('A', 'N', 'D'), "AD", "ca" },
    { IVS('B', 'G', 0),   "BG", "" },
    { IVS('B', 'I', 'H'), "BA", "hr" },
    { IVS('B', 'R', 0),   "BR", "pt" },
    { IVS('C', 'Z', 0),   "CZ", "cs" },
    { IVS('D', 'K', 0),   "DK", "da" },
    { IVS('F', 'I', 'N'), "FI", "fisv" },
    { IVS('S', 'F', 0),   "FI", "fisv" },
    { IVS('F', 'L', 0),   "LI", "de" },
    { IVS('G', 'R', 0),   "GR", "el" },
    { IVS('H', 0, 0),     "HU", "hu" },
    { IVS('H', 'R', 0),   "HR", "hr" },
    { IVS('I', 'R', 0),   "IR", "" },
    { IVS('I', 'S', 0),   "IS", "is" },
    { IVS('L', 'T', 0),   "LT", "" },
    { IVS('M', 0, 0),     "MT", "mten" },
    { IVS('M', 'C', 0),   "MC", "fr" },
    { IVS('N', 0, 0),     "NO", "nb" },
    { IVS('P', 0, 0),     "PT", "pt" },
    { IVS('P', 'L', 0),   "PL", "pl" },
    { IVS('R', 'O', 0),   "RO", "" },
    { IVS('R', 'S', 'M'), "SM", "it" },
    { IVS('R', 'U', 'S'), "RU", "" },
    { IVS('S', 0, 0),     "SE", "sv" },
    { IVS('S', 'K', 0),   "SK", "" },
    { IVS('S', 'L', 'O'), "SI", "" },
    { IVS('T', 0, 0),     "TH", "" },
    { IVS('T', 'R', 0),   "TR", "tr" },
    { IVS('U', 'A', 0),   "UA", "" },
};

static int MOS_FindLanguage(const char *name)
{
    int i;

    for (i = 0; name && i < (int)SDL_arraysize(mos_languages); i++) {
        const unsigned char *s = (const unsigned char *)name;
        const char *p = mos_languages[i].name;

        for (; *p; p++) {
            if (*p == '?') {
                if (*s < 0x80) {
                    break;
                }
                while (*s >= 0x80) {
                    s++;
                }
            } else if (SDL_tolower(*s++) != *p) {
                break;
            }
        }
        if (!*p && (!*s || SDL_strcasecmp((const char *)s, ".language") == 0)) {
            return i;
        }
    }
    return -1;
}

static int MOS_FindCountry(ULONG code)
{
    int i;

    for (i = 0; i < (int)SDL_arraysize(mos_countries); i++) {
        if (mos_countries[i].ivs == code) {
            return i;
        }
    }
    return -1;
}

static void MOS_AddLocale(char *buf, size_t buflen, int l, int c)
{
    const char *country;
    const char *s;
    char item[7];

    if (l < 0) {
        return;
    }
    country = mos_languages[l].country;
    for (s = (c < 0) ? "" : mos_countries[c].langs; *s; s += 2) {
        if (s[0] == mos_languages[l].lang[0] && s[1] == mos_languages[l].lang[1]) {
            country = mos_countries[c].iso;
            break;
        }
    }
    SDL_snprintf(item, sizeof(item), "%s%s_%s", *buf ? "," : "", mos_languages[l].lang, country);
    if (!SDL_strstr(buf, item + (*buf ? 1 : 0))) {
        SDL_strlcat(buf, item, buflen);
    }
}

bool SDL_SYS_GetPreferredLocales(char *buf, size_t buflen)
{
    struct Locale *loc = LocaleBase ? OpenLocale(NULL) : NULL;

    *buf = '\0';
    if (loc) {
        const int c = MOS_FindCountry(loc->loc_CountryCode);
        int i;

        for (i = 0; i < 10 && loc->loc_PrefLanguages[i]; i++) {
            MOS_AddLocale(buf, buflen, MOS_FindLanguage((const char *)loc->loc_PrefLanguages[i]), c);
        }
        if (!*buf) {
            MOS_AddLocale(buf, buflen, MOS_FindLanguage((const char *)loc->loc_LanguageName), c);
        }
        CloseLocale(loc);
    }
    if (!*buf) {
        char language[80];

        if (GetVar("LANGUAGE", language, sizeof(language), 0) > 0) {
            MOS_AddLocale(buf, buflen, MOS_FindLanguage(language), -1);
        }
    }
    if (!*buf) {
        SDL_strlcpy(buf, "en_GB", buflen);
    }
    D("'%s'", buf);
    return true;
}
