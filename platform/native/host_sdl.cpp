/* host_sdl.cpp - the native runner's window, input and sound (phase 5 P8): the game played on the
 * PC. SDL2 for the window, the keyboard and game controllers and the audio device; the sound chips
 * and their mixer are the engine's (audio/: ymfm's YM2612, the SN76489, the cycle-stamped event
 * queue the Z80's writes go into, audio.c's device bridge).
 *
 *   The players' keys and controllers are controls.cpp's (the launcher's Controls page, xmen2.ini's
 *   [player1]..[player4]); by default the keyboard is player 1's: arrows; Z = A, X = B, C = C,
 *   Enter = Start; A = X, S = Y, D = Z, Q = Mode; game controllers are players 1-4 in plug-in order
 *   (an unplugged one frees its player): D-pad or left stick; X = A, A = B, B = C, Start;
 *   LB = X, Y = Y, RB = Z, Back = Mode; Rewind (held: Backspace, a controller's left trigger).
 *   Esc or closing the window quits. F5 quick save, F6 the next slot, F7 quick load (rewind.cpp).
 * XMEN2_PAD_SELFTEST=1: the slots checked with virtual controllers, headless (host_pad_selftest).
 * Frames are paced to the NTSC rate (59.92 Hz) by the clock (high_fps: present.cpp's clock); F11
 * toggles fullscreen, F9 high_fps, F10 widescreen (from the next screen), F8 the shader preset
 * (shader_preset, shader.cpp: the picture through a RetroArch .slangp preset), F4 the next scaling
 * (sharp_bilinear, integer, nearest: draw()).
 * The renderer is Direct3D 11 on Windows where it can be (SDL's default, Direct3D 9, loses its
 * device when the window moves to another monitor or the displays change, and the picture froze
 * while the game and its music went on); a device reset or 30 failed draws in a row rebuild the
 * renderer and its texture.
 */
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <algorithm>
#include <cmath>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

extern "C" {
#include "audio.h"
#include "audio/event_queue.h"
#include "audio/mixer.h"
#include "genesis_clocks.h"
#include "video/genesis_bus.h"
}
#include "controls.h"
#include "host_sdl.h"
#include "native.h"
#include "osd_font.h"
#include "shader.h"
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <commdlg.h>
#include <dwmapi.h>
#else
#include <sys/wait.h>
#endif

/* a run started without a ROM (main.cpp find_rom): what went wrong, and the ROM to play */
/* Windows: the program is a console program (its output is for the command line and the harness), so a
   start from Explorer gets a console window of its own, empty but for log lines. A played run that is
   the only process on its console (a double-click; a terminal or a script shares theirs) lets it go:
   the window closes, and what a player must know comes in message boxes (host_message, netplay's
   failures). Output a parent redirected (pipes) is unaffected. */
void host_detach_lone_console(void)
{
#ifdef _WIN32
    DWORD pids[2];
    if (GetConsoleWindow() && GetConsoleProcessList(pids, 2) == 1) FreeConsole();
#endif
}

void host_message(const char *text)
{
    if (getenv("XMEN2_NO_MESSAGEBOX")) {   /* tests: the text, no dialog on the desktop */
        fprintf(stderr, "[native] message: %s\n", text);
        return;
    }
    if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "X-Men 2: Clone Wars", text, nullptr) != 0)
        fprintf(stderr, "[native] %s\n", text);
}

/* a file picker: filter is Windows' ("Name\0*.a;*.b\0...\0"), glob the same patterns for zenity
   ("*.a *.b"); title has no single quote */
static bool pick_file(char *path, size_t cap, const char *filter, const char *glob, const char *title)
{
#ifdef _WIN32
    (void)glob;
    OPENFILENAMEA o = {};
    path[0] = 0;
    o.lStructSize = sizeof o;
    o.lpstrFilter = filter;
    o.lpstrFile = path;
    o.nMaxFile = (DWORD)cap;
    o.lpstrTitle = title;
    o.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
    return GetOpenFileNameA(&o) != 0;
#else
    (void)filter;
    /* zenity (GNOME and most desktops) or kdialog (KDE); neither: false */
    std::string z = std::string("zenity --file-selection --title='") + title + "' --file-filter='" + glob +
                    "' --file-filter='*' 2>/dev/null";
    std::string k = std::string("kdialog --getopenfilename . '") + glob + "' 2>/dev/null";
    const char *const cmds[] = { z.c_str(), k.c_str() };
    for (const char *cmd : cmds) {
        FILE *p = popen(cmd, "r");
        if (!p) continue;
        bool got = fgets(path, (int)cap, p) != nullptr;
        int rc = pclose(p);
        if (got) {
            size_t n = strlen(path);
            while (n && (path[n - 1] == '\n' || path[n - 1] == '\r')) path[--n] = 0;
            if (n) return true;
        }
        if (WIFEXITED(rc) && WEXITSTATUS(rc) == 1) return false;   /* it ran and was cancelled: not the next one */
    }
    return false;
#endif
}

bool host_pick_rom(char *path, size_t cap)
{
    if (const char *t = getenv("XMEN2_PICK_ROM_TEST")) {   /* testing: the picker's answer ("" = cancelled) */
        snprintf(path, cap, "%s", t);
        return *t != 0;
    }
    return pick_file(path, cap, "Genesis ROM (*.bin, *.md, *.gen)\0*.bin;*.md;*.gen\0All files\0*.*\0", "*.bin *.md *.gen",
                     "X-Men 2: Clone Wars - choose your ROM (USA/Europe)");
}

bool host_pick_shader(char *path, size_t cap)
{
    if (const char *t = getenv("XMEN2_PICK_SHADER_TEST")) {   /* testing: the picker's answer ("" = cancelled) */
        snprintf(path, cap, "%s", t);
        return *t != 0;
    }
    return pick_file(path, cap, "RetroArch shader preset (*.slangp)\0*.slangp\0All files\0*.*\0", "*.slangp",
                     "X-Men 2: Clone Wars - choose a shader preset (.slangp)");
}

