/* BroodBoss_StateScream - BROODBOSS state after the scream: frame 0; the body rises back 1 px a
 * frame (2 in two-player step mode) on its y offset, clearing g_BroodBossFlags bit 6, until the
 * offset is 0; then the next AI-script step at once.
 *
 * self: the boss. Returns flags & 8 while it rises, nothing after. */
#include "xmen2.h"

#define g_BroodBossFlags	(*(s16 *)g_SceneVar3B0C)	/* scene variable: 1 fight on, 2 floor shock, $10 lower the body, $40 wall smashed */

short BroodBoss_StateScream(BroodBoss *self)
{
	register short step = 1;

	if (g_TwoPlayerStep)
		step <<= 1;
	(*self->cls->method[20])(self, 0);		/* +$56 SetFrame */
	if (self->offY > 0) {
		self->offY -= step;
		g_BroodBossFlags &= ~0x40;
		SET_STATE(self, BroodBoss_StateScream);
		return self->flags & 8;
	}
	self->offY = 0;
	AI_NEXT_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
