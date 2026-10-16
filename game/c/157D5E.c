/* MagneticGlob_Activate - MAGNETICGLOB Activate (class slot +$3E; the parent's isn't called):
 * put the globe into play: its sprite gets its own VRAM tiles (tiles, Spr_Init), its sprite
 * priority is the (first live) player's, its timer starts and its Draw joins the draw list. It
 * joins no hit list: it finds what it hits itself (MagneticGlob_Update).
 *
 * self: the globe. */
#include "xmen2.h"

void MagneticGlob_Activate(register MagneticGlob *self)
{
	if (self->spr.def)
		Spr_Init(&self->spr, 1, self->spr.def, -1, self->spr.flags & 3, self->tiles, 0);
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
	} else {
		if (g_Player1.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	}
	if (self->timerObj)
		Timer_Start(self->timerObj);
	self->drawNode = DrawList_Add(self->cls->method[5], self, self->spr.flags & 4, self->layer,
		self->flags6 & 8, 1);					/* +$1A Draw */
}
