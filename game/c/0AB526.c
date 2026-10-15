/* FloatBubble_SetDirection - FLOATBUBBLE: set off in its direction (dir 0-7: 0 up, 1 up-right,
 * 2 right... 7 up-left): accelerate 8 a frame that way on each axis it moves along, turn its
 * speed on that axis to point the same way and stop it on the other.
 *
 * self: the bubble (FloatBubble_StateStart, FloatBubble_StateRelease).
 * Other directions change nothing. The cases are in the order the original listed them
 * (counter-clockwise from up). */
#include "xmen2.h"

void FloatBubble_SetDirection(FloatBubble *self)
{
	switch (self->dir) {
	case 0:				/* FloatBubble_SetDirection_Up */
		self->ax = 0;
		self->ay = -8;
		self->vx = 0;
		self->vy = -ABS(self->vy);
		break;
	case 7:				/* up-left */
		self->ax = -8;
		self->ay = -8;
		self->vx = -ABS(self->vx);
		self->vy = -ABS(self->vy);
		break;
	case 6:				/* FloatBubble_SetDirection_Left */
		self->ax = -8;
		self->ay = 0;
		self->vx = -ABS(self->vx);
		self->vy = 0;
		break;
	case 5:				/* down-left */
		self->ax = -8;
		self->ay = 8;
		self->vx = -ABS(self->vx);
		self->vy = ABS(self->vy);
		break;
	case 4:				/* FloatBubble_SetDirection_Down */
		self->ax = 0;
		self->ay = 8;
		self->vx = 0;
		self->vy = ABS(self->vy);
		break;
	case 3:				/* FloatBubble_SetDirection_DownRight */
		self->ax = 8;
		self->ay = 8;
		self->vx = ABS(self->vx);
		self->vy = ABS(self->vy);
		break;
	case 2:				/* FloatBubble_SetDirection_Right */
		self->ax = 8;
		self->ay = 0;
		self->vx = ABS(self->vx);
		self->vy = 0;
		break;
	case 1:				/* FloatBubble_SetDirection_UpRight */
		self->ax = 8;
		self->ay = -8;
		self->vx = ABS(self->vx);
		self->vy = -ABS(self->vy);
		break;
	}
	/* FloatBubble_SetDirection_Done */
}
