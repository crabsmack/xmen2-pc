/* SavageBio_ActAnimA - SAVAGEBIGBIO script action: switch to the animation Tbl_SavageBioAnim11
 * and go on with the script now.
 *
 * self: the boss (a step of its AI script; never runs in the corpus). */
#include "xmen2.h"

void SavageBio_ActAnimA(SavageBigBio *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_SavageBioAnim11));
	AI_NEXT_STEP(self, self->ai, 0);
}
