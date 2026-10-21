/* host_sdl.h - the native runner's window, input and sound (host_sdl.cpp). */
#ifndef XMEN2_HOST_SDL_H
#define XMEN2_HOST_SDL_H

#include <stdint.h>

int  host_open(const char *title);           /* 0: no display */
void host_input(uint16_t pads[2]);           /* GPAD_* masks of players 1 and 2; quits on Esc / close */
void host_audio(uint64_t frame);             /* the Z80 frame's sound, mixed, to the device */
void host_present(const uint32_t *fb, int stride, int w, int h);   /* the picture, then paced to NTSC */
void host_screenshot(const char *path, const uint32_t *fb, int stride, int w, int h);   /* BMP */

#endif /* XMEN2_HOST_SDL_H */
