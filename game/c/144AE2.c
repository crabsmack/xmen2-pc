/* ToxicCan_Update - TOXICCAN Update (class slot +$4A; its Timer runs it every frame).
 *
 * self: the can.
 * Out of the active window: Deactivate, Kill when out of health, and done unless still present.
 * The timer runs down (by 2 in two-player step mode). Above 2 health: frame 0; 1-2: frame 1
 * (damaged) and the leak drawn (ToxicCan_DrawLeak; its puff steps restart when it just turned
 * damaged); 0: it blows up in a chain of explosions (ExplodeFx_Chain, Tbl_ScatterExplodeChain,
 * damaging as its attack) and that is all. Objects standing on it move with it and get their
 * Update (as Actor_Update); the hit cooldown runs down. */
#include "xmen2.h"

void ToxicCan_Update(register ToxicCan *self)
{
	register short steps = g_TwoPlayerStep ? 2 : 1;
	register long x0;
	register long y0;
	register short i;
	register Actor *child;

	x0 = self->x;
	y0 = self->y;
	if ((char)(*self->cls->method[15])(self)) {		/* +$42 OutsideActiveWindow */
		(*self->cls->method[16])(self);			/* +$46 Deactivate */
		if (self->health <= 0)
			(*self->cls->method[19])(self);		/* +$52 Kill */
		if (!(self->flags & 0x8000))
			return;
	}
	if (self->timer > 0)
		self->timer -= steps;
	if (self->timer < 0)
		self->timer = 0;
	if (self->health > 2) {
		(*self->cls->method[20])(self, 0);		/* +$56 SetFrame */
	} else if (self->health > 0) {
		if (self->spr.frame != 1) {
			self->countdown = 0;
			self->leakStep = 0;
		}
		ToxicCan_DrawLeak(self->x >> 8, self->boxTop, &self->countdown, &self->leakStep);
		(*self->cls->method[20])(self, 1);		/* +$56 SetFrame: damaged */
	} else {
		ExplodeFx_Chain(self, CODEPTR(Tbl_ScatterExplodeChain), self->attack, 0, 0);
		return;
	}
	if (self->spr.firstChild != 0xFF) {
		if (self->flags & 8) {
			g_ShiftDX = (short)((self->x - x0) >> 8);
			g_ShiftDY = (short)((self->y - y0) >> 8);
			Obj_ForEachChild(self, Obj_CarryChild);
		}
		for (i = self->spr.firstChild; i != 0xFF; ) {
			child = i == 0xFF ? 0 : g_ObjTable[i];
			i = child->spr.nextSibling;
			(*child->cls->method[17])(child);	/* +$4A Update */
		}
	}
	if (self->hitCooldown > 0)
		self->hitCooldown -= steps;
	if (self->hitCooldown < 0)
		self->hitCooldown = 0;
}
