/* Spr_SetFrame - select the frame a sprite instance shows.
 *
 * spr: the sprite instance.  frame: the frame ($FF none).
 * Flags bit 5 (frame uploaded) is set only when the frame is the one whose tiles are in VRAM. */
#include "xmen2.h"

void Spr_SetFrame(SprInst *spr, short frame)
{
	spr->frame = (u8)frame;
	spr->flags &= ~0x20;
	if (frame != 0xFF && frame == spr->uploaded)
		spr->flags |= 0x20;
}
