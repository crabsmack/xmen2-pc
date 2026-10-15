/* XPlayer_Activate - XPLAYER class slot +$3E: put the player in play.
 *
 * self: the player.
 * Clears flags $4C00 (hidden among them), starts its timer, joins the hit lists (the enemies'
 * while it is hostile: playerFlags $1000000) and, when not in it yet, the draw list with its
 * Draw method. Marks the HUD for redraw. */
#include "xmen2.h"

void XPlayer_Activate(XPlayer *self)
{
	self->flags &= ~0x4C00;
	Timer_Start(self->timerObj);
	if (self->playerFlags & 0x1000000) {
		self->attackNode = HitList_Add(self, &g_HitListEnemyAtk);
		self->bodyNode = HitList_Add(self, &g_HitListEnemyBody);
	} else {
		self->attackNode = HitList_Add(self, &g_HitListPlayerAtk);
		self->bodyNode = HitList_Add(self, &g_HitListPlayerBody);
	}
	if (self->drawNode < 0)
		self->drawNode = DrawList_Add(self->cls->method[5], self, self->spr.flags & 4, self->layer,
			self->flags6 & 8, 1);					/* Draw */
	g_HudDirty = 1;
}
