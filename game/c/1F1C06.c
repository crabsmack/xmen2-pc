/* XPlayer_Deactivate - XPLAYER class slot +$46: take a present, alive player out of play.
 *
 * self: the player.
 * Hidden (flags $4000), its frame no longer uploaded, detached (sprite links $FF), its timer
 * stopped, out of the hit lists (the enemies' while it is hostile: playerFlags $1000000) and
 * the draw list. Marks the HUD for redraw. */
#include "xmen2.h"

void XPlayer_Deactivate(XPlayer *self)
{
	if (IS_ALIVE(self)) {
		self->flags |= 0x4000;
		self->spr.flags &= ~0x20;
		self->spr.firstChild = 0xFF;
		self->spr.parent = 0xFF;
		Timer_Stop(self->timerObj);
		if (self->playerFlags & 0x1000000) {
			List_Unlink(self->attackNode, &g_HitListEnemyAtk);
			self->attackNode = 0;
			List_Unlink(self->bodyNode, &g_HitListEnemyBody);
			self->bodyNode = 0;
		} else {
			List_Unlink(self->attackNode, &g_HitListPlayerAtk);
			self->attackNode = 0;
			List_Unlink(self->bodyNode, &g_HitListPlayerBody);
			self->bodyNode = 0;
		}
		if (self->drawNode > -1) {
			DrawList_Remove(self->drawNode);
			self->drawNode = -1;
		}
		g_HudDirty = 1;
	}
}