static SDL_Window *s_win;
static SDL_Renderer *s_ren;
static SDL_Texture *s_tex;
static SDL_Texture *s_osd_tex;   /* shader_preset: the hotkeys' messages, drawn over the shaded picture */
static SDL_Texture *s_pre;       /* scaling=sharp_bilinear: the picture at a whole multiple, smoothed from there */
static int s_pre_w, s_pre_h;
static uint64_t s_next;         /* performance-counter time of the next frame */
static double s_period;          /* counter ticks per frame */
static bool s_at_started;        /* high_fps: the present clock has started */
static double s_at_base;         /* high_fps: performance-counter ticks at t_us 0 */
static int s_audio;
static bool s_rebuild;           /* the renderer's device was reset: make it again before drawing */
static bool s_vsync;             /* pacing=display: the renderer presents on the refresh */
static double s_disp_period;     /* counter ticks per refresh of the display the window opened on (0: unknown) */
static uint64_t s_last_flip;     /* when the last vsync'd present returned (the refresh grid off Windows) */
static double s_last_refresh;    /* the counter time of the refresh the last picture was for */
static uint64_t s_vsync_off_until; /* the display is holding presents back: the timer paces until then */
static uint64_t s_vsync_holds;   /* how often (stderr at the first) */
static int s_draw_fails;         /* failed draws in a row */
static void ptrace_open(void);   /* XMEN2_PRESENT_TRACE (below, with draw) */

/* the renderer and the frame's texture, (re)made; 0 when there is none */
static int make_renderer(void)
{
    shader_close();   /* its objects are the old device's */
    if (s_osd_tex) { SDL_DestroyTexture(s_osd_tex); s_osd_tex = nullptr; }
    if (s_pre) { SDL_DestroyTexture(s_pre); s_pre = nullptr; s_pre_w = s_pre_h = 0; }
    if (s_tex) { SDL_DestroyTexture(s_tex); s_tex = nullptr; }
    if (s_ren) { SDL_DestroyRenderer(s_ren); s_ren = nullptr; }
#ifdef _WIN32
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d11");
    s_ren = SDL_CreateRenderer(s_win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "");
#else
    if (!options_shader_preset().empty()) {   /* shader.cpp's runtime there is OpenGL; else SDL's own choice */
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
        s_ren = SDL_CreateRenderer(s_win, -1, SDL_RENDERER_ACCELERATED);
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "");
    }
#endif
    if (!s_ren) s_ren = SDL_CreateRenderer(s_win, -1, SDL_RENDERER_ACCELERATED);
    if (!s_ren) s_ren = SDL_CreateRenderer(s_win, -1, 0);
    if (!s_ren) { fprintf(stderr, "[native] SDL renderer: %s\n", SDL_GetError()); return 0; }
    /* pacing=display: the present waits for the display's refresh */
    s_vsync = g_opt.pacing && SDL_RenderSetVSync(s_ren, 1) == 0;
    s_vsync_off_until = 0;
    if (g_opt.pacing && !s_vsync) fprintf(stderr, "[native] pacing=display: no vsync (%s), the timer paces\n", SDL_GetError());
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    s_tex = SDL_CreateTexture(s_ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 512, 480);
    if (!s_tex) { fprintf(stderr, "[native] SDL texture: %s\n", SDL_GetError()); return 0; }
    SDL_RendererInfo info;
    if (SDL_GetRendererInfo(s_ren, &info) == 0) fprintf(stderr, "[native] renderer: %s\n", info.name);
    shader_open(s_ren);   /* shader_preset (shader.cpp); nothing when it is unset */
    if (!options_shader_preset().empty()) {
        s_osd_tex = SDL_CreateTexture(s_ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 512, 256);
        if (s_osd_tex) SDL_SetTextureBlendMode(s_osd_tex, SDL_BLENDMODE_BLEND);
    }
    s_rebuild = false;
    s_draw_fails = 0;
    return 1;
}

/* The controllers (controls.cpp): each player's, plugged in and out while playing. A change shows on
   the picture ("PLAYER 2: Xbox Controller") once the game is running; host_open's own scan doesn't. */
static bool s_pads_osd;

static void pad_event(const SDL_Event &e)
{
    std::string msg = controls_event(e);
    if (!msg.empty() && s_pads_osd) {
        for (char &c : msg) c = (char)toupper((unsigned char)c);
        host_osd(msg.c_str());
    }
}

static void pads_scan(void)   /* those present at SDL_Init */
{
    controls_devices_init();
}

int host_open(const char *title)
{
    SDL_SetMainReady();
    controls_sdl_hints();
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "[native] SDL: %s\n", SDL_GetError());
        return 0;
    }
    /* high_fps / widescreen auto: the display the window opens on (centred on the first); a failed
       query leaves both off */
    SDL_DisplayMode dm = {};
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0) dm = SDL_DisplayMode{};
    if (const char *t = getenv("XMEN2_DISPLAY_TEST"))   /* testing: W,H,HZ in place of the display's */
        if (sscanf(t, "%d,%d,%d", &dm.w, &dm.h, &dm.refresh_rate) != 3) dm = SDL_DisplayMode{};
    options_detect_display(dm.w, dm.h, dm.refresh_rate, true);
    s_win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (g_opt.widescreen ? 400 : 320) * 3,
                             224 * 3, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!s_win) { fprintf(stderr, "[native] SDL window: %s\n", SDL_GetError()); return 0; }
    if (getenv("XMEN2_FULLSCREEN")) SDL_SetWindowFullscreen(s_win, SDL_WINDOW_FULLSCREEN_DESKTOP);   /* as F11 (measuring) */
    int tw, th;   /* testing: XMEN2_WINDOW_SIZE=W,H the window's size in place of 3x (scaling's checks) */
    if (const char *t = getenv("XMEN2_WINDOW_SIZE"))
        if (sscanf(t, "%d,%d", &tw, &th) == 2 && tw > 0 && th > 0) SDL_SetWindowSize(s_win, tw, th);
    if (!make_renderer()) return 0;
    pads_scan();
    s_audio = audio_init((int)GENESIS_PSG_SAMPLE_RATE_NTSC) == 0;
    if (s_audio) audio_set_playback_enabled(1);
    else fprintf(stderr, "[native] no audio device: playing silent\n");
    s_period = (double)SDL_GetPerformanceFrequency() * 262.0 * 3420.0 / 53693175.0;   /* one NTSC frame */
    s_disp_period = dm.refresh_rate > 0 ? (double)SDL_GetPerformanceFrequency() / dm.refresh_rate : 0;
    s_next = SDL_GetPerformanceCounter();
    ptrace_open();
    return 1;
}

