/* BigAcolyte_StateBecomeSprite - BIGACOLYTE state (out of the wall): from the scaled picture back
 * to its own sprite (Spr_Init with its definition, palette and VRAM tiles, no frame), high
 * priority, drawn in front (DrawList_SetPriority), facing left, moved by (18, 17) px onto the
 * picture's place, the idle animation (Tbl_BigAcolyteAnim); then it glides to x $1B49 and
 * becomes hittable (BigAcolyte_StateJoinList).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short BigAcolyte_StateBecomeSprite(register BigAcolyte *self)
{
	Spr_Init(&self->spr, 1, self->spr.def, -1, self->spr.flags & 3, self->spr.tileBase, 0);
	self->picture = 0;
	self->spr.flags |= 4;
	DrawList_SetPriority(self->drawNode, 1, self->layer, self->flags6 & 8);
	self->spr.flags |= 0x10;
	self->x += 0x1200;
	self->y += 0x1100;
	Obj_SetAnim(self, CODEPTR(Tbl_BigAcolyteAnim));
	BigAcolyte_SetTargetX(self, 0x1B49);
	SET_STATE(self, (StateFn)BigAcolyte_StateJoinList);
	return self->flags & 8;
}
