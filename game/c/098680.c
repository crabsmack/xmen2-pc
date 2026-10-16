/* EvilNightcrawler_StateLand - EVILNIGHTCRAWLER state at the end of a teleport's fall: the crouch
 * pose of Nightcrawler's character record, then the AI script goes on in 6 frames.
 *
 * self: the clone (never runs in the corpus). */
#include "xmen2.h"

void EvilNightcrawler_StateLand(EvilNightcrawler *self)
{
	Obj_ChangeAnim(self, CHAR_PART(self->charRec, crouchAnim));
	AI_NEXT_STEP(self, self->ai, 6);
}
