/* options.cpp - the native runner's options (xmen2disasm docs/phases/P6-enhance.md R1, E2): each
 * enhancement is a runtime option of the one release runner, off by default, except music_voices
 * (the user, 2026-10-24: "as an option (default on)"; 2026-10-27: its `full`, the default), which
 * changes only the sound, and high_fps and widescreen, `auto` (the user, 2026-10-30), which follow
 * the display and change only the picture (no window: off, so every headless check is unchanged).
 *
 *   --options --json     print the options (default, values, the effective value) and exit 0
 *   --options K=V,K=V    set them for this run
 *   xmen2.ini            beside the runner: [options] K=V lines set them too; the command line wins.
 *                        Its [rom] path = PATH is the ROM a run started without one plays (main.cpp)
 *
 * The game's C reads them through g_opt (platform/xmen2_platform.h, native-only branches); with
 * every option at its default no option code runs in the game (music_voices is the platform's).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <utility>
#include <vector>

#include "host_sdl.h"
#include "native.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif

extern "C" XmOptions g_opt = { 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0 };

struct OptDef {
    const char *name;
    const char *values[10];   /* the first is the default (off, but music_voices'); null-terminated */
    const char *what;
    std::string value;
};

static OptDef s_defs[] = {
    { "full_rate_2p", { "0", "1", nullptr }, "two players at the full 60 Hz (R3)", "0" },
    /* auto (the user, 2026-10-30: "let's use auto for refresh rate and aspect ratio"): the display's,
       read when the window opens (options_detect_display); no window or no answer: off */
    { "high_fps", { "auto", "0", "120", "144", "165", "240", nullptr },
      "display rate in Hz with frames interpolated between logic frames, 0 = off (R4); auto = the display's rate when it is over 60 Hz, else off", "auto" },
    { "widescreen", { "auto", "0", "16:9", nullptr },
      "16:9 view in levels, 0 = off (R5-R8); auto = 16:9 on a display wider than 3:2, else off", "auto" },
    { "widescreen_gameplay", { "0", "1", nullptr },
      "with widescreen: objects activate across the wide view and the camera keeps it inside the level (R6); 0 = the view is presentation only, the game plays as in 4:3 (R8)", "0" },
    /* cheats (the user, 2026-10-21; not phase-6 criteria) */
    { "lives", { "0", "inf", "1", "5", "20", "99", nullptr },
      "cheat: lives at a new game (any 1-99; 0 = the original's 9), inf = lives are never spent", "0" },
    { "magneto", { "0", "1", nullptr }, "cheat: Magneto selectable from the start, 0 = from Bani Maza", "0" },
    { "start_char", { "0", "beast", "cyclops", "gambit", "nightcrawler", "psylocke", "wolverine", "magneto", nullptr },
      "cheat: player 1's character in Siberia at power-on, 0 = at random (the original)", "0" },
    /* gameplay tweaks (the user, 2026-10-22; a balance prototype, not a phase-6 criterion) */
    { "magneto_flight", { "0", "1", nullptr },
      "tweak: Magneto moves left/right at his walking speed while hovering, 0 = he only turns (the original)", "0" },
    /* (the user, 2026-10-27) */
    { "select_idles", { "0", "1", nullptr },
      "tweak: on the player select each character plays its idle animation now and then, as when standing in a level, and Magneto picked does his standing shot with the flux bomb's explosion sound; Gambit's idle, which the game never plays, plays here and in the stages, half as often as most others'; 0 = only the standing pose, Magneto's pick silent, no Gambit idle (the original)", "0" },
    /* phase 7 (P7-multiplayer.md R7) */
    { "players", { "2", "3", "4", nullptr }, "players at once: 2 (the original), 3 or 4 (pads 3 and 4 join with Start)", "2" },
    /* sound (the user, 2026-10-24; on by default; full: 2026-10-27, the default) */
    { "music_voices", { "full", "1", "0", nullptr },
      "the music on sound chips of its own, the players' parts on further sets, so no part or sound effect takes its notes, and each player's part in full, as one player hears it; 1 = the same with the game's arrangement for two or more players (each part with one track muted); 0 = one set of chips, as the original", "full" },
};
static const int NDEFS = (int)(sizeof s_defs / sizeof s_defs[0]);
static std::string s_ini_path, s_exe_dir;
static int s_ini_found;
static std::string s_ini_rom;   /* [rom] path: the ROM a run without one on the command line plays */
static int s_auto_hz, s_auto_wide;   /* what auto means on this display (0 until a window has looked) */

static OptDef *find(const std::string &k)
{
    for (OptDef &d : s_defs)
        if (k == d.name) return &d;
    return nullptr;
}

