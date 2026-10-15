/* Beast_StateRoll - Beast's rolling leap (Beast_StartRoll): it ends against a wall (flags $100)
 * or when countdown runs out (Beast_StateRollEnd once the roll animation, Tbl_Anim_1829F8,
 * ends); landing during it rolls on along the floor at $1E0 and lands at the animation's end
 * (Beast_RollLand).
 *
 * self: the player.
 * At countdown's end he rolls on at $1E0 the way he faces. In the air, falling faster than $96,
 * the falling roll frame $97.
 * Returns self->flags & 8 when it changes state; nothing in the air. */
#include "xmen2.h"

short Beast_StateRoll(register Beast *self)
{
	if (self->flags & 0x100) {
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1829F8));
		SET_STATE(self, (StateFn)Beast_StateRollEnd);
		return self->flags & 8;
	}
	if (self->countdown <= 0) {
		self->vx = self->spr.flags & 0x10 ? -0x1E0 : 0x1E0;
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1829F8));
		SET_STATE(self, (StateFn)Beast_StateRollEnd);
		return self->flags & 8;
	}
	if (self->flags & 0x1000) {
		if (self->vy > 0x96)
			(*self->cls->method[20])(self, 0x97);	/* SetFrame */
		RETURN_NO_VALUE;
	}
	Obj_SetAnim(self, CODEPTR(Tbl_Anim_1829F8));
	self->vx = self->spr.flags & 0x10 ? -0x1E0 : 0x1E0;
	SET_STATE_WAIT(self, (StateFn)Beast_RollLand, 0xFF);
	return self->flags & 8;
}
