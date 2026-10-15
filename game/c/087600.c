/* BroodBoss_StateHitWall - BROODBOSS state: smash the wall (it blows itself up, BroodBossWall_Update;
 * the boss forgets it), frame $F, debris, g_BroodBossFlags bit 6, the body 10 px lower; then
 * the recoil (BroodBoss_StateRecoil).
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

#define g_BroodBossFlags	(*(s16 *)g_SceneVar3B0C)	/* scene variable: 1 fight on, 2 floor shock, $10 lower the body, $40 wall smashed */

short BroodBoss_StateHitWall(BroodBoss *self)
{
	if (self->wall) {
		self->wall->smashed = 1;
		self->wall = 0;
	}
	(*self->cls->method[20])(self, 0xF);		/* +$56 SetFrame */
	BroodBoss_SpawnDebris(self);
	g_BroodBossFlags |= 0x40;
	self->offY += 10;
	SET_STATE(self, BroodBoss_StateRecoil);
	return self->flags & 8;
}
