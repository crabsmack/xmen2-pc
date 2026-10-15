/* Actor_SetFrame - ACTOR SetFrame (class slot +$56): show frame `frame` of the object's sprite.
 *
 * obj: the object, any Actor class (Anim_ApplyStep calls it for each frame step).  f: the frame ($FF none).
 * Sprite flags bit 5 says whether the frame's tiles are already in VRAM (the frame uploaded);
 * when they aren't, the object is redrawn (flags |= 8). */
#include "xmen2.h"

void Actor_SetFrame(void *obj, short f)
{
	register Actor *self = (Actor *)obj;
	register short frame = f;
	register SprInst *spr = &self->spr;

	spr->frame = (u8)frame;
	if (frame != 0xFF && frame == spr->uploaded)
		spr->flags |= 0x20;
	else {
		spr->flags &= ~0x20;
		self->flags |= 8;
	}
}
