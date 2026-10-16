/* BroodBoss_StateAfterBomb - BROODBOSS state after either bomb drop: hold frame 5, next AI-script
 * step on the next update.
 *
 * self: the boss. */
#include "xmen2.h"

void BroodBoss_StateAfterBomb(BroodBoss *self)
{
	(*self->cls->method[20])(self, 5);		/* +$56 SetFrame */
	AI_NEXT_STEP(self, self->ai, 0);
}
