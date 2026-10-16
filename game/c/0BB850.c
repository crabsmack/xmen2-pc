/* SavageBio_ActAnimB - SAVAGEBIGBIO script action: switch to the animation Tbl_SavageBioAnim
 * and go on with the script on the next update.
 *
 * self: the boss (a step of its AI script). */
#include "xmen2.h"

void SavageBio_ActAnimB(SavageBigBio *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_SavageBioAnim));
	AI_NEXT_STEP(self, self->ai, 0);
}
