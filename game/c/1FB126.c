/* Spr_InvalidateFrame - make a sprite instance upload its frame's tiles again at its next draw.
 *
 * spr: the sprite instance (object+$2E). Flags bit 5 (frame uploaded) off, no frame uploaded ($FF). */
#include "xmen2.h"

void Spr_InvalidateFrame(SprInst *spr)
{
	spr->flags &= ~0x20;
	spr->uploaded = 0xFF;
}
