/* XPlayer_DropThrough - XPLAYER class slot +$86 (Down+C): drop through the floor: the camera keeps
 * its look offset, leave the floor, and from a layer above 0 go one layer down and $25 px lower;
 * then a jump without the upward speed: jump sound ($FF: none), the drop frame.
 *
 * self: the player. playerFlags 8 (it jumped); the animation stops. */
#include "xmen2.h"

void XPlayer_DropThrough(XPlayer *self)
{
	Cam_CommitLook(g_Camera);
	Obj_Detach(self);
	Seg_AlignObjVelocity(self);
	if (self->layer > 0) {
		self->layer--;
		self->y += 0x2500;
	}
	Obj_Detach(self);
	g_CamModeSwitched = 1;
	self->playerFlags |= 8;
	self->anim = 0;
	if (self->charRec->jumpSound != 0xFF) {
		Snd_Stop(self->charRec->jumpSound);
		Snd_Play(self->charRec->jumpSound);
	}
	(*self->cls->method[20])(self, self->charRec->dropFrame);	/* SetFrame */
}
