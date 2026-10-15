/* BigBio_StateStart - BIGBIO's first state: no animation, frame 6, orbs not drawn, upright and
 * facing right, standing on the floor segment (type $21) nearest its position; top speeds $7000;
 * then wait for the players (BigBio_StateWaitPlayers).
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BigBio_StateStart(register BigBio *self)
{
	self->anim = 0;
	(*self->cls->method[20])(self, 6);		/* SetFrame */
	self->orbMode &= ~8;
	self->spr.flags &= ~8;
	self->spr.flags &= ~0x10;
	self->x = self->px << 8;
	Obj_StandOnSeg(self, Seg_FindNearestOfType(self->x >> 8, self->y >> 8, 0x21));
	self->maxVx = 0x7000;
	self->maxVy = 0x7000;
	SET_STATE(self, BigBio_StateWaitPlayers);
	return self->flags & 8;
}
