/* BroodBossWall_Update - BROODBOSSWALL Update (ICANDY slot +$12): a standing wall keeps a solid
 * block ($18 px wide, $C0 high, from x - 8 to x + $10 above y; made once) and moves it with its
 * own x speed; a smashed one (BroodBoss_StateHitWall) frees the block, throws 6 clone debris
 * bursts up the wall ($20 px apart from its foot), plays sound $35 and destroys itself (the
 * parent's slot +$1A).
 *
 * self: the wall. */
#include "xmen2.h"

#define g_CloneDebris	(*(PTR(DebrisPool) *)g_SceneVar3B1C)	/* scene variable: the clone-maker debris pool */

void BroodBossWall_Update(BroodBossWall *self)
{
	short px;
	short py;
	short i;

	if (self->smashed) {
		if (self->seg) {
			Seg_Free(self->seg);
			self->seg = 0;
		}
		px = self->x >> 8;
		py = self->y >> 8;
		for (i = 0; i < 6; i++, py -= 0x20)
			Debris_Spawn(px, py, g_CloneDebris, 3);
		Snd_Stop(0x35);
		Snd_Play(0x35);
		(*CLASS(Tbl_BroodBossWall_vtable)->parent->method[5])(self);	/* ICANDY Destroy */
	} else {
		px = self->x >> 8;
		py = self->y >> 8;
		if (!self->seg)
			self->seg = Seg_Create(0x14, 0, px - 8, py - 0xC0, px + 0x10, py);
		self->x += self->vx;
		if (self->seg) {
			self->seg->x1 += self->vx >> 8;
			self->seg->x2 += self->vx >> 8;
		}
	}
}
