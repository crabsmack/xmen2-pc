/* host_sdl.cpp - the native runner's window, input and sound (phase 5 P8): the game played on the
 * PC. SDL2 for the window, the keyboard and game controllers and the audio device; the sound chips
 * and their mixer are the engine's (audio/: ymfm's YM2612, the SN76489, the cycle-stamped event
 * queue the Z80's writes go into, audio.c's device bridge).
 *
 *   Keyboard (player 1): arrows; Z = A, X = B, C = C, Enter = Start; A = X, S = Y, D = Z, Q = Mode.
 *   Game controllers (players 1 and 2): D-pad or left stick; X = A, A = B, B = C, Start;
 *   LB = X, Y = Y, RB = Z, Back = Mode.  Esc or closing the window quits.
 * Frames are paced to the NTSC rate (59.92 Hz) by the clock; F11 toggles fullscreen.
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

static SDL_Window *s_win;
static SDL_Renderer *s_ren;
static SDL_Texture *s_tex;
static SDL_GameController *s_pad[2];
static uint64_t s_next;          /* performance-counter time of the next frame */
static double s_period;          /* counter ticks per frame */
static int s_audio;

int host_open(const char *title)
{
    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "[native] SDL: %s\n", SDL_GetError());
        return 0;
    }
    s_win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320 * 3, 224 * 3,
                             SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!s_win) { fprintf(stderr, "[native] SDL window: %s\n", SDL_GetError()); return 0; }
    s_ren = SDL_CreateRenderer(s_win, -1, SDL_RENDERER_ACCELERATED);
    if (!s_ren) s_ren = SDL_CreateRenderer(s_win, -1, 0);
    if (!s_ren) { fprintf(stderr, "[native] SDL renderer: %s\n", SDL_GetError()); return 0; }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    s_tex = SDL_CreateTexture(s_ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 512, 480);
    for (int i = 0, n = 0; i < SDL_NumJoysticks() && n < 2; i++)
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

void host_input(uint16_t pads[2])
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
        if (e.type == SDL_CONTROLLERDEVICEADDED) {
            for (int n = 0; n < 2; n++)
                if (!s_pad[n]) { s_pad[n] = SDL_GameControllerOpen(e.cdevice.which); break; }
        }
    }
    pads[0] = (uint16_t)(keyboard_pad() | controller_pad(s_pad[0]));
    pads[1] = controller_pad(s_pad[1]);
}

void host_screenshot(const char *path, const uint32_t *fb, int stride, int w, int h)
{
    SDL_Surface *s = SDL_CreateRGBSurfaceWithFormatFrom((void *)fb, w, h, 32, stride * 4, SDL_PIXELFORMAT_ARGB8888);
    if (!s || SDL_SaveBMP(s, path) != 0) fprintf(stderr, "[native] screenshot %s: %s\n", path, SDL_GetError());
    if (s) SDL_FreeSurface(s);
}

void host_audio(uint64_t frame)
{
    /* the chips' writes of the Z80's frame (stamped in master clocks from its start), mixed, to
       the device (as the engine's sim step: GENESIS_SIM_WALL_FRAME_MASTER_CYCLES) */
    static int16_t fm[8192 * 2], psg[16384];
    size_t fm_n = 0, psg_n = 0;
    audio_mixer_drain(895780u, fm, 8192, &fm_n, psg, sizeof psg / sizeof psg[0], &psg_n);
    if (s_audio) audio_flush((uint32_t)frame, 1, fm, fm_n, psg, psg_n);
}

void host_present(const uint32_t *fb, int stride, int w, int h)
{
    SDL_Rect src = { 0, 0, w, h };
    SDL_UpdateTexture(s_tex, &src, fb, stride * 4);
    int ww, wh;
    SDL_GetRendererOutputSize(s_ren, &ww, &wh);
    double sx = (double)ww / w, sy = (double)wh / (h * 1.0);
    double s = sx < sy ? sx : sy;
    SDL_Rect dst = { (int)((ww - w * s) / 2), (int)((wh - h * s) / 2), (int)(w * s), (int)(h * s) };
    SDL_SetRenderDrawColor(s_ren, 0, 0, 0, 255);
    SDL_RenderClear(s_ren);
    SDL_RenderCopy(s_ren, s_tex, &src, &dst);
    SDL_RenderPresent(s_ren);

    /* pace: one NTSC frame per frame; a late frame does not try to catch up */
    s_next += (uint64_t)s_period;
    uint64_t now = SDL_GetPerformanceCounter();
    if (now > s_next + (uint64_t)(4 * s_period)) s_next = now;
    while ((now = SDL_GetPerformanceCounter()) < s_next) {
        double left_ms = (double)(s_next - now) * 1000.0 / (double)SDL_GetPerformanceFrequency();
        if (left_ms > 2.0) SDL_Delay((Uint32)(left_ms - 1.5));
    }
}
