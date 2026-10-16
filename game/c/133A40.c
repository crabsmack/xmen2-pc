/* BrainRocket_StateSetDirection - BRAINROCKET state after BrainRocket_StateLaunch: the velocity
 * and animation of its heading, then BrainRocket_StateFly from the next update.
 *   0 right $380 (Tbl_BrainRocketAnim2), 45 up-right $140 each (Tbl_BrainRocketAnim3), 90 up
 *   $380 (Tbl_BrainRocketAnim4), 135 up-left (Anim3), 180 left (Anim2), 270 down (Anim4,
 *   v-flipped); another heading: no change.
 *
 * self: the rocket. Returns flags & 8. */
#include "xmen2.h"

short BrainRocket_StateSetDirection(BrainRocket *self)
{
	switch (self->angle) {
	case 0:
		self->vx = 0x380;
		self->vy = 0;
		Obj_SetAnim(self, CODEPTR(Tbl_BrainRocketAnim2));
		break;
	case 45:
		self->vx = 0x140;
		self->vy = -0x140;
		Obj_SetAnim(self, CODEPTR(Tbl_BrainRocketAnim3));
		break;
	case 90:
		self->vx = 0;
		self->vy = -0x380;
		Obj_SetAnim(self, CODEPTR(Tbl_BrainRocketAnim4));
		break;
	case 135:
		self->vx = -0x140;
		self->vy = -0x140;
		Obj_SetAnim(self, CODEPTR(Tbl_BrainRocketAnim3));
		break;
	case 180:
		self->vx = -0x380;
		self->vy = 0;
		Obj_SetAnim(self, CODEPTR(Tbl_BrainRocketAnim2));
		break;
	case 270:
		self->vx = 0;
		self->vy = 0x380;
		Obj_SetAnim(self, CODEPTR(Tbl_BrainRocketAnim4));
		self->spr.flags |= 8;
		break;
	}
	SET_STATE(self, (StateFn)BrainRocket_StateFly);
	return self->flags & 8;
}
