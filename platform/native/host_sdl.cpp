/* host_sdl.cpp - the native runner's window, input and sound (phase 5 P8): the game played on the
 * PC. SDL2 for the window, the keyboard and game controllers and the audio device; the sound chips
 * and their mixer are the engine's (audio/: ymfm's YM2612, the SN76489, the cycle-stamped event
 * queue the Z80's writes go into, audio.c's device bridge).
 *
 *   Keyboard (player 1): arrows; Z = A, X = B, C = C, Enter = Start; A = X, S = Y, D = Z, Q = Mode.
 *   Game controllers (players 1 and 2): D-pad or left stick; X = A, A = B, B = C, Start;
 *   LB = X, Y = Y, RB = Z, Back = Mode.  Esc or closing the window quits.
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

int host_open(const char *title)
{
    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "[native] SDL: %s\n", SDL_GetError());
        return 0;
    }
    s_win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (g_opt.widescreen ? 400 : 320) * 3,
                             224 * 3, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!s_win) { fprintf(stderr, "[native] SDL window: %s\n", SDL_GetError()); return 0; }
    if (!make_renderer()) return 0;
    for (int i = 0, n = 0; i < SDL_NumJoysticks() && n < 4; i++)
        if (SDL_IsGameController(i) && (s_pad[n] = SDL_GameControllerOpen(i))) n++;
    s_audio = audio_init((int)GENESIS_PSG_SAMPLE_RATE_NTSC) == 0;
    if (s_audio) audio_set_playback_enabled(1);
    else fprintf(stderr, "[native] no audio device: playing silent\n");
    s_period = (double)SDL_GetPerformanceFrequency() * 262.0 * 3420.0 / 53693175.0;   /* one NTSC frame */
    s_next = SDL_GetPerformanceCounter();
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
            SDL_SetWindowFullscreen(s_win, (f & SDL_WINDOW_FULLSCREEN_DESKTOP) ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
        }
        /* phase 6 R1: the two presentation options */
        if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_F9 && !e.key.repeat) options_toggle_high_fps();
        if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_F10 && !e.key.repeat) options_toggle_widescreen();
        if (e.type == SDL_RENDER_DEVICE_RESET) s_rebuild = true;
        if (e.type == SDL_CONTROLLERDEVICEADDED) {
            for (int n = 0; n < 4; n++)
                if (!s_pad[n]) { s_pad[n] = SDL_GameControllerOpen(e.cdevice.which); break; }
        }
    }
    pads[0] = (uint16_t)(keyboard_pad() | controller_pad(s_pad[0]));
    pads[1] = controller_pad(s_pad[1]);
    pads[2] = controller_pad(s_pad[2]);
    pads[3] = controller_pad(s_pad[3]);
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
    snd2_mix(fm, fm_n, psg, psg_n, mute || capture_mute_snd2());   /* phase 7 R9: players 3-4's parts (XMEN2_STEM=sfx: silent) */
    if (s_audio && !mute) audio_flush((uint32_t)frame, 1, fm, fm_n, psg, psg_n);
}

static void draw(const uint32_t *fb, int stride, int w, int h)
{
    if ((s_rebuild || s_draw_fails >= 30) && !make_renderer()) return;
    if (!s_ren || !s_tex) return;
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
    SDL_RenderPresent(s_ren);
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
    draw(fb, stride, w, h);
}

void host_present(const uint32_t *fb, int stride, int w, int h)
{
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
