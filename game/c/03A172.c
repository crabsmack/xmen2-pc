/* BanimazaDoor_Shatter - BANIMAZADOOR state after its last hit (BanimazaDoor_TakeDamage): sound
 * $35, not hittable (flags6 $4000, out of the enemy body hit list), its floor segment open (flag
 * $40), the shards thrown from the hit point, away from the attacker; next
 * BanimazaDoor_StateShattered.
 *
 * self: the door. Returns 0. */
#include "xmen2.h"

short BanimazaDoor_Shatter(register BanimazaDoor *self)
{
	register unsigned char right = self->attacker->x > self->x;

	Snd_Stop(0x35);
	Snd_Play(0x35);
	self->flags6 |= 0x4000;
	List_Unlink(self->bodyNode, &g_HitListEnemyBody);
	self->bodyNode = 0;
	g_Segs[self->segIndex].flags |= 0x40;
	BanimazaDoor_InitDebris(self->shards, self->shardCount, self->hitX << 8, self->hitY << 8, right);
	self->state = BanimazaDoor_StateShattered;
	return 0;
}