void host_input(uint16_t pads[4])
{
    s_pads_osd = true;
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
            audio_close();
            SDL_Quit();
            exit(0);
        }
        if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_F11 && !e.key.repeat) {
            Uint32 f = SDL_GetWindowFlags(s_win);
            bool full = !(f & SDL_WINDOW_FULLSCREEN_DESKTOP);
            SDL_SetWindowFullscreen(s_win, full ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
            host_osd(full ? "FULLSCREEN" : "WINDOWED");
        }
        /* phase 6 R1: the two presentation options */
        if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_F9 && !e.key.repeat) options_toggle_high_fps();
        if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_F10 && !e.key.repeat) options_toggle_widescreen();
        if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_F8 && !e.key.repeat) shader_toggle();
        if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_F4 && !e.key.repeat) options_toggle_scaling();
        /* quick saves (rewind.cpp): F5 save, F6 the next slot, F7 load; Rewind held (controls.cpp, by
           default Backspace and a controller's left trigger) rewinds (below) */
        if (e.type == SDL_KEYDOWN && !e.key.repeat &&
            (e.key.keysym.scancode == SDL_SCANCODE_F5 || e.key.keysym.scancode == SDL_SCANCODE_F6 ||
             e.key.keysym.scancode == SDL_SCANCODE_F7)) {
            if (!rewind_on()) host_osd(netplay_enabled() ? "NO SAVES OR REWIND ONLINE" : "REWIND AND SAVES ARE OFF");
            else if (e.key.keysym.scancode == SDL_SCANCODE_F5) rewind_key_save();
            else if (e.key.keysym.scancode == SDL_SCANCODE_F6) rewind_key_slot();
            else if (e.key.keysym.scancode == SDL_SCANCODE_F7) rewind_key_load();
        }
        if (e.type == SDL_RENDER_DEVICE_RESET) s_rebuild = true;
        /* high_fps=auto follows the window to another display's rate (the 16:9 view stays as it is) */
        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_DISPLAY_CHANGED && options_high_fps_auto()) {
            SDL_DisplayMode dm = {};
            if (SDL_GetDesktopDisplayMode(e.window.data1, &dm) != 0) dm = SDL_DisplayMode{};
            options_detect_display(dm.w, dm.h, dm.refresh_rate, false);
            present_reset();
        }
        pad_event(e);
    }
    for (int p = 0; p < 4; p++) pads[p] = controls_mask(p);
    if (rewind_on()) rewind_key_held(controls_rewind_held());
}

/* XMEN2_PAD_SELFTEST=1: the slots with SDL's virtual game controllers, headless (no window, no ROM).
   A controller attached after SDL_Init is found by host_open's scan and then announced by
   SDL_CONTROLLERDEVICEADDED, as one plugged in before launch; unplugged, plugged in again, a second
   one, the first replugged. Each step's slots are printed; exit 0 when every step holds. */
static int virtual_pad(bool generic = false)   /* its instance ID, or -1; generic: a joystick SDL has no mapping for */
{
    SDL_VirtualJoystickDesc d;
    SDL_zero(d);
    d.version = SDL_VIRTUAL_JOYSTICK_DESC_VERSION;
    d.type = generic ? SDL_JOYSTICK_TYPE_UNKNOWN : SDL_JOYSTICK_TYPE_GAMECONTROLLER;
    d.naxes = generic ? 2 : SDL_CONTROLLER_AXIS_MAX;
    d.nbuttons = generic ? 10 : SDL_CONTROLLER_BUTTON_MAX;
    d.nhats = generic ? 1 : 0;
    d.vendor_id = generic ? 0x1234 : 0;   /* its own GUID */
    d.product_id = generic ? 0x5678 : 0;
    d.name = generic ? "xmen2 selftest generic pad" : "xmen2 selftest pad";
    int index = SDL_JoystickAttachVirtualEx(&d);
    return index < 0 ? -1 : (int)SDL_JoystickGetDeviceInstanceID(index);
}

static SDL_Joystick *joy_of(int id)   /* an instance ID's open joystick */
{
    for (int p = 0; p < 4; p++)
        if (SDL_GameController *c = controls_player_pad(p))
            if (SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(c)) == id) return SDL_GameControllerGetJoystick(c);
    return SDL_JoystickFromInstanceID(id);
}

static void unplug(int id)
{
    for (int i = 0; i < SDL_NumJoysticks(); i++)
        if (SDL_JoystickGetDeviceInstanceID(i) == id) { SDL_JoystickDetachVirtual(i); return; }
}

static void pump(void)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) pad_event(e);
}

