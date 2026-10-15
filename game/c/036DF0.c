/* MagnetoBoss_StateBurstIn - MAGNETOBOSS bursts through the broken wall: the camera is free
 * again (zone 0 cleared), it flies right (vx $500, vy -$48) firing a globe down-right
 * (direction 8), becomes hittable (flags6 bit 1 off, enemy body hit list) and starts its
 * wobble (bossFlags bit 0); then the chase (MagnetoBoss_Zone0Setup).
 *
 * self: the boss.
 * Returns self->flags & 8. */
#include "xmen2.h"

short MagnetoBoss_StateBurstIn(register MagnetoBoss *self)
{
	AvalonA4_ClearCameraZone(0);
	self->vx = 0x500;
	self->vy = -0x48;
	self->shotDir = 8;
	Magneto_SpawnShot(self->x, self->y, self->shotDir, 0, 0, self->spr.flags & 4 ? 1 : 0, 1,
		self->shotTiles);
	self->flags6 &= ~2;
	self->bodyNode = HitList_Add(self, &g_HitListEnemyBody);
	self->bossFlags |= 1;
	SET_STATE(self, MagnetoBoss_Zone0Setup);
	return self->flags & 8;
}