/* high_fps takes any rate from 61 to 1000 Hz besides the listed ones, lives any count 1-99 */
static bool valid(const OptDef &d, const std::string &v)
{
    for (int i = 0; d.values[i]; i++)
        if (v == d.values[i]) return true;
    char *e;
    long n = strtol(v.c_str(), &e, 10);
    if (v.empty() || *e) return false;
    if (!strcmp(d.name, "high_fps")) return n >= 61 && n <= 1000;
    if (!strcmp(d.name, "lives")) return n >= 1 && n <= 99;   /* the select screen shows 2 digits */
    return false;
}

static std::string trim(const std::string &s)
{
    size_t a = s.find_first_not_of(" \t\r\n"), b = s.find_last_not_of(" \t\r\n");
    return a == std::string::npos ? std::string() : s.substr(a, b - a + 1);
}

static void set(const std::string &k, const std::string &v, const char *from)
{
    OptDef *d = find(k);
    if (!d) { fprintf(stderr, "[native] %s: unknown option %s\n", from, k.c_str()); exit(2); }
    if (!valid(*d, v)) { fprintf(stderr, "[native] %s: %s=%s is not one of its values\n", from, k.c_str(), v.c_str()); exit(2); }
    d->value = v;
}

static std::string exe_dir(const char *argv0)
{
    char buf[4096] = "";
#ifdef _WIN32
    DWORD n = GetModuleFileNameA(nullptr, buf, (DWORD)sizeof buf);
    if (n == 0 || n >= sizeof buf) snprintf(buf, sizeof buf, "%s", argv0);
#else
    ssize_t n = readlink("/proc/self/exe", buf, sizeof buf - 1);
    if (n > 0) buf[n] = 0; else snprintf(buf, sizeof buf, "%s", argv0);
#endif
    std::string p = buf;
    size_t s = p.find_last_of("/\\");
    return s == std::string::npos ? std::string(".") : p.substr(0, s);
}

static void read_ini(void)
{
    FILE *f = fopen(s_ini_path.c_str(), "r");
    if (!f) return;
    s_ini_found = 1;
    char line[4096];
    std::string section;
    while (fgets(line, sizeof line, f)) {
        std::string s = trim(line);
        if (s.empty() || s[0] == '#' || s[0] == ';') continue;
        if (s[0] == '[') { section = s; continue; }
        size_t eq = s.find('=');
        if (eq == std::string::npos) continue;
        if (section == "[options]") set(trim(s.substr(0, eq)), trim(s.substr(eq + 1)), s_ini_path.c_str());
        else if (section == "[rom]" && trim(s.substr(0, eq)) == "path") s_ini_rom = trim(s.substr(eq + 1));
    }
    fclose(f);
}

static void apply(void)
{
    g_opt.full_rate_2p = find("full_rate_2p")->value == "1";
    const std::string &hf = find("high_fps")->value, &ws = find("widescreen")->value;
    g_opt.high_fps = hf == "auto" ? s_auto_hz : atoi(hf.c_str());
    g_opt.widescreen = ws == "16:9" || (ws == "auto" && s_auto_wide);
    g_opt.widescreen_gameplay = find("widescreen_gameplay")->value == "1";
    const std::string &l = find("lives")->value;
    g_opt.lives = l == "inf" ? -1 : atoi(l.c_str());
    g_opt.magneto = find("magneto")->value == "1";
    const OptDef *sc = find("start_char");
    g_opt.start_char = 0;
    for (int i = 1; sc->values[i]; i++)   /* the values after "0" in g_P1CharIndex's order */
        if (sc->value == sc->values[i]) g_opt.start_char = i;
    g_opt.magneto_flight = find("magneto_flight")->value == "1";
    g_opt.select_idles = find("select_idles")->value == "1";
    g_opt.players = atoi(find("players")->value.c_str());
    const std::string &mv = find("music_voices")->value;
    g_opt.music_voices = mv == "full" ? 2 : mv == "1";
}

static std::string json_str(const std::string &s)
{
    std::string o = "\"";
    for (char c : s) {
        if (c == '"' || c == '\\') o += '\\';
        if ((unsigned char)c < 0x20) { char b[8]; snprintf(b, sizeof b, "\\u%04x", c); o += b; continue; }
        o += c;
    }
    return o + "\"";
}

/* an option's value with auto resolved: what this run plays with */
static std::string effective(const OptDef &d)
{
    if (d.value != "auto") return d.value;
    if (!strcmp(d.name, "high_fps")) return std::to_string(g_opt.high_fps);
    if (!strcmp(d.name, "widescreen")) return g_opt.widescreen ? "16:9" : "0";
    return d.value;
}