int host_pad_selftest(void)
{
    SDL_SetMainReady();
    controls_sdl_hints();
    if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "[native] pad selftest: SDL: %s\n", SDL_GetError());
        return 2;
    }
    controls_load();
    for (int p = 0; p < 4; p++) controls_defaults(p);   /* not this folder's xmen2.ini */
    int steps = 0, good = 0;
    auto pad_id = [](SDL_GameController *c) { return SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(c)); };
    auto expect = [&](const char *what, int p1, int p2) {   /* instance IDs in slots 1 and 2, -1 empty; 3-4 empty */
        int got[4];
        for (int n = 0; n < 4; n++) got[n] = controls_player_pad(n) ? (int)pad_id(controls_player_pad(n)) : -1;
        bool ok = got[0] == p1 && got[1] == p2 && got[2] == -1 && got[3] == -1;
        steps++;
        good += ok;
        fprintf(stderr, "[native] pad selftest: %s: slots %d %d %d %d (want %d %d -1 -1) %s\n", what, got[0], got[1], got[2],
                got[3], p1, p2, ok ? "ok" : "WRONG");
    };
    int a = virtual_pad();
    if (a < 0) {
        fprintf(stderr, "[native] pad selftest: no virtual controller: %s\n", SDL_GetError());
        return 2;
    }
    pads_scan();   /* host_open's, then the event SDL queued for it */
    pump();
    expect("one controller before launch", a, -1);
    auto expect_pads = [&](const char *what, uint16_t w0, uint16_t w1) {   /* players 1 and 2's bits; 3-4 none */
        uint16_t pads[4];
        host_input(pads);
        bool ok = pads[0] == w0 && pads[1] == w1 && !pads[2] && !pads[3];
        steps++;
        good += ok;
        fprintf(stderr, "[native] pad selftest: %s: pads %04x %04x %04x %04x (want %04x %04x 0 0) %s\n", what, pads[0], pads[1],
                pads[2], pads[3], w0, w1, ok ? "ok" : "WRONG");
    };
    if (SDL_Joystick *j = joy_of(a)) {   /* its button reaches player 1 only */
        SDL_JoystickSetVirtualButton(j, SDL_CONTROLLER_BUTTON_A, 1);
        expect_pads("its A button", GPAD_B, 0);
        /* remapped: the south button is C, and the left stick (pushed right, past the dead zone) is nothing */
        controls_bind_pad(0, CB_C, 0, PadIn{PadIn::BUTTON, SDL_CONTROLLER_BUTTON_A, 0});
        g_controls[0].pad[CB_RIGHT][1] = PadIn();
        SDL_JoystickSetVirtualAxis(j, SDL_CONTROLLER_AXIS_LEFTX, 30000);
        expect_pads("remapped: A = C, no stick", GPAD_C, 0);
        controls_defaults(0);
        expect_pads("the defaults again: A = B, stick right", GPAD_B | GPAD_RIGHT, 0);
        g_controls[0].deadzone = 95;
        expect_pads("a dead zone past the stick", GPAD_B, 0);
        controls_defaults(0);
        /* Rewind (rewind.cpp): the left trigger by default, held; no pad bit; then unbound, not held */
        SDL_JoystickSetVirtualAxis(j, SDL_CONTROLLER_AXIS_TRIGGERLEFT, 30000);
        expect_pads("the left trigger: no game button", GPAD_B | GPAD_RIGHT, 0);
        bool rw = controls_rewind_held() && controls_rewind_held_by(0) && !controls_rewind_held_by(1);
        g_controls[0].pad[CB_REWIND][0] = PadIn();
        rw = rw && !controls_rewind_held();
        steps++;
        good += rw;
        fprintf(stderr, "[native] pad selftest: the left trigger is player 1's Rewind, unbound it is nothing: %s\n", rw ? "ok" : "WRONG");
        controls_defaults(0);
        SDL_JoystickSetVirtualAxis(j, SDL_CONTROLLER_AXIS_TRIGGERLEFT, 0);
        SDL_JoystickSetVirtualButton(j, SDL_CONTROLLER_BUTTON_A, 0);
        SDL_JoystickSetVirtualAxis(j, SDL_CONTROLLER_AXIS_LEFTX, 0);
        /* player 1 with no controller: it goes to player 2 */
        g_controls[0].device = "none";
        controls_reassign();
        expect("player 1 set to none", -1, a);
        g_controls[0].device = "auto";
        controls_reassign();
    }
    unplug(a);
    pump();
    expect("unplugged", -1, -1);
    int a2 = virtual_pad();
    pump();
    expect("plugged in again", a2, -1);
    int b = virtual_pad();
    pump();
    expect("a second controller", a2, b);
    unplug(a2);
    pump();
    expect("the first unplugged", -1, b);
    int a3 = virtual_pad();
    pump();
    expect("the first plugged in again", a3, b);
    /* player 2 asks for this model by GUID (all the virtual ones share it): it takes the first plugged in */
    char guid[64];
    SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(joy_of(a3)), guid, sizeof guid);
    g_controls[1].device = guid;
    controls_reassign();
    expect("player 2 pinned to the model: the first plugged in (b)", a3, b);
    g_controls[1].device = "auto";
    controls_reassign();   /* a settings change: plug-in order */
    expect("player 2 auto again", b, a3);
    /* a joystick SDL has no mapping for: mapped here, player 3, its buttons in order and its hat */
    int g = virtual_pad(true);
    pump();
    SDL_GameController *gc = controls_player_pad(2);
    bool mapped = gc && (int)pad_id(gc) == g;
    steps++;
    good += mapped;
    fprintf(stderr, "[native] pad selftest: a generic joystick is player 3: %s\n", mapped ? "ok" : "WRONG");
    if (mapped) {
        SDL_Joystick *j = SDL_GameControllerGetJoystick(gc);
        SDL_JoystickSetVirtualButton(j, 0, 1);
        SDL_JoystickSetVirtualButton(j, 9, 1);
        SDL_JoystickSetVirtualHat(j, 0, SDL_HAT_UP);
        uint16_t pads[4];
        host_input(pads);
        bool ok = pads[2] == (GPAD_B | GPAD_START | GPAD_UP) && !pads[0] && !pads[1];
        steps++;
        good += ok;
        fprintf(stderr, "[native] pad selftest: its button 1, button 10 and hat up: player 3 %04x (want %04x) %s; labels %s, %s\n",
                pads[2], GPAD_B | GPAD_START | GPAD_UP, ok ? "ok" : "WRONG",
                controls_pad_label(g_controls[2].pad[CB_B][0], 2).c_str(), controls_pad_label(g_controls[2].pad[CB_START][0], 2).c_str());
    }
    int bad = controls_selftest();
    steps++;
    good += bad == 0;
    fprintf(stderr, "[native] pad selftest: %d of %d steps ok\n", good, steps);
    SDL_Quit();
    return good == steps ? 0 : 1;
}

void host_screenshot(const char *path, const uint32_t *fb, int stride, int w, int h)
{
    SDL_Surface *s = SDL_CreateRGBSurfaceWithFormatFrom((void *)fb, w, h, 32, stride * 4, SDL_PIXELFORMAT_ARGB8888);
    if (!s || SDL_SaveBMP(s, path) != 0) fprintf(stderr, "[native] screenshot %s: %s\n", path, SDL_GetError());
    if (s) SDL_FreeSurface(s);
}

/* XMEN2_WAV=path (dev, for recordings): the generated sound as a WAV (the engine's bit-exact tap,
   223,721 Hz stereo, from the first frame with sound), headless too: with no window, an audio device
   opened for it (SDL_AUDIODRIVER=dummy plays nothing) */
static void wav_stop(void) { audio_wav_stop(); }

static void wav_maybe_start(void)
{
    static bool tried;
    if (tried) return;
    tried = true;
    const char *path = getenv("XMEN2_WAV");
    if (!path || !*path) return;
    if (!s_audio) {
        SDL_SetMainReady();
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) == 0) s_audio = audio_init((int)GENESIS_PSG_SAMPLE_RATE_NTSC) == 0;
        if (s_audio) audio_set_playback_enabled(1);
    }
    if (s_audio && audio_wav_start(path) == 0) atexit(wav_stop);
    else fprintf(stderr, "[native] XMEN2_WAV: no audio device or can't write %s\n", path);
}

/* [rewind] sound = reversed: a rewind step's frame (frame C-1, played again after C) is heard
   backwards, so the steps C-1, C-2, ... are the sound before them backwards, end to end. A
   backwards frame is held back one frame: the last one, before the game plays on, the held picture's
   silence or a load, ends faded out, and the first sound after it starts faded in (no click where the
   wave turns round). The fades are an eighth of a frame (~2 ms). */
