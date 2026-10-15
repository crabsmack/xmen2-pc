/* SavageBio_ActDashTo - SAVAGEBIGBIO script action: dash up to the nearest player (speed $220,
 * acceleration $56, sound $EA): brake halfway, stop at the player's x (SavageBio_StateDash,
 * SavageBio_StateDashBrake). Within $20 px it only turns to face it, in $3C frames
 * (SavageBio_ActFacePlayer).
 *
 * self: the boss (a step of its AI script; never runs in the corpus).
 * At an end of the arena it first steps 1 px inside and turns to face the other end; then it
 * faces the player. With no arena (left = right) it just stops and goes on with the script in
 * $3C frames.
 * Returns self->flags & 8 unless the script goes on. */
#include "xmen2.h"

short SavageBio_ActDashTo(SavageBigBio *self)
{
	XPlayer *p = Player_Nearest((Actor *)self);
	short px = p->x >> 8;
	short x = self->x >> 8;

	if (self->arenaLeft == self->arenaRight) {
		STOP_MOVING(self);
		AI_NEXT_STEP(self, self->ai, 0x3C);
	} else if (ABS(x - px) < 0x20) {
		SET_STATE_WAIT(self, SavageBio_ActFacePlayer, 0x3C);
		return self->flags & 8;
	} else {
		self->vy = 0;
		self->ay = 0;
		if (x == self->arenaLeft) {
			self->x += 0x100;
			self->spr.flags &= ~0x10;
		} else if (x == self->arenaRight) {
			self->x -= 0x100;
			self->spr.flags |= 0x10;
		}
		self->vx = (self->spr.flags & 0x10) ? -0x220 : 0x220;
		Snd_Stop(0xEA);
		Snd_Play(0xEA);
		self->dashBrakeX = ((px - x) >> 1) + x;
		self->dashStopX = px;
		if (x < self->dashBrakeX)
			self->spr.flags &= ~0x10;
		else
			self->spr.flags |= 0x10;
		self->ax = (self->spr.flags & 0x10) ? -0x56 : 0x56;
		SET_STATE(self, SavageBio_StateDash);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
