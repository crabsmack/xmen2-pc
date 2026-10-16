/* BroodClaw_Init - BROODCLAW constructor (class slot +$12): ACTOR's Init, then a still claw hanging
 * 28 px above its placement, swinging with BroodClaw_Swing once it moves.
 *
 * self:  the new claw.
 * x, y:  position (8 fraction bits; replaced by the placement's).
 * place: its placement entry (x, y: pixels).
 * Sprite definition g_BroodClawSprDef, flags6 |= $1082, state and default state
 * BroodClaw_StateWaitMusic, swinging out (not back), no pair, swing length $32 at offset 0, home
 * x = px; its boxes from the start. */
#include "xmen2.h"

#define SWING_BACK(o)	((o)->unk_8A[1])	/* ACTOR +$8B, for a claw: 0 swinging out, 1 swinging back (BroodClaw_Swing; other classes use the byte differently) */

void BroodClaw_Init(register BroodClaw *self, long x, long y, register Placement *place)
{
	(*CLASS(Tbl_BroodClaw_vtable)->parent->method[3])(self, x, y, place);	/* ACTOR Init */
	STOP_MOVING(self);
	self->px = place->x;
	self->py = place->y + -0x1C;
	self->x = self->px << 8;
	self->y = self->py << 8;
	Obj_SetSpriteDef((Actor *)self, g_BroodClawSprDef);
	self->flags6 |= 0x1082;
	self->state = self->defaultState = (StateFn)BroodClaw_StateWaitMusic;
	SWING_BACK(self) = 0;		/* swinging out */
	self->scrollEnded = 0;
	self->motion = BroodClaw_Swing;
	self->pair = 0;
	self->swingLen = 0x32;
	self->swingOffset = 0;
	self->homeX = self->px;
	Obj_UpdateBoxes(self);
}
