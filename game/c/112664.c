/* Deathbird_StateHurtFlash - DEATHBIRD after a hit: flash palette line 1's colours 1-4 between
 * its own and white-ish every 2 frames for 12 steps (g_SceneVar3B06), going on with the fly-off
 * once the hurt animation has ended (SetMoveMode 4); then its colours back and, with no health
 * left, fall (Deathbird_StateFalling), else hittable again (both hit lists) and fly off
 * (Deathbird_StateFlyOff).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

#define g_DeathbirdFlash	(*(s16 *)g_SceneVar3B06)	/* scene variable: hurt-flash steps left */

short Deathbird_StateHurtFlash(Deathbird *self)
{
	g_PalBaseDirty = 1;
	if (!self->anim)
		(*self->cls->method[27])(self, 4);	/* SetMoveMode */
	--g_DeathbirdFlash;
	if (g_DeathbirdFlash < 0) {
		g_PalBase[0x22] = 0x002;
		g_PalBase[0x23] = 0x022;
		g_PalBase[0x24] = 0x024;
		g_PalBase[0x21] = 0x000;
		if (self->health <= 0) {
			SET_STATE(self, (StateFn)Deathbird_StateFalling);
			return self->flags & 8;
		}
		self->flags6 &= ~2;
		self->bodyNode = HitList_Add(self, &g_HitListEnemyBody);
		self->attackNode = HitList_Add(self, &g_HitListEnemyAtk);
		SET_STATE(self, (StateFn)Deathbird_StateFlyOff);
		return self->flags & 8;
	}
	if (g_PalBase[0x22] == 0x462) {
		g_PalBase[0x22] = 0x002;
		g_PalBase[0x23] = 0x022;
		g_PalBase[0x24] = 0x024;
		g_PalBase[0x21] = 0x000;
	} else {
		g_PalBase[0x22] = 0x462;
		g_PalBase[0x23] = 0x484;
		g_PalBase[0x24] = 0x886;
		g_PalBase[0x21] = 0xCCC;
	}
	SET_STATE_WAIT(self, (StateFn)Deathbird_StateHurtFlash, 2);
	return self->flags & 8;
}
