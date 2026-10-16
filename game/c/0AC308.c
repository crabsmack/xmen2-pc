/* FloatBubbleExplode_Init - FLOATBUBBLEEXPLODE constructor (class slot +$12): ACTOR's Init, then the
 * burst's look: layer 0, sprite priority, flags6 |= $508A, sprite definition
 * Art_FloatBubbleExplodeSpr in palette 0 in the jungle scenes (SavageC1, SavageB1), else 2; first
 * state (and default) FloatBubbleExplode_StateStart.
 *
 * self:  the new object.
 * x, y:  position (8 fraction bits).
 * place: its placement (0 from FloatBubbleExplode_Spawn). */
#include "xmen2.h"

void FloatBubbleExplode_Init(FloatBubbleExplode *self, long x, long y, Placement *place)
{
	register short pal = g_ScnSavageC1 == g_SceneIndex || g_ScnSavageB1 == g_SceneIndex ? 0 : 2;

	(*CLASS(Tbl_FloatBubbleExplode_vtable)->parent->method[3])(self, x, y, place);	/* ACTOR Init */
	if (1) {	/* the ROM pops the call's arguments at the end of this block */
		self->layer = 0;
		self->spr.flags |= 4;
		self->flags6 |= 0x508A;
	}
	self->spr.def = CODEPTR(Art_FloatBubbleExplodeSpr);
	self->spr.flags = self->spr.flags & ~3 | pal;
	self->state = self->defaultState = (StateFn)FloatBubbleExplode_StateStart;
}
