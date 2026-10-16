/* YoyoAcolyte_StateYoyoOut - YOYOACOLYTE state: wait for the yoyo; once it is caught
 * (yoyoFlags bit 1): not out any more, frame 8, stop sound $DB, unhittable again (its body node
 * out of g_HitListEnemyBody, flags6 bit 1), and the next script step on the next update.
 *
 * self: the object. */
#include "xmen2.h"

void YoyoAcolyte_StateYoyoOut(register YoyoAcolyte *self)
{
	if (self->yoyoFlags & 2) {
		self->yoyoFlags &= ~3;
		(*self->cls->method[20])(self, 8);		/* +$56 SetFrame */
		Snd_Stop(0xDB);
		List_Unlink(self->bodyNode, &g_HitListEnemyBody);
		self->bodyNode = 0;
		self->flags6 |= 2;
		AI_NEXT_STEP(self, self->ai, 0);
	}
}
