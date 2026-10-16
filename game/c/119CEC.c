/* Synchronizer_Nop26 - SYNCHRONIZER slot +$26: does nothing.
 *
 * self: the synchronizer. Never runs in the corpus.
 * The original copies self into a register and returns (whatever used it was compiled out):
 * the unused register local reproduces that (as Actor_Nop66). */
#include "xmen2.h"

void Synchronizer_Nop26(Synchronizer *self)
{
	register Synchronizer *p = self;
}