namespace {
struct Rev {
    std::vector<int16_t> fm, psg;   /* fm stereo pairs */
    uint32_t frame = 0;
    bool held = false;      /* a backwards frame waits */
    bool fade_in = false;   /* the next sound starts faded in */
} s_rev;

void ramp(int16_t *b, size_t n, int ch, bool in)
{
    size_t k = n / 8;
    for (size_t i = 0; i < k; i++) {
        float g = ((float)i + 0.5f) / (float)k;
        size_t at = in ? i : n - 1 - i;
        for (int c = 0; c < ch; c++) b[at * ch + c] = (int16_t)((float)b[at * ch + c] * g);
    }
}

void rev_send(bool fade_out)
{
    if (!s_rev.held) return;
    s_rev.held = false;
    size_t fm_n = s_rev.fm.size() / 2, psg_n = s_rev.psg.size();
    if (fade_out) {
        ramp(s_rev.fm.data(), fm_n, 2, false);
        ramp(s_rev.psg.data(), psg_n, 1, false);
        s_rev.fade_in = true;
    }
    if (s_audio) audio_flush(s_rev.frame, 1, s_rev.fm.data(), fm_n, s_rev.psg.data(), psg_n);
}
}  // namespace

void host_audio(uint64_t frame, bool mute, int how)
{
    wav_maybe_start();
    /* the chips' writes of the Z80's frame (stamped in master clocks from its start), mixed, to
       the device (as the engine's sim step: GENESIS_SIM_WALL_FRAME_MASTER_CYCLES); muted (a
       rollback's replay of frames already heard), the chips still run; a rewind's frames shown
       again (how): the chips run and the device gets the frame's length of silence, or its sound
       backwards */
    static int16_t fm[8192 * 2], psg[16384];
    size_t fm_n = 0, psg_n = 0;
    bool silent = how == REWIND_AUDIO_SILENT, reversed = how == REWIND_AUDIO_REVERSED && !mute;
    audio_mixer_drain(895780u, fm, 8192, &fm_n, psg, sizeof psg / sizeof psg[0], &psg_n);
    /* phase 7 R9: players 3-4's parts; music_voices: the music (XMEN2_STEM=sfx: silent) */
    sound_mix(fm, fm_n, psg, psg_n, mute || silent || capture_mute_snd2());
    if (silent) {
        memset(fm, 0, fm_n * 2 * sizeof fm[0]);
        memset(psg, 0, psg_n * sizeof psg[0]);
    }
    rev_send(!reversed);   /* a backwards frame before this one: as it is, or faded out when this one isn't */
    if (reversed) {
        /* the FM taken to the PSG rate first, as audio_flush's mix does (nearest: fi = i * fm_n / psg_n), so
           the two reversed at one rate mix to the forward sound exactly backwards (FM reversed at its own
           rate would pick the neighbouring sample for about one in four) */
        s_rev.fm.resize(psg_n * 2);
        for (size_t i = 0; i < psg_n; i++) {
            size_t fi = fm_n ? i * fm_n / psg_n : 0, at = psg_n - 1 - i;
            s_rev.fm[2 * at] = fm_n ? fm[2 * fi] : 0;
            s_rev.fm[2 * at + 1] = fm_n ? fm[2 * fi + 1] : 0;
        }
        s_rev.psg.assign(psg, psg + psg_n);
        std::reverse(s_rev.psg.begin(), s_rev.psg.end());
        if (s_rev.fade_in) {
            ramp(s_rev.fm.data(), psg_n, 2, true);
            ramp(s_rev.psg.data(), psg_n, 1, true);
        }
        s_rev.fade_in = false;
        s_rev.frame = (uint32_t)frame;
        s_rev.held = true;
        return;
    }
    if (s_rev.fade_in && !mute && !silent) {
        ramp(fm, fm_n, 2, true);
        ramp(psg, psg_n, 1, true);
    }
    if (!mute) s_rev.fade_in = silent;   /* forward after a silent stretch fades in too */
    if (s_audio && !mute) audio_flush((uint32_t)frame, 1, fm, fm_n, psg, psg_n);
}

/* rewind.cpp: a frame's length of silence (the picture held at the rewind's oldest frame) */
void host_audio_silence(void)
{
    static int16_t psg[3734];   /* one NTSC frame at the PSG rate (223,721 Hz / 59.92) */
    rev_send(true);
    s_rev.fade_in = true;
    if (s_audio) audio_flush(0, 1, nullptr, 0, psg, sizeof psg / sizeof psg[0]);
}

/* ... the sound queued for the device, dropped (a load: it was the other timeline's; a rewind's
   start) */
void host_audio_discard(void)
{
    s_rev.held = false;
    s_rev.fade_in = true;
    if (s_audio) audio_discard_playback();
}

/* The hotkeys' messages (F9, F10, F11): 8x8 text, white on a translucent black box, centred at the
   top of the game's picture for 2 s. Drawn on a copy at present time, so the game's frame (its
   screenshots, captures and --fb hashes) never has it. */
static char     s_osd[48];
static uint64_t s_osd_until;   /* SDL ticks (ms) */

void host_osd(const char *text)
{
    snprintf(s_osd, sizeof s_osd, "%s", text);
    s_osd_until = SDL_GetTicks64() + 2000;
}

static bool osd_live(int w, int h) { return s_osd[0] && SDL_GetTicks64() < s_osd_until && w <= 512 && h <= 256; }

/* the box and the text into buf (512 wide): over the picture copied there, or (layer, the shaded picture's:
   drawn over it after the chain) on a clear layer of their own, the box as black at 2/3 alpha */
static void osd_paint(uint32_t *buf, int w, int h, bool layer)
{
    int n = (int)strlen(s_osd), tw = n * 8, x0 = (w - tw) / 2, y0 = 8;
    for (int y = y0 - 3; y < y0 + 11; y++)
        for (int x = x0 - 4; x < x0 + tw + 3; x++) {
            if (x < 0 || x >= w || y < 0 || y >= h) continue;
            uint32_t p = buf[y * 512 + x];   /* 2/3 black over the picture */
            buf[y * 512 + x] = layer ? 0xAA000000u
                                     : (p & 0xFF000000u) | ((p >> 16 & 0xFF) / 3) << 16 | ((p >> 8 & 0xFF) / 3) << 8 | (p & 0xFF) / 3;
        }
    for (int i = 0; i < n; i++) {
        unsigned char c = (unsigned char)s_osd[i];
        if (c < 0x20 || c > 0x7E) continue;
        for (int y = 0; y < 8; y++)
            for (int x = 0; x < 8; x++)
                if (k_osd_font[c - 0x20][y] >> x & 1) {
                    int px = x0 + i * 8 + x, py = y0 + y;
                    if (px >= 0 && px < w && py < h) buf[py * 512 + px] = 0xFFFFFFFFu;
                }
    }
}

