/* host_sdl.cpp - the native runner's window, input and sound (phase 5 P8): the game played on the
 * PC. SDL2 for the window, the keyboard and game controllers and the audio device; the sound chips
 * and their mixer are the engine's (audio/: ymfm's YM2612, the SN76489, the cycle-stamped event
 * queue the Z80's writes go into, audio.c's device bridge).
 *
 *   Keyboard (player 1): arrows; Z = A, X = B, C = C, Enter = Start; A = X, S = Y, D = Z, Q = Mode.
 *   Game controllers (players 1-4 in plug-in order, the keyboard with the first; an unplugged one
 *   frees its slot): D-pad or left stick; X = A, A = B, B = C, Start;
 *   LB = X, Y = Y, RB = Z, Back = Mode.  Esc or closing the window quits.
 * XMEN2_PAD_SELFTEST=1: the slots checked with virtual controllers, headless (host_pad_selftest).
 * Frames are paced to the NTSC rate (59.92 Hz) by the clock (high_fps: present.cpp's clock); F11
 * toggles fullscreen, F9 high_fps, F10 widescreen (from the next screen).
 * The renderer is Direct3D 11 on Windows where it can be (SDL's default, Direct3D 9, loses its
 * device when the window moves to another monitor or the displays change, and the picture froze
 * while the game and its music went on); a device reset or 30 failed draws in a row rebuild the
 * renderer and its texture.
 */
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include "audio.h"
#include "audio/event_queue.h"
#include "audio/mixer.h"
#include "genesis_clocks.h"
#include "video/genesis_bus.h"
}
#include "host_sdl.h"
#include "native.h"
#include "osd_font.h"
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
void host_message(const char *text)
{
    if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "X-Men 2: Clone Wars", text, nullptr) != 0)
        fprintf(stderr, "[native] %s\n", text);
}

bool host_pick_rom(char *path, size_t cap)
{
    if (const char *t = getenv("XMEN2_PICK_ROM_TEST")) {   /* testing: the picker's answer ("" = cancelled) */
        snprintf(path, cap, "%s", t);
        return *t != 0;
    }
#ifdef _WIN32
    OPENFILENAMEA o = {};
    path[0] = 0;
    o.lStructSize = sizeof o;
    o.lpstrFilter = "Genesis ROM (*.bin, *.md, *.gen)\0*.bin;*.md;*.gen\0All files\0*.*\0";
    o.lpstrFile = path;
    o.nMaxFile = (DWORD)cap;
    o.lpstrTitle = "X-Men 2: Clone Wars - choose your ROM (USA/Europe)";
    o.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
    return GetOpenFileNameA(&o) != 0;
#else
    /* zenity (GNOME and most desktops) or kdialog (KDE); neither: false */
    static const char *const cmds[] = {
        "zenity --file-selection --title='X-Men 2: Clone Wars - choose your ROM (USA/Europe)' 2>/dev/null",
        "kdialog --getopenfilename . 2>/dev/null" };
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

static SDL_Window *s_win;
static SDL_Renderer *s_ren;
static SDL_Texture *s_tex;
static SDL_GameController *s_pad[4];   /* players 1-4 (3-4: phase 7, `players` 3 or 4) */
static uint64_t s_next;          /* performance-counter time of the next frame */
static double s_period;          /* counter ticks per frame */
static bool s_at_started;        /* high_fps: the present clock has started */
static double s_at_base;         /* high_fps: performance-counter ticks at t_us 0 */
static int s_audio;
static bool s_rebuild;           /* the renderer's device was reset: make it again before drawing */
static int s_draw_fails;         /* failed draws in a row */
static void ptrace_open(void);   /* XMEN2_PRESENT_TRACE (below, with draw) */

/* the renderer and the frame's texture, (re)made; 0 when there is none */
static int make_renderer(void)
{
    if (s_tex) { SDL_DestroyTexture(s_tex); s_tex = nullptr; }
    if (s_ren) { SDL_DestroyRenderer(s_ren); s_ren = nullptr; }
#ifdef _WIN32
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d11");
    s_ren = SDL_CreateRenderer(s_win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "");
#endif
    if (!s_ren) s_ren = SDL_CreateRenderer(s_win, -1, SDL_RENDERER_ACCELERATED);
    if (!s_ren) s_ren = SDL_CreateRenderer(s_win, -1, 0);
    if (!s_ren) { fprintf(stderr, "[native] SDL renderer: %s\n", SDL_GetError()); return 0; }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    s_tex = SDL_CreateTexture(s_ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 512, 480);
    if (!s_tex) { fprintf(stderr, "[native] SDL texture: %s\n", SDL_GetError()); return 0; }
    SDL_RendererInfo info;
    if (SDL_GetRendererInfo(s_ren, &info) == 0) fprintf(stderr, "[native] renderer: %s\n", info.name);
    s_rebuild = false;
    s_draw_fails = 0;
    return 1;
}

/* The controllers' slots (players 1-4 in plug-in order). SDL2 also sends SDL_CONTROLLERDEVICEADDED
   for the controllers present at SDL_Init, which host_open has opened already: opening one again
   gives the same handle, so a slot is taken only by a device in none (v1.0.0 on Linux: one pad was
   players 1 and 2). An unplugged controller frees its slot; plugged in again it takes the first free
   one, its own when the others are still there. */
static SDL_JoystickID pad_id(SDL_GameController *c) { return SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(c)); }

