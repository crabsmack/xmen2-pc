/* Wolverine_StateLunge - the air lunge (Wolverine_AirSpecial): rising fast, it is slowed; on landing
 * he slashes on the ground, hopping forward into a leaping slash while the animation step is
 * below 5, else the slashes end (Wolverine_StateLungeEnd).
 *
 * self: the player.
 * Animation over: unlatch A and the control state (slot +$A6), now (returns nothing). In the
 * air, vy below -$300 gains $100 (returns nothing). Landed: the leaping-slash animation (strong
 * Tbl_WolverineAnim7 above 7 health units, else Tbl_WolverineAnim) replaces the lunge's without
 * a restart, its step moved on by 2; below step 5 he leaves the floor (Obj_Detach) with vy
 * -$300, gravity $3F and vx $396 the way he faces (maximum $396), and Wolverine_LeapSlashEnd in
 * 20 frames.
 * Returns self->flags & 8 after landing. */
#include "xmen2.h"

short Wolverine_StateLunge(register Wolverine *self)
{
	if (!self->anim) {
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskA;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskA;
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		RETURN_NO_VALUE;
	}
	if (!(self->flags & 0x1000)) {
		if (self->health >= 0x701) {
			self->anim = CODEPTR(Tbl_WolverineAnim7);
			self->animStep += 2;
		} else {
			self->anim = CODEPTR(Tbl_WolverineAnim);
			self->animStep += 2;
		}
		if (5 > self->animStep) {
			Obj_Detach(self);
			self->vy = -0x300;
			self->ay = 0x3F;
			self->maxVx = 0x396;
			self->vx = self->spr.flags & 0x10 ? -0x396 : 0x396;
			SET_STATE_WAIT(self, (StateFn)Wolverine_LeapSlashEnd, 0x14);
			return self->flags & 8;
		}
		SET_STATE(self, (StateFn)Wolverine_StateLungeEnd);
		return self->flags & 8;
	}
	if (self->vy < -0x300)
		self->vy += 0x100;
	NO_RETURN_VALUE
}
