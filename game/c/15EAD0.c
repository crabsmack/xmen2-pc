/* Actor_Nop66 - ACTOR class slot +$66: does nothing.
 *
 * self: the object. Never runs in the corpus.
 * The original copies self into a register and returns (whatever used it was compiled out):
 * the unused register local reproduces that. */
#include "xmen2.h"

void Actor_Nop66(Actor *self)
{
	register Actor *p = self;
}
