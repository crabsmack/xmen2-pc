/* host_sdl.h - the native runner's window, input and sound (host_sdl.cpp). */
#ifndef XMEN2_HOST_SDL_H
#define XMEN2_HOST_SDL_H

#include <stddef.h>
#include <stdint.h>

int  host_open(const char *title);           /* 0: no display */
void host_input(uint16_t pads[4]);           /* GPAD_* masks of players 1-4; quits on Esc / close */
void host_audio(uint64_t frame, bool mute);  /* the Z80 frame's sound, mixed, to the device (mute: not heard) */
void host_present(const uint32_t *fb, int stride, int w, int h);   /* the picture, then paced to NTSC */
void host_present_at(const uint32_t *fb, int stride, int w, int h, double t_us);   /* high_fps: the picture
                                                                                      at t_us on the present clock */
void host_slip(double us);   /* netplay: both present clocks move by us (the scheduler held a frame) */
/* pacing=display: the renderer presents on the display's refresh (vsync), each picture at a refresh
   of its own, the one nearest its time on the present clock (host_present_at's) */
bool host_vsync(void);       /* display pacing is on (a window, vsync, a known refresh period) */
int  host_refreshes(double t0_us, double t1_us, double *t_us, int max);   /* the refreshes in [t0, t1), on the
                                                                             present clock; how many */
double host_refresh_us(void);   /* the refresh period on the present clock (us) */
void host_present_refresh(const uint32_t *fb, int stride, int w, int h, double t_us);
void host_screenshot(const char *path, const uint32_t *fb, int stride, int w, int h);   /* BMP */
int  host_pad_selftest(void);   /* XMEN2_PAD_SELFTEST: the controllers' slots with virtual ones; 0 = pass */
void host_message(const char *text);   /* a message box (before or without the window) */
void host_osd(const char *text);       /* a hotkey's message on the picture for 2 s (up to 47 characters) */
bool host_pick_rom(char *path, size_t cap);   /* a file picker for the ROM; false: cancelled or none */

#endif /* XMEN2_HOST_SDL_H */
