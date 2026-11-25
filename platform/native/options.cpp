/* options.cpp - the native runner's options (xmen2disasm docs/phases/P6-enhance.md R1, E2): each
 * enhancement is a runtime option of the one release runner, off by default, except music_voices
 * (the user, 2026-10-24: "as an option (default on)"; 2026-10-27: its `full`, the default) and
 * sample_retime and sample_smooth (2026-10-30: "default on"), which change only the sound, and high_fps and widescreen, `auto` (the user, 2026-10-30), which
 * follow the display and change only the picture (no window: off, so every headless check is
 * unchanged).
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
#include <map>
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

extern "C" XmOptions g_opt = { 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 1, 1, 0, 0 };

struct OptDef {
    const char *name;
    const char *values[32];   /* the first is the default (off, but music_voices'); null-terminated */
    const char *what;
    std::string value;
};

static OptDef s_defs[] = {
    { "full_rate_2p", { "0", "1", nullptr }, "two players at the full 60 Hz (R3)", "0" },
    /* auto (the user, 2026-10-30: "let's use auto for refresh rate and aspect ratio"): the display's,
       read when the window opens (options_detect_display); no window or no answer: off */
    { "high_fps", { "auto", "0", "120", "144", "165", "240", nullptr },
      "display rate in Hz with frames interpolated between logic frames, 0 = off (R4); auto = the display's rate when it is over 60 Hz, else off", "auto" },
    /* the vsync work (the user, 2026-10-30: "I agree this should be an option"; its default is the
       user's call, not yet made: timer, as before) */
    { "pacing", { "timer", "display", "auto", nullptr },
      "when pictures are shown: timer = by the PC's clock, as before; display = on the display's refreshes (vsync), each picture at the refresh nearest its time, and with high_fps each refresh shows the game at its own moment (high_fps is then at most that rate); auto = display", "timer" },
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
    /* (GitHub issue #2, "Level Select"): the values are k_start_stages' names, in the game's order */
    { "start_stage", { "0", "sentinels", "sentinels_b1", "sentinels_a1", "sentinels_b2", "avalon", "avalon_b2",
                       "avalon_a2", "avalon_a3", "avalon_a4", "banimaza", "banimaza_a1", "banimaza_a2", "savage",
                       "savage_b1", "savage_a2", "savage_a3", "savage_a4", "space", "space_a1", "clone", "clone_a2",
                       "clone_a5", nullptr },
      "cheat: every new game starts at this stage, and power-on goes there in place of Siberia's demo: a world's name = its Cerebro briefing (sentinels, avalon, banimaza, savage, space, clone), WORLD_XN = the player select before that later stage; 0 = Siberia (the original)", "0" },
    /* gameplay tweaks (the user, 2026-10-22; a balance prototype, not a phase-6 criterion) */
    { "magneto_flight", { "0", "1", nullptr },
      "tweak: Magneto moves left/right at his walking speed while hovering, 0 = he only turns (the original)", "0" },
    /* (the user, 2026-10-27) */
    { "select_idles", { "0", "1", nullptr },
      "tweak: on the player select each character plays its idle animation now and then, as when standing in a level, and Magneto picked does his standing shot with the flux bomb's explosion sound; Gambit's idle, which the game plays only as his look-up (hold Up), never while standing, plays here and standing in the stages, half as often as most others'; 0 = only the standing pose, Magneto's pick silent, no standing Gambit idle (the original)", "0" },
    /* phase 7 (P7-multiplayer.md R7) */
    { "players", { "2", "3", "4", nullptr }, "players at once: 2 (the original), 3 or 4 (pads 3 and 4 join with Start)", "2" },
    /* sound (the user, 2026-10-24; on by default; full: 2026-10-27, the default) */
    { "music_voices", { "full", "1", "0", nullptr },
      "the music on sound chips of its own, the players' parts on further sets, so no part or sound effect takes its notes, and each player's part in full, as one player hears it; 1 = the same with the game's arrangement for two or more players (each part with one track muted); 0 = one set of chips, as the original", "full" },
    /* sound (the user, 2026-10-30: "1/2 yes unless there's a cost, let's have it, and default on") */
    { "sample_retime", { "1", "0", nullptr },
      "the sound driver's samples (the YM2612's DAC, in the music and the effects) played at a steady rate, up to 4 ms late, where the driver gets them to the chip unevenly; 0 = as the driver writes them (the original)", "1" },
    /* (the user, 2026-10-30: "yes add the option as well"; its default: "if ym3438 is the cleaner
       version and we don't have any reason to think the composers wrote around ym2612 limitations
       intentionally, I'd default to 3438"; 2026-10-31: "make the ym26 the default, not the 34") */
    { "fm_chip", { "ym2612", "ym3438", nullptr },
      "the FM sound chip modelled, on every sound machine (music, effects and samples): ym2612 = the YM2612, the Mega Drive's chip (the original); ym3438 = the YM3438, the same chip without the YM2612's DAC distortion (the ladder effect)", "ym2612" },
    /* (the user, 2026-10-30: "likewise, default on, separate option") */
    { "sample_smooth", { "1", "0", nullptr },
      "the sound driver's samples with a straight line between each value and the next at the chip's own rate, where the original holds each value until the next (a stair-step); with sample_retime=0 the lines lead to the values at the driver's times; 0 = held (the original)", "1" },
    /* (a player's request, 2026-11-22; backlog 2026-11-23): any path, "" = none (the original picture) */
    { "shader_preset", { "", nullptr },
      "a RetroArch shader preset (.slangp) the picture goes through, in played runs (librashader.dll / librashader.so beside the game; F8 turns it off and on): a path, relative ones from the game's folder; empty = none, the plain picture (the original)", "" },
    /* (a player's report, 2026-11-24: the 16:9 view at 1080p looked uneven; the user, 2026-11-25: all
       three, a key to switch, "for now make C the default") */
    { "scaling", { "sharp_bilinear", "integer", "nearest", nullptr },
      "how the picture is enlarged to the window, in played runs (F4 switches it): sharp_bilinear = filling the window with every game pixel the same size, a one-screen-pixel blend where two meet; integer = the largest whole multiple that fits, sharp, with a border; nearest = filling the window, sharp, with some pixels a screen pixel wider or taller than others at most sizes (as before). At a whole multiple, such as the 3x window, all three are the same", "sharp_bilinear" },
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
    if (!strcmp(d.name, "shader_preset"))   /* a path: one line, and no comma (--options K=V,K=V) */
        return v.find_first_of(",\r\n") == std::string::npos;
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

/* every section's keys as read (and as written since): the launcher's [launcher] and [netplay] */
static std::map<std::string, std::map<std::string, std::string>> s_ini;

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
        std::string k = trim(s.substr(0, eq)), v = trim(s.substr(eq + 1));
        if (section == "[options]") set(k, v, s_ini_path.c_str());
        else if (section == "[rom]" && k == "path") s_ini_rom = v;
        if (section.size() > 2) s_ini[section.substr(1, section.size() - 2)][k] = v;
    }
    fclose(f);
}

