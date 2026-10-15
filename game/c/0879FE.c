/* BroodBoss_StateHurt - BROODBOSS hurt state (+$60, after a hit): sound $37, unhittable (flags6
 * bit 1, out of the enemy body hit list), the claw stopped; palette line 2 saved to
 * g_BroodBossPalSave and colours 2-15 turned to the hit flash (unless they already are: colour
 * 2 = $424); at 0 health BroodBoss_Defeat. Hit flash 30 frames, the hurt animation
 * (Tbl_BroodBossAnim6), then BroodBoss_StateHurtEnd in 5 frames.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_StateHurt(BroodBoss *self)
{
	register s16 *save = g_BroodBossPalSave;
	register s16 *pal = &g_PalBase[32];
	register char i;

	self->flags6 |= 2;
	Snd_Stop(0x37);
	Snd_Play(0x37);
	List_Unlink(self->bodyNode, &g_HitListEnemyBody);
	self->bodyNode = 0;
	if (self->claw) {
		self->claw->vx = 0;
		self->claw->mode = 0;
		self->claw->motion = 0;
	}
	if (g_PalBase[34] != 0x424) {
		for (i = 0; i < 16; i++)
			*save++ = *pal++;
		pal = &g_PalBase[34];
		*pal++ = 0x424;
		*pal++ = 0x646;
		*pal++ = 0x848;
		*pal++ = 0xA6A;
		*pal++ = 0xC8C;
		*pal++ = 0xEAE;
		*pal++ = 0xECE;
		*pal++ = 0xEEE;
		*pal++ = 0x464;
		*pal++ = 0x486;
		*pal++ = 0x6CA;
		*pal++ = 0x8EE;
		*pal++ = 0xE6E;
		*pal++ = 0x426;
		g_PalBaseDirty = 1;
	}
	if (self->health <= 0)
		BroodBoss_Defeat(self);
	self->hitCooldown = 0x1E;
	Obj_SetAnim(self, CODEPTR(Tbl_BroodBossAnim6));
	SET_STATE_WAIT(self, BroodBoss_StateHurtEnd, 5);
	return self->flags & 8;
}
