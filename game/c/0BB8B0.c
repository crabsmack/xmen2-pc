/* SavageBio_ActAnimC - SAVAGEBIGBIO script action: switch to the animation Tbl_SavageBioAnim12
 * and go on with the script on the next update.
 *
 * self: the boss (a step of its AI script; never runs in the corpus). */
#include "xmen2.h"

void SavageBio_ActAnimC(SavageBigBio *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_SavageBioAnim12));
	AI_NEXT_STEP(self, self->ai, 0);
}