/* {"opt": "v", ...}: the effective values (the trace's "options"), auto resolved */
std::string options_effective_json(void)
{
    std::string o = "{";
    for (int i = 0; i < NDEFS; i++)
        o += (i ? ", " : "") + json_str(s_defs[i].name) + ": " + json_str(effective(s_defs[i]));
    return o + "}";
}

/* The display the window opens on (host_sdl.cpp): w x h desktop pixels, refresh in Hz (0 = unknown).
   high_fps=auto: its rate when over 61 Hz (one under a common rate, as 143 for 143.98, rounds up),
   else off; widescreen=auto: 16:9 when it is wider than 3:2 (16:10, 16:9, ultrawide), else off.
   Anything unknown leaves the original picture. aspect=false: only the rate (a later display). */
void options_detect_display(int w, int h, int hz, bool aspect)
{
    static const int common[] = { 60, 75, 90, 100, 120, 144, 165, 180, 200, 240, 280, 360, 480 };
    for (int c : common)
        if (hz + 1 == c) hz = c;
    s_auto_hz = hz > 61 && hz <= 1000 ? hz : 0;
    if (aspect) s_auto_wide = w > 0 && h > 0 && w * 2 > h * 3;
    const std::string &hf = find("high_fps")->value, &ws = find("widescreen")->value;
    if (hf == "auto") g_opt.high_fps = s_auto_hz;
    if (aspect && ws == "auto") g_opt.widescreen = s_auto_wide;
    fprintf(stderr, "[native] display %dx%d at %d Hz: high_fps %s, widescreen %s\n", w, h, hz,
            effective(*find("high_fps")).c_str(), effective(*find("widescreen")).c_str());
}

bool options_high_fps_auto(void) { return find("high_fps")->value == "auto"; }

static void print_json(void)
{
    std::string o = "{\"options\": {";
    for (int i = 0; i < NDEFS; i++) {
        const OptDef &d = s_defs[i];
        o += (i ? ", " : "") + json_str(d.name) + ": {\"default\": " + json_str(d.values[0]) + ", \"values\": [";
        for (int k = 0; d.values[k]; k++) o += (k ? ", " : "") + json_str(d.values[k]);
        o += "], \"value\": " + json_str(d.value) + ", \"what\": " + json_str(d.what) + "}";
    }
    o += "}, \"effective\": " + options_effective_json() + ", \"settings_file\": " + json_str(s_ini_path) +
         ", \"settings_file_found\": " + (s_ini_found ? "true" : "false") + "}";
    printf("%s\n", o.c_str());
    fflush(stdout);
}

/* Played runs (host_sdl.cpp, R1 "keys for the two presentation options"): F9 turns high_fps off
   and back to its rate (144 if none was given); F10 asks for the 16:9 view on or off, which takes
   effect at the next screen (widescreen.cpp: the camera, the streamer and what activates follow it,
   so it starts with a scene). */
void options_toggle_high_fps(void)
{
    static int rate = 0;
    if (!rate) rate = s_auto_hz ? s_auto_hz : 144;   /* turned on: the display's rate if known */
    if (g_opt.high_fps) { rate = g_opt.high_fps; g_opt.high_fps = 0; }
    else g_opt.high_fps = rate;
    find("high_fps")->value = std::to_string(g_opt.high_fps);
    present_reset();
    host_osd(g_opt.high_fps ? ("HIGH FPS: " + std::to_string(g_opt.high_fps)).c_str() : "HIGH FPS: OFF");
}

/* Online the 16:9 view is the host's with widescreen_gameplay (R10: it changes what activates and the
   camera's limits); presentation only it is each seat's own (phase 7 R12) and F10 works */
void options_toggle_widescreen(void)
{
    if (netplay_enabled() && g_opt.widescreen_gameplay) {
        fprintf(stderr, "[native] F10: the 16:9 view is the host's in a netplay session with widescreen_gameplay=1\n");
        host_osd("16:9 GAMEPLAY SET BY HOST");
        return;
    }
    host_osd(ws_request_toggle() ? "16:9 FROM THE NEXT SCREEN" : "4:3 FROM THE NEXT SCREEN");
}

void options_set_widescreen(int on) { find("widescreen")->value = on ? "16:9" : "0"; g_opt.widescreen = on; }

/* Netplay (phase 7 R10, R12): the options that change the game are the host's, sealed into the
   session; each player keeps the presentation ones: high_fps, music_voices (the sound only: not in
   the canonical digest), and widescreen unless the sealed widescreen_gameplay is 1. The text is
   `k=v,...` in table order. */
static bool presentation_only(const OptDef &d, bool ws_gameplay)
{
    return !strcmp(d.name, "high_fps") || !strcmp(d.name, "music_voices") ||
           (!strcmp(d.name, "widescreen") && !ws_gameplay);
}

