/* BanimazaDoor_StateStartRise - BANIMAZADOOR state: start rising: sound $BC, hittable again
 * (flags6 $4000 off, in the enemy body hit list), its floor segment solid (flag $40 off), look
 * 1, vy 1500 with no acceleration, solid; next BanimazaDoor_StateRising.
 *
 * self: the door. Returns 0. */
#include "xmen2.h"

short BanimazaDoor_StateStartRise(register BanimazaDoor *self)
{
	Snd_Stop(0xBC);
	Snd_Play(0xBC);
	self->flags6 &= ~0x4000;
	self->bodyNode = HitList_Add(self, &g_HitListEnemyBody);
	g_Segs[self->segIndex].flags &= ~0x40;
	self->look = 1;
	self->vy = 1500;
	self->ay = 0;
	self->solid = 1;
	self->state = BanimazaDoor_StateRising;
	return 0;
}
