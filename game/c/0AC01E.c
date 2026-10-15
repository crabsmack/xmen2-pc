/* FloatBubble_Init - FLOATBUBBLE Init (class slot +$12): the parent's (ACTOR) Init, then a
 * bubble that doesn't move yet.
 *
 * self: the new object.  x, y: position (8 fraction bits), also its first wobble anchor.
 * pl:   its placement entry (FloatBubble_Spawn makes one), or 0: then it drifts up at speed
 *       $100 (max $100, start $80 on both axes). With one: param 0 & 7 the direction, param 1
 *       the speed index (0-4: max speed $100-$500, other values $100), param 2 (when
 *       nonzero) unk_92, param 4 or'ed into bubbleFlags (which starts as 4: moving).
 * Sprite definition g_FloatBubbleSprDef; sprAttr = its first tile with palette 0 in the jungle
 * scenes (SavageC1, SavageB1), else 2. Layer 0, high priority, flags6 |= $88. First state
 * FloatBubble_StateGrowStart when bubbleFlags bit 0 is set, else FloatBubble_StateStart;
 * hurt state FloatBubble_StatePop. Wobble: offset 0, change +4/+3, limits 100. */
#include "xmen2.h"

void FloatBubble_Init(FloatBubble *self, long x, long y, Placement *pl)
{
	short v = g_ScnSavageC1 == g_SceneIndex || g_ScnSavageB1 == g_SceneIndex ? 0 : 2;	/* palette */

	(*CLASS(Tbl_FloatBubble_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	STOP_MOVING(self);
	self->layer = 0;
	self->spr.flags |= 4;
	self->flags6 |= 0x88;
	Obj_SetSpriteDef((Actor *)self, g_FloatBubbleSprDef);
	self->sprAttr = g_SpriteDefs[g_FloatBubbleSprDef].tileBase | v << 13;
	/* The original had a loop label here (a do { } while (0) macro, probably): MPW flushes the
	   pending argument pops of the two calls before it. */
	do {
		self->bubbleFlags = 4;
		self->unk_8E = 0;
		self->dir = 0;
		self->unk_92 = 0x20;
	} while (0);
	v = 0x100;				/* from here the speed */
	self->anchorX = x;
	self->anchorY = y;
	self->wobbleX = 0;
	self->wobbleY = 0;
	self->wobbleAx = 4;
	self->wobbleAy = 3;
	self->wobbleMaxX = 100;
	self->wobbleMaxY = 100;
	if (pl) {
		self->dir = pl->param[0] & 7;
		switch (pl->param[1]) {
		default:
		case 0:				/* FloatBubble_Init_Speed1 */
			v = 0x100;
			break;
		case 1:				/* FloatBubble_Init_Speed2 */
			v = 0x200;
			break;
		case 2:
			v = 0x300;
			break;
		case 3:
			v = 0x400;
			break;
		case 4:				/* FloatBubble_Init_Speed5 */
			v = 0x500;
			break;
		}
		/* FloatBubble_Init_SetSpeed */
		if (pl->param[2])
			self->unk_92 = pl->param[2];
		self->bubbleFlags |= pl->param[4];
	}
	self->maxVx = self->maxVy = v;
	self->vx = v >> 1;
	self->vy = v >> 1;
	if (self->bubbleFlags & 1) {
		self->state = FloatBubble_StateGrowStart;
		self->defaultState = FloatBubble_StateGrowStart;
	} else {
		self->state = FloatBubble_StateStart;
		self->defaultState = FloatBubble_StateStart;
	}
	self->hurtState = FloatBubble_StatePop;
}
