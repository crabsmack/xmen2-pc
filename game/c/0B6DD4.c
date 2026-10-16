/* BrainChild_StateLand - BRAINCHILD state: landed: frame 2 (slot +$56), stop, y back at py, it
 * becomes hittable (flags6 bits 2 and 1 off: in the enemy body and attack hit lists,
 * HitList_Add), palette line 3 from the 16 words at Art_BabyBioSpr + 4 (uploaded at the next
 * palette update), then the next step of its AI script in 10 frames.
 *
 * self: the object. */
#include "xmen2.h"

void BrainChild_StateLand(register BrainChild *self)
{
	s16 *src;
	register s16 *dst;
	register long i;

	(*self->cls->method[20])(self, 2);	/* +$56 SetFrame */
	STOP_MOVING(self);
	self->y = self->py << 8;
	self->flags6 &= ~2;
	self->bodyNode = HitList_Add(self, &g_HitListEnemyBody);
	self->flags6 &= ~1;
	self->attackNode = HitList_Add(self, &g_HitListEnemyAtk);
	src = (s16 *)CODEPTR(Art_BabyBioSpr) + 2;
	dst = &g_PalBase[48];
	for (i = 0; i < 16; i++)
		*dst++ = *src++;
	AI_NEXT_STEP(self, self->ai, 10);
}
