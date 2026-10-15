/* SavageBio_ActDashPast - SAVAGEBIGBIO script action: dash at the nearest player and past it
 * (speed $220, acceleration $56, sound $EA): brake at the player's x, stop $82 px beyond
 * (SavageBio_StateDash, SavageBio_StateDashBrake).
 *
 * self: the boss (a step of its AI script).
 * At an end of the arena it first steps 1 px inside and turns to face the other end; then it
 * faces the player. With no arena (left = right) it just stops and goes on with the script in
 * $3C frames.
 * Returns self->flags & 8 when dashing; nothing in particular otherwise. */
#include "xmen2.h"

short SavageBio_ActDashPast(SavageBigBio *self)
{
	XPlayer *p = Player_Nearest((Actor *)self);
	short px = p->x >> 8;
	short x = self->x >> 8;

	if (self->arenaLeft == self->arenaRight) {
		STOP_MOVING(self);
		AI_NEXT_STEP(self, self->ai, 0x3C);
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
		self->dashBrakeX = px;
		if (x < self->dashBrakeX)
			self->spr.flags &= ~0x10;
		else
			self->spr.flags |= 0x10;
		self->dashStopX = ((self->spr.flags & 0x10) ? -0x82 : 0x82) + px;
		self->ax = (self->spr.flags & 0x10) ? -0x56 : 0x56;
		SET_STATE(self, SavageBio_StateDash);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