static void pad_add(int index)   /* a device index */
{
    if (!SDL_IsGameController(index)) return;
    SDL_JoystickID id = SDL_JoystickGetDeviceInstanceID(index);
    for (int n = 0; n < 4; n++)
        if (s_pad[n] && pad_id(s_pad[n]) == id) return;
    for (int n = 0; n < 4; n++)
        if (!s_pad[n]) {
            if ((s_pad[n] = SDL_GameControllerOpen(index)))
                fprintf(stderr, "[native] controller %d: %s (instance %d)\n", n + 1, SDL_GameControllerName(s_pad[n]), (int)id);
            return;
        }
}

static void pad_remove(SDL_JoystickID id)   /* an instance ID */
{
    for (int n = 0; n < 4; n++)
        if (s_pad[n] && pad_id(s_pad[n]) == id) {
            SDL_GameControllerClose(s_pad[n]);
            s_pad[n] = nullptr;
            fprintf(stderr, "[native] controller %d removed (instance %d)\n", n + 1, (int)id);
        }
}

static void pad_event(const SDL_Event &e)
{
    if (e.type == SDL_CONTROLLERDEVICEADDED) pad_add(e.cdevice.which);
    else if (e.type == SDL_CONTROLLERDEVICEREMOVED) pad_remove(e.cdevice.which);
}

static void pads_scan(void)   /* those present at SDL_Init */
{
    for (int i = 0; i < SDL_NumJoysticks(); i++) pad_add(i);
}

int host_open(const char *title)
{
    SDL_SetMainReady();
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
    if (!make_renderer()) return 0;
    pads_scan();
    s_audio = audio_init((int)GENESIS_PSG_SAMPLE_RATE_NTSC) == 0;
    if (s_audio) audio_set_playback_enabled(1);
    else fprintf(stderr, "[native] no audio device: playing silent\n");
    s_period = (double)SDL_GetPerformanceFrequency() * 262.0 * 3420.0 / 53693175.0;   /* one NTSC frame */
    s_next = SDL_GetPerformanceCounter();
    ptrace_open();
    return 1;
}

static uint16_t keyboard_pad(void)
{
    const Uint8 *k = SDL_GetKeyboardState(nullptr);
    uint16_t p = 0;
    if (k[SDL_SCANCODE_UP]) p |= GPAD_UP;
    if (k[SDL_SCANCODE_DOWN]) p |= GPAD_DOWN;
    if (k[SDL_SCANCODE_LEFT]) p |= GPAD_LEFT;
    if (k[SDL_SCANCODE_RIGHT]) p |= GPAD_RIGHT;
    if (k[SDL_SCANCODE_Z]) p |= GPAD_A;
    if (k[SDL_SCANCODE_X]) p |= GPAD_B;
    if (k[SDL_SCANCODE_C]) p |= GPAD_C;
    if (k[SDL_SCANCODE_RETURN]) p |= GPAD_START;
    if (k[SDL_SCANCODE_A]) p |= GPAD_X;
    if (k[SDL_SCANCODE_S]) p |= GPAD_Y;
    if (k[SDL_SCANCODE_D]) p |= GPAD_Z;
    if (k[SDL_SCANCODE_Q]) p |= GPAD_MODE;
    return p;
}

static uint16_t controller_pad(SDL_GameController *c)
{
    if (!c) return 0;
    uint16_t p = 0;
    const int dead = 12000;
    int x = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTX), y = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTY);
    if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_UP) || y < -dead) p |= GPAD_UP;
    if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_DOWN) || y > dead) p |= GPAD_DOWN;
    if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_LEFT) || x < -dead) p |= GPAD_LEFT;
    if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) || x > dead) p |= GPAD_RIGHT;
    if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_X)) p |= GPAD_A;
    if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_A)) p |= GPAD_B;
    if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_B)) p |= GPAD_C;
    if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_START)) p |= GPAD_START;
    if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_LEFTSHOULDER)) p |= GPAD_X;
    if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_Y)) p |= GPAD_Y;
    if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) p |= GPAD_Z;
    if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_BACK)) p |= GPAD_MODE;
    return p;
}

void host_input(uint16_t pads[4])
{
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
    pads[0] = (uint16_t)(keyboard_pad() | controller_pad(s_pad[0]));
    pads[1] = controller_pad(s_pad[1]);
    pads[2] = controller_pad(s_pad[2]);
    pads[3] = controller_pad(s_pad[3]);
}

/* XMEN2_PAD_SELFTEST=1: the slots with SDL's virtual game controllers, headless (no window, no ROM).
   A controller attached after SDL_Init is found by host_open's scan and then announced by
   SDL_CONTROLLERDEVICEADDED, as one plugged in before launch; unplugged, plugged in again, a second
   one, the first replugged. Each step's slots are printed; exit 0 when every step holds. */