static const uint32_t *osd_overlay(const uint32_t *fb, int *stride, int w, int h)
{
    static uint32_t buf[256 * 512];
    if (!osd_live(w, h)) return fb;
    for (int y = 0; y < h; y++) memcpy(buf + y * 512, fb + y * *stride, (size_t)w * 4);
    osd_paint(buf, w, h, false);
    *stride = 512;
    return buf;
}

/* ... the layer over the shaded picture (shader.cpp): s_osd_tex at dst; false if there is no message */
static bool osd_layer(int w, int h)
{
    static uint32_t buf[256 * 512];
    if (!s_osd_tex || !osd_live(w, h)) return false;
    memset(buf, 0, sizeof buf);
    osd_paint(buf, w, h, true);
    SDL_Rect src = { 0, 0, w, h };
    return SDL_UpdateTexture(s_osd_tex, &src, buf, 512 * 4) == 0;
}

/* A played netplay session waiting for the other seats (netplay.cpp seal_exchange): the window stays
   responsive (Esc or closing it quits, F11 fullscreen, pads plugged in are kept) and shows LINES,
   centred, in the OSD font on black. Testing: XMEN2_WAIT_QUIT_MS=N acts as a close after N ms,
   XMEN2_WAIT_SHOT=FILE saves the screen once (BMP). */
static void draw(const uint32_t *fb, int stride, int w, int h);
void host_wait_screen(const char *const *lines, int n)
{
    static uint64_t t0 = SDL_GetTicks64();
    static const char *quit_ms = getenv("XMEN2_WAIT_QUIT_MS");
    bool quit = quit_ms && *quit_ms && SDL_GetTicks64() - t0 >= (uint64_t)atoll(quit_ms);
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) quit = true;
        if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_F11 && !e.key.repeat) {
            Uint32 f = SDL_GetWindowFlags(s_win);
            SDL_SetWindowFullscreen(s_win, (f & SDL_WINDOW_FULLSCREEN_DESKTOP) ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
        }
        if (e.type == SDL_RENDER_DEVICE_RESET) s_rebuild = true;
        pad_event(e);
    }
    if (quit) {
        fprintf(stderr, "[native] closed while waiting for the other players\n");
        audio_close();
        SDL_Quit();
        exit(0);
    }
    enum { W = 320, H = 224 };
    static uint32_t fb[W * H];
    for (uint32_t &p : fb) p = 0xFF000000u;   /* opaque black, as the game's own frames */
    int y0 = (H - n * 14) / 2;
    for (int l = 0; l < n; l++) {
        int len = (int)strlen(lines[l]);
        if (len > W / 8) len = W / 8;
        int x0 = (W - len * 8) / 2, y = y0 + l * 14;
        uint32_t ink = l == 0 ? 0xFFCECE00u : 0xFFDCDCF0u;   /* the first line in the Cerebro screens' yellow */
        for (int i = 0; i < len; i++) {
            unsigned char c = (unsigned char)lines[l][i];
            if (c < 0x20 || c > 0x7E) continue;
            for (int ry = 0; ry < 8; ry++)
                for (int rx = 0; rx < 8; rx++)
                    if (k_osd_font[c - 0x20][ry] >> rx & 1) fb[(y + ry) * W + x0 + i * 8 + rx] = ink;
        }
    }
    static const char *shot = getenv("XMEN2_WAIT_SHOT");
    if (shot && *shot) { host_screenshot(shot, fb, W, W, H); shot = nullptr; }
    draw(fb, W, W, H);
}

/* XMEN2_PRESENT_TRACE=FILE (played runs; the frame-pacing work, NOTES "Present trace"): a row per
   picture shown: its content time (us on the present clock), when it was due and the performance
   counter before and after SDL_RenderPresent; on Windows also DWM's last V-blank and refresh period
   (DwmGetCompositionTimingInfo). tools/presenttrace.py works out from them what each refresh of the
   display showed. */
static FILE *s_ptrace;
static double s_content_us;      /* the picture being drawn: its time on the present clock */
static uint64_t s_due;           /* when it was due (performance counter) */

static void ptrace_open(void)
{
    const char *path = getenv("XMEN2_PRESENT_TRACE");
    if (!path || !*path) return;
    s_ptrace = fopen(path, "w");
    if (!s_ptrace) { fprintf(stderr, "[native] cannot write XMEN2_PRESENT_TRACE %s\n", path); return; }
    setvbuf(s_ptrace, nullptr, _IOFBF, 1 << 20);
    SDL_DisplayMode dm = {};
    SDL_GetDesktopDisplayMode(SDL_GetWindowDisplayIndex(s_win), &dm);
    SDL_RendererInfo info = {};
    if (s_ren) SDL_GetRendererInfo(s_ren, &info);
    fprintf(s_ptrace, "# freq %llu display_hz %d renderer %s high_fps %d\n",
            (unsigned long long)SDL_GetPerformanceFrequency(), dm.refresh_rate, info.name ? info.name : "?", g_opt.high_fps);
    fprintf(s_ptrace, "content_us,due,pre,post,vblank,period,fullscreen,high_fps\n");
    atexit([] { if (s_ptrace) fclose(s_ptrace); });
}

static void ptrace_row(uint64_t pre, uint64_t post)
{
    uint64_t vblank = 0, period = 0;
#ifdef _WIN32
    DWM_TIMING_INFO ti = {};
    ti.cbSize = sizeof ti;
    if (SUCCEEDED(DwmGetCompositionTimingInfo(nullptr, &ti))) { vblank = ti.qpcVBlank; period = ti.qpcRefreshPeriod; }
#endif
    int fs = (SDL_GetWindowFlags(s_win) & SDL_WINDOW_FULLSCREEN_DESKTOP) ? 1 : 0;
    fprintf(s_ptrace, "%.1f,%llu,%llu,%llu,%llu,%llu,%d,%d\n", s_content_us, (unsigned long long)s_due,
            (unsigned long long)pre, (unsigned long long)post, (unsigned long long)vblank, (unsigned long long)period,
            fs, g_opt.high_fps);
}

