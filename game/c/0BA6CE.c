/* SavageBigBio_TakeDamage - SAVAGEBIGBIO TakeDamage (class slot +$26): unless it takes no damage
 * (bioFlags bit 0), it loses the damage, can't be hit for 20 frames and flashes 20 frames
 * (flags6 bit $40). At 0 health it stops, dies (SavageBio_StateDying from the next update),
 * leaves both enemy hit lists and can't be hit nor hit (flags6 bits 1, 0); else sound $D8
 * (restarted). Its health then goes to g_A4BossHealth (the SavageA4 frame hook's palette stages).
 *
 * self: the boss.  attacker: what hit it (unused).  type: hit type (unused).
 * damage: health to lose.  hx, hy: the hit point (unused). */
#include "xmen2.h"

#define g_A4BossHealth		(*(s16 *)g_SceneVar3B02)	/* scene variable: SAVAGEBIGBIO's health (SavageBigBio_TakeDamage) */

void SavageBigBio_TakeDamage(register SavageBigBio *self, Actor *attacker, char type, char damage,
	short hx, short hy)
{
	if (self->bioFlags & 1)
		return;
	self->health -= damage;
	self->hitCooldown = 20;
	if (self->health <= 0) {
		STOP_MOVING(self);
		self->state = SavageBio_StateDying;
		self->flags6 |= 0x40;
		self->flashTimer = 20;
		List_Unlink(self->attackNode, &g_HitListEnemyAtk);
		self->attackNode = 0;
		List_Unlink(self->bodyNode, &g_HitListEnemyBody);
		self->bodyNode = 0;
		self->flags6 |= 2;
		self->flags6 |= 1;
	} else {
		self->flags6 |= 0x40;
		Snd_Stop(0xD8);
		Snd_Play(0xD8);
		self->flashTimer = 20;
	}
	g_A4BossHealth = self->health;
}
