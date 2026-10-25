/* CoreElevator_Init - COREELEVATOR constructor (class slot +$12): the parent's (ACTOR) Init, then
 * an idle lift.
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry (never null).
 * Sprite set g_CoreElevatorSprDef; state and default state CoreElevator_StateIdle; flags6 |= $5283;
 * sprite priority (bit 2) copied from the live player (player 1 when neither is); not moving;
 * animation Tbl_CoreElevatorAnim; boxes. Stops and speed from the placement: param[1] the upper
 * stop (0: y - $60), param[2] the lower (0: y + $60), param[3] the speed (0: $200). */
#include "xmen2.h"

void CoreElevator_Init(register CoreElevator *self, long x, long y, register Placement *pl)
{
	register short v;		/* the placement param being read */

	(*CLASS(Tbl_CoreElevator_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	Obj_SetSpriteDef((Actor *)self, g_CoreElevatorSprDef);
	self->state = self->defaultState = CoreElevator_StateIdle;
	self->flags6 |= 0x5283;
	if (IS_ALIVE(&g_Player1)) {
		if (g_Player1.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	} else if (IS_ALIVE(&g_Player2)) {
		if (g_Player2.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	} else
#if defined(XMEN2_NATIVE)
	if (!P34_CopyLiveLayer(&self->spr, 1))	/* players 1-2 out: a live player 3-4's (R11) */
#endif
	{
		if (g_Player1.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	}
	STOP_MOVING(self);
	Obj_SetAnim(self, CODEPTR(Tbl_CoreElevatorAnim));
	Obj_UpdateBoxes(self);
	self->top = (v = pl->param[1]) ? v : pl->y - 0x60;
	self->bottom = (v = pl->param[2]) ? v : pl->y + 0x60;
	self->speed = (v = pl->param[3]) ? v : 0x200;
}
