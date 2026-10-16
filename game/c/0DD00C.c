/* Blast_Latch - BLAST state after it hit something (Blast_Attack): out of the enemy attacker hit
 * list (List_Unlink), the first zap at once (countdown 0), $1E zaps to go, sound $D5 (restarted);
 * Blast_StateZap next frame.
 *
 * self: the blast. Returns self->flags & 8. */
#include "xmen2.h"

short Blast_Latch(register Blast *self)
{
	List_Unlink(self->attackNode, &g_HitListEnemyAtk);
	self->attackNode = 0;
	self->countdown = 0;
	self->zaps = 0x1E;
	Snd_Stop(0xD5);
	Snd_Play(0xD5);
	SET_STATE(self, Blast_StateZap);
	return self->flags & 8;
}
