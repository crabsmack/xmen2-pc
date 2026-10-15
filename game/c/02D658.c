/* Exodus_StateVulnerable - EXODUS state after an attack: hittable for 90 frames (back in the
 * enemy body hit list, Exodus_Update takes it out again), swaying in x and y (steps 5 and 10
 * degrees, sine >> 8); after those 90 frames Exodus_StateEndVulnerable.
 *
 * self: the boss.
 * Returns self->flags & 8 (the wait is hittableTimer; the test for -1, run the next state now,
 * can't be true for the u8). */
#include "xmen2.h"

short Exodus_StateVulnerable(register Exodus *self)
{
	self->bodyNode = HitList_Add(self, &g_HitListEnemyBody);
	self->hittableTimer = 90;
	self->moveFlags |= 0xC;
	self->swayAngleX = 0;
	self->swayAngleY = 0;
	self->swayStepX = 5;
	self->swayStepY = 10;
	self->swayShiftX = 8;
	self->swayShiftY = 8;
	if (self->hittableTimer == -1)
		GOTO_STATE(self, Exodus_StateEndVulnerable);
	else {
		SET_STATE_WAIT(self, Exodus_StateEndVulnerable, self->hittableTimer);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