static void draw(const uint32_t *fb, int stride, int w, int h)
{
    if ((s_rebuild || s_draw_fails >= 30) && !make_renderer()) return;
    if (!s_ren || !s_tex) return;
    /* testing: XMEN2_OSD_TEST=N:TEXT shows TEXT at the Nth picture drawn, which goes to
       $XMEN2_DUMP_DIR/osd.bmp with it */
    static const char *osd_test = getenv("XMEN2_OSD_TEST");
    static long draws;
    bool osd_dump = false;
    if (osd_test && ++draws == atol(osd_test) && strchr(osd_test, ':')) {
        host_osd(strchr(osd_test, ':') + 1);
        osd_dump = getenv("XMEN2_DUMP_DIR") != nullptr;
    }
    /* testing: XMEN2_KEY_TEST=N:KEY,... presses KEY (an SDL scancode name: F4) as the Nth picture is drawn,
       through the event queue the keyboard's presses take */
    static const char *key_test = getenv("XMEN2_KEY_TEST");
    static long key_draws;
    if (key_test) {
        ++key_draws;
        for (const char *q = key_test; *q; ) {
            char *e, name[32] = "";
            long n = strtol(q, &e, 10);
            if (e == q || *e != ':') break;
            size_t len = strcspn(e + 1, ",");
            snprintf(name, sizeof name, "%.*s", (int)(len < 31 ? len : 31), e + 1);
            if (n == key_draws) {
                SDL_Event ev = {};
                ev.type = SDL_KEYDOWN;
                ev.key.keysym.scancode = SDL_GetScancodeFromName(name);
                SDL_PushEvent(&ev);
            }
            q = e + 1 + len;
            if (*q == ',') q++;
        }
    }
    int ww, wh;
    if (SDL_GetRendererOutputSize(s_ren, &ww, &wh) != 0 || ww <= 0 || wh <= 0) { s_draw_fails++; return; }
    /* scaling: the largest size that fits; integer keeps a whole multiple of it (when one fits) */
    double sx = (double)ww / w, sy = (double)wh / (h * 1.0);
    double s = sx < sy ? sx : sy;
    int mode = options_scaling(), k = (int)s;
    if (mode == SCALE_INTEGER && k >= 1) s = k;
    SDL_Rect dst = { (int)((ww - w * s) / 2), (int)((wh - h * s) / 2), (int)(w * s), (int)(h * s) };
    SDL_Rect src = { 0, 0, w, h };
    /* sharp_bilinear between whole multiples: sharp to k times, then smoothed to the size (each game pixel
       k screen pixels plus at most one blended one where it meets the next); at a whole multiple, or with
       no render targets, it is drawn as nearest */
    bool prescale = mode == SCALE_SHARP_BILINEAR && k >= 1 && s - k > 1e-6 && SDL_RenderTargetSupported(s_ren);
    if (prescale && (s_pre_w != w * k || s_pre_h != h * k)) {
        if (s_pre) SDL_DestroyTexture(s_pre);
        s_pre = SDL_CreateTexture(s_ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, w * k, h * k);
        s_pre_w = s_pre ? w * k : 0;
        s_pre_h = s_pre ? h * k : 0;
        if (s_pre) SDL_SetTextureScaleMode(s_pre, SDL_ScaleModeLinear);
        else fprintf(stderr, "[native] scaling=sharp_bilinear: no %dx%d target (%s), drawn sharp\n", w * k, h * k, SDL_GetError());
    }
    prescale = prescale && s_pre;
    SDL_SetRenderDrawColor(s_ren, 0, 0, 0, 255);
    SDL_RenderClear(s_ren);
    int bad = 0;
    if (shader_draw(s_ren, fb, stride, w, h, &dst)) {   /* shader_preset: the message over the shaded picture */
        if (osd_layer(w, h)) bad |= SDL_RenderCopy(s_ren, s_osd_tex, &src, &dst) != 0;
    } else {
        fb = osd_overlay(fb, &stride, w, h);
        if (osd_dump) host_screenshot((std::string(getenv("XMEN2_DUMP_DIR")) + "/osd.bmp").c_str(), fb, stride, w, h);
        bad |= SDL_UpdateTexture(s_tex, &src, fb, stride * 4) != 0;
        if (prescale) {
            SDL_Rect whole = { 0, 0, w * k, h * k };
            bad |= SDL_SetRenderTarget(s_ren, s_pre) != 0;
            bad |= SDL_RenderCopy(s_ren, s_tex, &src, &whole) != 0;
            bad |= SDL_SetRenderTarget(s_ren, nullptr) != 0;
            bad |= SDL_RenderCopy(s_ren, s_pre, &whole, &dst) != 0;
        } else {
            bad |= SDL_RenderCopy(s_ren, s_tex, &src, &dst) != 0;
        }
    }
    /* testing: XMEN2_BACKBUFFER_SHOT=N,M,... the window as presented at the Nth picture drawn (after the
       shader chain and the message), as $XMEN2_DUMP_DIR/backbuffer_N.bmp */
    static const char *bb_list = getenv("XMEN2_BACKBUFFER_SHOT"), *bb_dir = getenv("XMEN2_DUMP_DIR");
    static long bb_draws;
    if (bb_list && bb_dir) {
        ++bb_draws;
        for (const char *q = bb_list; *q; ) {
            char *e;
            long n = strtol(q, &e, 10);
            if (e == q) break;
            if (n == bb_draws) {
                SDL_Surface *sf = SDL_CreateRGBSurfaceWithFormat(0, ww, wh, 32, SDL_PIXELFORMAT_ARGB8888);
                char path[600];
                snprintf(path, sizeof path, "%s/backbuffer_%ld.bmp", bb_dir, n);
                if (!sf || SDL_RenderReadPixels(s_ren, nullptr, SDL_PIXELFORMAT_ARGB8888, sf->pixels, sf->pitch) != 0 ||
                    SDL_SaveBMP(sf, path) != 0)
                    fprintf(stderr, "[native] XMEN2_BACKBUFFER_SHOT %s: %s\n", path, SDL_GetError());
                if (sf) SDL_FreeSurface(sf);
            }
            q = *e == ',' ? e + 1 : e;
        }
    }
    uint64_t pre = s_ptrace ? SDL_GetPerformanceCounter() : 0;
    SDL_RenderPresent(s_ren);
    if (s_ptrace) ptrace_row(pre, SDL_GetPerformanceCounter());
    s_draw_fails = bad ? s_draw_fails + 1 : 0;
}

/* high_fps (phase 6 R4): the picture waits for its time on the present clock (us since the first
   frame); a frame more than 4 NTSC frames late moves the clock instead of hurrying. */
