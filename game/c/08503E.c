/* SmallBrood_ActKamikaze - SMALLBROOD AI-script action (kamikaze variant): run to the far end of
 * the patrol range (left when aiFlags bit 0, else right), then arm (SmallBrood_StateArmed).
 *
 * self: the object.
 * At the end: animation off, frame 0, flags6 |= $1000, returns flags & 8. On the way: walk
 * animation and speed; when the cooldown has run out it is reloaded and the next script step
 * follows (nothing returned). (The cooldown block is a do/while (0), as a macro would be: its
 * loop label is where MPW C pops the Obj_ChangeAnim arguments.) */
#include "xmen2.h"

short SmallBrood_ActKamikaze(SmallBrood *self)
{
	char atEnd = 0;
	short vx = self->speed;

	if (self->aiFlags & 1) {
		self->spr.flags |= 0x10;
		vx = -vx;
		if ((self->x >> 8) == self->xMin)
			atEnd = 1;
	} else {
		self->spr.flags &= ~0x10;
		if ((self->x >> 8) == self->xMax)
			atEnd = 1;
	}
	if (atEnd) {
		self->anim = 0;
		(*self->cls->method[20])(self, 0);	/* +$56 SetFrame */
		self->flags6 |= 0x1000;
		SET_STATE(self, SmallBrood_StateArmed);
		return self->flags & 8;
	}
	Obj_ChangeAnim(self, CODEPTR(Tbl_SmallBroodAnim));
	self->vx = vx;
	do {
		if (self->cooldown == 0) {
			self->cooldown = self->cooldownTime;
			AI_NEXT_STEP(self, self->ai, 0);
		}
	} while (0);
	NO_RETURN_VALUE
}