static void apply(void)
{
    g_opt.full_rate_2p = find("full_rate_2p")->value == "1";
    const std::string &hf = find("high_fps")->value, &ws = find("widescreen")->value;
    g_opt.high_fps = hf == "auto" ? s_auto_hz : atoi(hf.c_str());
    g_opt.pacing = find("pacing")->value != "timer";
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
    g_opt.sample_retime = find("sample_retime")->value == "1";
    g_opt.sample_smooth = find("sample_smooth")->value == "1";
    g_opt.fm_chip = find("fm_chip")->value == "ym3438";
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

/* scaling (host_sdl.cpp draw): F4 goes through the three in the table's order */
int options_scaling(void)
{
    const std::string &v = find("scaling")->value;
    return v == "integer" ? SCALE_INTEGER : v == "nearest" ? SCALE_NEAREST : SCALE_SHARP_BILINEAR;
}

void options_toggle_scaling(void)
{
    static const char *const said[] = { "SCALING: EVEN PIXELS", "SCALING: WHOLE NUMBER", "SCALING: SHARP" };
    int m = (options_scaling() + 1) % 3;
    OptDef *d = find("scaling");
    d->value = d->values[m];   /* the table's order is the modes' */
    host_osd(said[m]);
}

void options_set_widescreen(int on) { find("widescreen")->value = on ? "16:9" : "0"; g_opt.widescreen = on; }

/* Netplay (phase 7 R10, R12): the options that change the game are the host's, sealed into the
   session; each player keeps the presentation ones: high_fps, music_voices, sample_retime,
   sample_smooth and fm_chip (the sound only: not in the canonical digest), and widescreen unless the
   sealed widescreen_gameplay is 1. The text is `k=v,...` in table order. */
static bool presentation_only(const OptDef &d, bool ws_gameplay)
{
    return !strcmp(d.name, "high_fps") || !strcmp(d.name, "pacing") || !strcmp(d.name, "shader_preset") ||
           !strcmp(d.name, "scaling") ||
           !strcmp(d.name, "music_voices") ||
           !strcmp(d.name, "sample_retime") ||
           !strcmp(d.name, "sample_smooth") || !strcmp(d.name, "fm_chip") ||
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

/* start_stage's scenes: g_SceneIndex in Scene_RegisterAll's order (game/c/0054C6.c; SiberiaB1 is 0).
   A world starts at its Cerebro briefing (which runs the player select), a later stage at the player
   select registered before it; SentinelsB2 follows SentinelsA1 with no select, and Game_ResetPlayers
   puts the select first for it. These are tools/ws_warp.py's starts (WORLDS, LATER), all played there. */
static const struct { const char *name; int scene; } k_start_stages[] = {
    { "sentinels", 1 }, { "sentinels_b1", 3 }, { "sentinels_a1", 5 }, { "sentinels_b2", 7 },
    { "avalon", 8 }, { "avalon_b2", 10 }, { "avalon_a2", 12 }, { "avalon_a3", 14 }, { "avalon_a4", 16 },
    { "banimaza", 18 }, { "banimaza_a1", 20 }, { "banimaza_a2", 22 },
    { "savage", 24 }, { "savage_b1", 26 }, { "savage_a2", 28 }, { "savage_a3", 30 }, { "savage_a4", 32 },
    { "space", 34 }, { "space_a1", 36 },
    { "clone", 38 }, { "clone_a2", 40 }, { "clone_a5", 42 },
};

/* Game_ResetPlayers' first scene: --scene N, else XMEN2_DEBUG_SCENE=n (development tests), else the
   start_stage option's, else `scene` (the original's SiberiaB1) */
extern "C" int plat_first_scene(int scene)
{
    if (s_first_scene >= 0) return s_first_scene;
    const char *e = getenv("XMEN2_DEBUG_SCENE");
    if (e && *e) return atoi(e);
    const std::string &v = find("start_stage")->value;
    for (const auto &s : k_start_stages)
        if (v == s.name) return s.scene;
    return scene;
}

std::string options_exe_dir(void) { return s_exe_dir; }
std::string options_shader_preset(void) { return find("shader_preset")->value; }
std::string options_ini_rom(void) { return s_ini_rom; }

std::string options_ini_get(const char *section, const char *key, const std::string &def)
{
    auto s = s_ini.find(section);
    if (s == s_ini.end()) return def;
    auto k = s->second.find(key);
    return k == s->second.end() ? def : k->second;
}

/* xmen2.ini's [section] becomes kv (after a `# comment` line, if any; no kv: the section goes), the rest
   of the file as it was (a new file if none) */
bool options_ini_write(const char *section, const std::vector<std::pair<std::string, std::string>> &kv,
                       const char *comment)
{
    std::string head = std::string("[") + section + "]";
    std::vector<std::string> keep;
    if (FILE *f = fopen(s_ini_path.c_str(), "r")) {
        char line[4096];
        bool in = false;
        while (fgets(line, sizeof line, f)) {
            std::string s = trim(line);
            if (!s.empty() && s[0] == '[') in = s == head;
            if (in) continue;
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
    if (!kv.empty()) {
        fprintf(f, "%s%s\n", keep.empty() ? "" : "\n", head.c_str());
        if (comment && *comment) fprintf(f, "# %s\n", comment);
        for (const auto &p : kv) fprintf(f, "%s = %s\n", p.first.c_str(), p.second.c_str());
    }
    bool ok = fclose(f) == 0;
    if (ok) {
        s_ini_found = 1;
        auto &m = s_ini[section];
        m.clear();
        for (const auto &p : kv) m[p.first] = p.second;
    }
    return ok;
}

/* xmen2.ini's [rom] section becomes path = PATH */
bool options_remember_rom(const std::string &path)
{
    bool ok = options_ini_write("rom", {{"path", path}}, "the ROM the game plays when started without one (set when you chose it)");
    if (ok) s_ini_rom = path;
    return ok;
}

/* The table, for the launcher (launcher.cpp): names, values (the default first), what each does */
int options_count(void) { return NDEFS; }
const char *options_name(int i) { return s_defs[i].name; }
const char *options_what(int i) { return s_defs[i].what; }
std::vector<std::string> options_values(int i)
{
    std::vector<std::string> v;
    for (int k = 0; s_defs[i].values[k]; k++) v.push_back(s_defs[i].values[k]);
    return v;
}
std::string options_get(int i) { return s_defs[i].value; }
bool options_set(int i, const std::string &v)
{
    if (!valid(s_defs[i], v)) return false;
    s_defs[i].value = v;
    apply();
    return true;
}
bool options_presentation_only(int i) { return presentation_only(s_defs[i], false) || !strcmp(s_defs[i].name, "widescreen"); }

/* [options]: every option not at its default, in table order */
bool options_save(void)
{
    std::vector<std::pair<std::string, std::string>> kv;
    for (const OptDef &d : s_defs)
        if (d.value != d.values[0]) kv.emplace_back(d.name, d.value);
    return options_ini_write("options", kv, "changed from the defaults (the launcher writes this section; the README lists every option)");
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
