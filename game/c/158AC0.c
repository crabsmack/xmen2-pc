/* FluxBomb_SetFrame - FLUXBOMB SetFrame (class slot +$56): show a frame of the shared flux bomb
 * sprite g_FluxBombSpr (its flags bit 5: the frame's tiles are already in VRAM).
 *
 * self: the bomb (unused).  frame: the frame ($FF: none). */
#include "xmen2.h"

void FluxBomb_SetFrame(FluxBomb *self, register short frame)
{
	register SprInst *spr = &g_FluxBombSpr;

	spr->frame = (u8)frame;
	if (frame != 0xFF && frame == spr->uploaded)
		spr->flags |= 0x20;
	else
		spr->flags &= ~0x20;
}
