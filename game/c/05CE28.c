/* Tusk_Nop66 - class slot +$66 of TUSK: does nothing.
 *
 * self: the object. Never runs in the corpus.
 * The original copies self into a register and returns (whatever used it was compiled out):
 * the unused register local reproduces that. */
#include "xmen2.h"

void Tusk_Nop66(Tusk *self)
{
	register Tusk *p = self;
}