std::string options_game_text(void)
{
    bool ws_gameplay = find("widescreen_gameplay")->value == "1";
    std::string o;
    for (const OptDef &d : s_defs)
        if (!presentation_only(d, ws_gameplay))   /* auto as the host resolved it: a guest's display must not decide */
            o += (o.empty() ? "" : ",") + std::string(d.name) + "=" + effective(d);
    return o;
}

/* The host's text: refused (false), changing nothing, when it names an option this build lacks, a
   value it can't take or a presentation option */
bool options_adopt_game(const std::string &text)
{
    std::vector<std::pair<OptDef *, std::string>> kvs;
    bool ws_gameplay = false;
    for (size_t p = 0; p < text.size(); ) {
        size_t c = text.find(',', p);
        if (c == std::string::npos) c = text.size();
        std::string kv = text.substr(p, c - p);
        p = c + 1;
        size_t eq = kv.find('=');
        OptDef *d = eq == std::string::npos ? nullptr : find(kv.substr(0, eq));
        if (!d || !valid(*d, kv.substr(eq + 1))) return false;
        kvs.emplace_back(d, kv.substr(eq + 1));
        if (!strcmp(d->name, "widescreen_gameplay")) ws_gameplay = kvs.back().second == "1";
    }
    for (const auto &kv : kvs)
        if (presentation_only(*kv.first, ws_gameplay)) return false;
    for (const auto &kv : kvs) kv.first->value = kv.second;
    apply();
    return true;
}

/* --scene N (phase 7's stage inputs, `trace --scene`) or XMEN2_DEBUG_SCENE=n: a new game starts at
   scene n */
static int s_first_scene = -1;
void options_set_first_scene(int scene) { s_first_scene = scene; }
int options_first_scene(void) { return s_first_scene; }

extern "C" int plat_debug_first_scene(int scene)
{
    if (s_first_scene >= 0) return s_first_scene;
    const char *e = getenv("XMEN2_DEBUG_SCENE");
    return e && *e ? atoi(e) : scene;
}

std::string options_exe_dir(void) { return s_exe_dir; }
std::string options_ini_rom(void) { return s_ini_rom; }

/* xmen2.ini's [rom] section becomes path = PATH (the rest of the file as it was; a new file if none) */
bool options_remember_rom(const std::string &path)
{
    std::vector<std::string> keep;
    if (FILE *f = fopen(s_ini_path.c_str(), "r")) {
        char line[4096];
        bool in_rom = false;
        while (fgets(line, sizeof line, f)) {
            std::string s = trim(line);
            if (!s.empty() && s[0] == '[') in_rom = s == "[rom]";
            if (in_rom) continue;
            std::string l = line;
            while (!l.empty() && (l.back() == '\n' || l.back() == '\r')) l.pop_back();
            keep.push_back(l);
        }
        fclose(f);
    }
    while (!keep.empty() && trim(keep.back()).empty()) keep.pop_back();
    FILE *f = fopen(s_ini_path.c_str(), "w");
    if (!f) return false;
    for (const std::string &l : keep) fprintf(f, "%s\n", l.c_str());
    fprintf(f, "%s[rom]\n# the ROM the game plays when started without one (set when you chose it)\npath = %s\n",
            keep.empty() ? "" : "\n", path.c_str());
    bool ok = fclose(f) == 0;
    if (ok) s_ini_rom = path;
    return ok;
}

/* Removes --options and its argument from argv (main parses the rest); exits after --options --json. */
void options_init(int *argc, char **argv)
{
    s_exe_dir = exe_dir(argv[0]);
    s_ini_path = s_exe_dir + "/xmen2.ini";
    read_ini();
    bool list = false;
    int w = 1;
    for (int i = 1; i < *argc; i++) {
        if (!strcmp(argv[i], "--options") && i + 1 < *argc) {
            const char *a = argv[++i];
            if (!strcmp(a, "--json")) { list = true; continue; }
            std::string s = a;
            for (size_t p = 0; p <= s.size(); ) {
                size_t c = s.find(',', p);
                if (c == std::string::npos) c = s.size();
                std::string kv = trim(s.substr(p, c - p));
                p = c + 1;
                if (kv.empty()) continue;
                size_t eq = kv.find('=');
                if (eq == std::string::npos) { fprintf(stderr, "[native] --options: %s is not K=V\n", kv.c_str()); exit(2); }
                set(trim(kv.substr(0, eq)), trim(kv.substr(eq + 1)), "--options");
            }
            continue;
        }
        argv[w++] = argv[i];
    }
    *argc = w;
    argv[w] = nullptr;
    apply();
    if (list) { print_json(); exit(0); }
}