static int virtual_pad(void)   /* its instance ID, or -1 */
{
    SDL_VirtualJoystickDesc d;
    SDL_zero(d);
    d.version = SDL_VIRTUAL_JOYSTICK_DESC_VERSION;
    d.type = SDL_JOYSTICK_TYPE_GAMECONTROLLER;
    d.naxes = SDL_CONTROLLER_AXIS_MAX;
    d.nbuttons = SDL_CONTROLLER_BUTTON_MAX;
    d.name = "xmen2 selftest pad";
    int index = SDL_JoystickAttachVirtualEx(&d);
    return index < 0 ? -1 : (int)SDL_JoystickGetDeviceInstanceID(index);
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
    if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "[native] pad selftest: SDL: %s\n", SDL_GetError());
        return 2;
    }
    int steps = 0, good = 0;
    auto expect = [&](const char *what, int p1, int p2) {   /* instance IDs in slots 1 and 2, -1 empty; 3-4 empty */
        int got[4];
        for (int n = 0; n < 4; n++) got[n] = s_pad[n] ? (int)pad_id(s_pad[n]) : -1;
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
    if (s_pad[0]) {   /* its button reaches player 1 only */
        SDL_JoystickSetVirtualButton(SDL_GameControllerGetJoystick(s_pad[0]), SDL_CONTROLLER_BUTTON_A, 1);
        uint16_t pads[4];
        host_input(pads);
        bool ok = pads[0] == GPAD_B && !pads[1] && !pads[2] && !pads[3];
        steps++;
        good += ok;
        fprintf(stderr, "[native] pad selftest: its A button: pads %04x %04x %04x %04x (want %04x 0 0 0) %s\n", pads[0], pads[1],
                pads[2], pads[3], GPAD_B, ok ? "ok" : "WRONG");
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

void host_audio(uint64_t frame, bool mute)
{
    wav_maybe_start();
    /* the chips' writes of the Z80's frame (stamped in master clocks from its start), mixed, to
       the device (as the engine's sim step: GENESIS_SIM_WALL_FRAME_MASTER_CYCLES); muted (a
       rollback's replay of frames already heard), the chips still run */
    static int16_t fm[8192 * 2], psg[16384];
    size_t fm_n = 0, psg_n = 0;
    audio_mixer_drain(895780u, fm, 8192, &fm_n, psg, sizeof psg / sizeof psg[0], &psg_n);
    /* phase 7 R9: players 3-4's parts; music_voices: the music (XMEN2_STEM=sfx: silent) */
    sound_mix(fm, fm_n, psg, psg_n, mute || capture_mute_snd2());
    if (s_audio && !mute) audio_flush((uint32_t)frame, 1, fm, fm_n, psg, psg_n);
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

static const uint32_t *osd_overlay(const uint32_t *fb, int *stride, int w, int h)
{
    static uint32_t buf[256 * 512];
    if (!s_osd[0] || SDL_GetTicks64() >= s_osd_until || w > 512 || h > 256) return fb;
    for (int y = 0; y < h; y++) memcpy(buf + y * 512, fb + y * *stride, (size_t)w * 4);
    int n = (int)strlen(s_osd), tw = n * 8, x0 = (w - tw) / 2, y0 = 8;
    for (int y = y0 - 3; y < y0 + 11; y++)
        for (int x = x0 - 4; x < x0 + tw + 3; x++) {
            if (x < 0 || x >= w || y < 0 || y >= h) continue;
            uint32_t p = buf[y * 512 + x];   /* 2/3 black over the picture */
            buf[y * 512 + x] = (p & 0xFF000000u) | ((p >> 16 & 0xFF) / 3) << 16 | ((p >> 8 & 0xFF) / 3) << 8 | (p & 0xFF) / 3;
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
    *stride = 512;
    return buf;
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
    fb = osd_overlay(fb, &stride, w, h);
    if (osd_dump) host_screenshot((std::string(getenv("XMEN2_DUMP_DIR")) + "/osd.bmp").c_str(), fb, stride, w, h);
    SDL_Rect src = { 0, 0, w, h };
    int bad = SDL_UpdateTexture(s_tex, &src, fb, stride * 4) != 0;
    int ww, wh;
    if (SDL_GetRendererOutputSize(s_ren, &ww, &wh) != 0 || ww <= 0 || wh <= 0) { s_draw_fails++; return; }
    double sx = (double)ww / w, sy = (double)wh / (h * 1.0);
    double s = sx < sy ? sx : sy;
    SDL_Rect dst = { (int)((ww - w * s) / 2), (int)((wh - h * s) / 2), (int)(w * s), (int)(h * s) };
    SDL_SetRenderDrawColor(s_ren, 0, 0, 0, 255);
    SDL_RenderClear(s_ren);
    bad |= SDL_RenderCopy(s_ren, s_tex, &src, &dst) != 0;
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