void host_present_at(const uint32_t *fb, int stride, int w, int h, double t_us)
{
    double f = (double)SDL_GetPerformanceFrequency();
    double now = (double)SDL_GetPerformanceCounter();
    if (!s_at_started) { s_at_base = now - t_us * f / 1e6; s_at_started = true; }
    double due = s_at_base + t_us * f / 1e6;
    if (now > due + 4 * s_period) s_at_base += now - due;
    while ((now = (double)SDL_GetPerformanceCounter()) < due) {
        double left_ms = (due - now) * 1000.0 / f;
        if (left_ms > 2.0) SDL_Delay((Uint32)(left_ms - 1.5));
    }
    s_content_us = t_us;
    s_due = (uint64_t)due;
    draw(fb, stride, w, h);
}

/* pacing=display (the vsync work, NOTES "Present trace"): the refresh grid as a V-blank and a period
   in counter ticks: DWM's on Windows (the trace's), else the last vsync'd present's return and the
   display's rate. false: none known, the timer paces. */
static bool grid(double *vblank, double *period)
{
#ifdef _WIN32
    DWM_TIMING_INFO ti = {};
    ti.cbSize = sizeof ti;
    if (SUCCEEDED(DwmGetCompositionTimingInfo(nullptr, &ti)) && ti.qpcRefreshPeriod && ti.qpcVBlank) {
        *vblank = (double)ti.qpcVBlank;
        *period = (double)ti.qpcRefreshPeriod;
        return true;
    }
#endif
    if (!s_disp_period || !s_last_flip) return false;
    *vblank = (double)s_last_flip;
    *period = s_disp_period;
    return true;
}

/* A vsync'd present that blocks longer than two NTSC frames means the display isn't taking pictures
   at its rate: a locked session gave 260 ms a present (2026-10-31), so the game, paced by its
   presents, ran at 4 frames a second. Then vsync is off and the timer paces for 5 s, and is tried
   again. */
bool host_vsync(void)
{
    double v, p;
    if (!s_vsync || !s_ren) return false;
    if (s_vsync_off_until) {
        if (SDL_GetPerformanceCounter() < s_vsync_off_until) return false;
        s_vsync_off_until = 0;
        SDL_RenderSetVSync(s_ren, 1);
        s_last_refresh = 0;
    }
    return grid(&v, &p);
}

static double clock_start(double t_us)   /* the present clock's start (host_present_at's), set at its first use */
{
    double f = (double)SDL_GetPerformanceFrequency();
    if (!s_at_started) { s_at_base = (double)SDL_GetPerformanceCounter() - t_us * f / 1e6; s_at_started = true; }
    return f;
}

double host_refresh_us(void)
{
    double v, p;
    return grid(&v, &p) ? p * 1e6 / (double)SDL_GetPerformanceFrequency() : 0;
}

int host_refreshes(double t0_us, double t1_us, double *t_us, int max)
{
    double v, p, f = clock_start(t0_us);
    if (!grid(&v, &p)) return 0;
    double w0 = s_at_base + t0_us * f / 1e6, w1 = s_at_base + t1_us * f / 1e6;
    int n = 0;
    for (double w = v + std::ceil((w0 - v) / p) * p; w < w1 && n < max; w += p) t_us[n++] = (w - s_at_base) * 1e6 / f;
    return n;
}

/* The picture at the refresh nearest t_us, a refresh after the last picture's at the earliest: drawn
   half a refresh before it, so the vsync'd present gives it that refresh (a present returns when the
   picture is queued; later than that refresh, the next one). A refresh more than 4 NTSC frames late
   moves the clock, as host_present_at. */
void host_present_refresh(const uint32_t *fb, int stride, int w, int h, double t_us)
{
    double v, p, f = clock_start(t_us);
    if (!grid(&v, &p)) { host_present_at(fb, stride, w, h, t_us); return; }
    double due = s_at_base + t_us * f / 1e6, now = (double)SDL_GetPerformanceCounter();
    if (now > due + 4 * s_period) { s_at_base += now - due; due = now; }
    double r = v + std::floor((due - v) / p + 0.5) * p;
    if (r < s_last_refresh + p * 0.5) r = s_last_refresh + p;
    s_last_refresh = r;
    while ((now = (double)SDL_GetPerformanceCounter()) < r - p * 0.5) {
        double left_ms = (r - p * 0.5 - now) * 1000.0 / f;
        if (left_ms > 2.0) SDL_Delay((Uint32)(left_ms - 1.5));
    }
    s_content_us = t_us;
    s_due = (uint64_t)r;
    uint64_t pre = SDL_GetPerformanceCounter();
    draw(fb, stride, w, h);
    s_last_flip = SDL_GetPerformanceCounter();
    if ((double)(s_last_flip - pre) > 2 * s_period) {   /* held back (host_vsync) */
        if (!s_vsync_holds++)
            fprintf(stderr, "[native] pacing=display: the display held a present %.0f ms (a locked, hidden or minimized "
                    "window?): the timer paces, vsync tried again every 5 s\n", (double)(s_last_flip - pre) * 1e3 / f);
        SDL_RenderSetVSync(s_ren, 0);
        s_vsync_off_until = s_last_flip + (uint64_t)(5 * f);
        s_next = s_last_flip + (uint64_t)s_period;   /* the timer's next frame (host_present) */
    }
}

void host_present(const uint32_t *fb, int stride, int w, int h)
{
    static uint64_t shown;   /* pictures this way so far: the content time is one NTSC frame each */
    s_content_us = (double)shown++ * 1e6 * 262.0 * 3420.0 / 53693175.0;
    s_due = s_next;
    draw(fb, stride, w, h);

    /* pace: one NTSC frame per frame; a late frame does not try to catch up */
    s_next += (uint64_t)s_period;
    uint64_t now = SDL_GetPerformanceCounter();
    if (now > s_next + (uint64_t)(4 * s_period)) s_next = now;
    while ((now = SDL_GetPerformanceCounter()) < s_next) {
        double left_ms = (double)(s_next - now) * 1000.0 / (double)SDL_GetPerformanceFrequency();
        if (left_ms > 2.0) SDL_Delay((Uint32)(left_ms - 1.5));
    }
}

/* netplay (phase 7): the scheduler held a frame (its timesync pacing, its wait for a remote input);
   the clocks move by the hold, so it slows this seat instead of being made up on the next frames */
void host_slip(double us)
{
    double ticks = us * (double)SDL_GetPerformanceFrequency() / 1e6;
    s_next += (uint64_t)ticks;
    s_at_base += ticks;
}

/* rewind.cpp (present_jump): the frame-numbered present clock only (host_present's timer counts
   pictures, not frames) */
void host_present_shift(double us)
{
    if (s_at_started) s_at_base += us * (double)SDL_GetPerformanceFrequency() / 1e6;
}
