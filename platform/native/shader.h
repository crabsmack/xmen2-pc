/* shader.h - RetroArch shader presets (.slangp) on the played picture (shader.cpp, the shader_preset
 * option): librashader's filter chain between the game's frame and the window. */
#ifndef XMEN2_SHADER_H
#define XMEN2_SHADER_H

#include <stdint.h>

struct SDL_Renderer;
struct SDL_Rect;

void shader_open(SDL_Renderer *ren);    /* after the renderer is (re)made: the option's preset, if any */
void shader_close(void);                /* before the renderer goes */
/* the frame w x h through the chain into dst of the renderer's output (after its clear); false: no chain
   (none set, failed, or toggled off): the caller draws the plain picture */
bool shader_draw(SDL_Renderer *ren, const uint32_t *fb, int stride, int w, int h, const SDL_Rect *dst);
void shader_toggle(void);               /* played: F8, the chain off and on again */

#endif /* XMEN2_SHADER_H */
