/* MagnetoBoss_Activate - MAGNETOBOSS Activate (class slot +$3E; replaces ACTOR's, which it
 * copies): set its sprite up in its own VRAM tiles (tileBase, reserved by Init) instead of at
 * the tile cursor, and when it fits (or the definition is of type $20) register it like ACTOR
 * Activate: flags $4800 cleared, its timer started, attacker and body hit lists by flags6, its
 * Draw method in the draw list.
 *
 * self: the boss.
 * The sprite flags and frame are kept across Spr_Init. ACTOR Activate's floor segments and
 * attached objects are left out; the read of the first attached object stayed. */
#include "xmen2.h"

void MagnetoBoss_Activate(register MagnetoBoss *self)
{
	register char flags = self->spr.flags;
	register u8 frame = self->spr.frame;
	short tiles;

	tiles = Spr_Init(&self->spr, 1, self->spr.def, -1, self->spr.flags & 3, self->tileBase, 0);
	if (tiles <= g_VramTileLimit || *(u8 *)self->spr.def == 0x20) {
		tiles = self->spr.firstChild;
		self->spr.flags = flags;
		self->spr.frame = frame;
		self->flags &= ~0x4800;
		if (self->timerObj)
			Timer_Start(self->timerObj);
		if (!(self->flags6 & 1)) {
			if (self->flags6 & 0x10)
				self->attackNode = HitList_Add(self, &g_HitListPlayerAtk);
			else
				self->attackNode = HitList_Add(self, &g_HitListEnemyAtk);
		}
		if (!(self->flags6 & 2) && !(self->flags6 & 0x10))
			self->bodyNode = HitList_Add(self, &g_HitListEnemyBody);
		self->drawNode = DrawList_Add(self->cls->method[5], self, self->spr.flags & 4, self->layer,
			self->flags6 & 8, 1);					/* Draw */
	}
}
