/* Deathbird_StateHurt - DEATHBIRD's hurt state (+$60, after a hit): leave both hit lists
 * (flags6 bit 1: unhittable) and flash for 12 steps (Deathbird_StateHurtFlash); with no health
 * left stop the burst, death sound $63 and fall (SetMoveMode 1), else hurt sound $AA and recoil
 * (SetMoveMode 10).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

#define g_DeathbirdFlash	(*(s16 *)g_SceneVar3B06)	/* scene variable: hurt-flash steps left */

short Deathbird_StateHurt(Deathbird *self)
{
	self->flags6 |= 2;
	List_Unlink(self->bodyNode, &g_HitListEnemyBody);
	self->bodyNode = 0;
	List_Unlink(self->attackNode, &g_HitListEnemyAtk);
	self->attackNode = 0;
	g_DeathbirdFlash = 12;
	if (self->health <= 0) {
		Timer_Stop(self->burstTimer);
		Snd_Stop(0x63);
		Snd_Play(0x63);
		(*self->cls->method[27])(self, 1);	/* SetMoveMode */
	} else {
		Snd_Stop(0xAA);
		Snd_Play(0xAA);
		(*self->cls->method[27])(self, 10);	/* SetMoveMode */
	}
	SET_STATE(self, (StateFn)Deathbird_StateHurtFlash);
	return self->flags & 8;
}
