/* Deathbird_Move - DEATHBIRD Move (class slot +$4E; the parent's isn't called): move by moveKind
 * for the frame's steps: 4 circle (Deathbird_MovePolar with the circling globals), 3 fly by
 * velocity and acceleration plus the wobble, anything else stay still.
 *
 * self: the boss.  steps: movement steps this frame (2 in two-player step mode, else 1).
 * The wobble: an offset (wobbleX/Y, -100..100) added to the position after each step, its rate
 * pushed back (-/+4 in x, -/+3 in y) once the position is past -/+$60 from the anchor, and the
 * anchor moved to the position while that speed is slow (under 16) or has just changed sign.
 * The distance from the anchor is taken in the low words of the 8.8 positions, as the original
 * does: $60 is 0.375 px, and it wraps every 256 px.
 * The cases are Deathbird_Move_Polar, _Wobble and _Done (the end). */
#include "xmen2.h"

#define g_DeathbirdHeading	(*(s32 *)&g_SceneVar3B20)	/* scene variable: circling heading, degrees, 8 fraction bits */
#define g_DeathbirdTurnRate	(*(s32 *)g_SceneVar3B24)	/* scene variable: added to the heading each step */
#define g_DeathbirdTurnAccel	(*(s32 *)g_SceneVar3B28)	/* scene variable: added to the turn rate each step */

void Deathbird_Move(Deathbird *self, short steps)
{
	short dx, dy;

	switch (self->moveKind) {
	case 0:
	case 1:
	case 2:
		break;
	case 4:				/* Deathbird_Move_Polar */
		Deathbird_MovePolar(&self->speed, &self->accel, &self->maxSpeed,
			&g_DeathbirdHeading, &g_DeathbirdTurnRate, &g_DeathbirdTurnAccel,
			&self->x, &self->y, &self->vx, &self->vy, &self->prevVx, &self->prevVy, steps);
		break;
	case 3:				/* Deathbird_Move_Wobble */
		do {
			self->prevVx = self->vx;
			self->prevVy = self->vy;
			self->x += self->vx;
			self->y += self->vy;
			self->vx += self->ax;
			self->vy += self->ay;
			dx = (short)(self->x - self->anchorX);
			dy = (short)(self->y - self->anchorY);
			self->wobbleX += self->wobbleVx;
			self->wobbleY += self->wobbleVy;
			if (self->wobbleX > 100)
				self->wobbleX = 100;
			else if (self->wobbleX < -100)
				self->wobbleX = -100;
			if (self->wobbleY > 100)
				self->wobbleY = 100;
			else if (self->wobbleY < -100)
				self->wobbleY = -100;
			if (dx > 0x60)
				self->wobbleVx = -4;
			else if (dx < -0x60)
				self->wobbleVx = 4;
			if (dy > 0x60)
				self->wobbleVy = -3;
			else if (dy < -0x60)
				self->wobbleVy = 3;
			if (ABS(self->vx) < 16 || ((self->vx & 0x8000) ^ (self->prevVx & 0x8000)) > 0)
				self->anchorX = self->x;
			if (ABS(self->vy) < 16 || ((self->vy & 0x8000) ^ (self->prevVy & 0x8000)) > 0)
				self->anchorY = self->y;
			self->x += self->wobbleX;
			self->y += self->wobbleY;
		} while (--steps > 0);
		break;
	}
	/* Deathbird_Move_Done */
}
