/* Psylocke_StateGrabWall - PSYLOCKE climbing state (class slot +$AA, after XClimber_CheckGrab): grab
 * the wall: stop, face it and put her front against it, then cling (Psylocke_StateCling) or,
 * when C was pressed as she grabbed it (climbFlags $40), leap off it again (Psylocke_WallJump in
 * 8 frames).
 *
 * self: the player (climbSeg: the wall; climbFlags & 3: 2 wall on her right, 3 on her left).
 * Clears playerFlags 2 and 4 (dive and double jump used), stateTimer and unk_A8, the animation
 * and the B latch; tells the camera (g_CamModeSwitched); sound $81. The cling frame (200) is
 * fitted to the wall: x moves by the distance from the box's front to the wall's x.
 * Returns self->flags & 8, nothing when the climb state is no wall. */
#include "xmen2.h"

short Psylocke_StateGrabWall(register Psylocke *self)
{
	register Seg *wall = self->climbSeg;

	self->playerFlags &= ~2;
	self->playerFlags &= ~4;
	g_CamModeSwitched = 1;
	self->stateTimer = 0;
	self->unk_A8 = 0;
	STOP_MOVING(self);
	self->anim = 0;
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
	Snd_Stop(0x81);
	Snd_Play(0x81);
	switch (self->climbFlags & 3) {
	case 2:
		self->spr.flags &= ~0x10;
		(*self->cls->method[20])(self, 200);		/* SetFrame */
		Obj_UpdateBox(self);
		self->x += (wall->x2 - self->boxLeft) << 8;
		break;
	case 3:
		self->spr.flags |= 0x10;
		(*self->cls->method[20])(self, 200);
		Obj_UpdateBox(self);
		self->x += (wall->x1 - self->boxRight) << 8;
		break;
	default:
		RETURN_NO_VALUE;
	}
	if (self->climbFlags & 0x40) {
		SET_STATE_WAIT(self, (StateFn)Psylocke_WallJump, 8);
		return self->flags & 8;
	}
	SET_STATE(self, (StateFn)Psylocke_StateCling);
	return self->flags & 8;
}
