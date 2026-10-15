/* SavageBio_ActToCentre - SAVAGEBIGBIO script action: play the animation Tbl_SavageBioAnim and
 * walk to the middle of the view (SavageBio_StateToCentre).
 *
 * self: the boss (a step of its AI script).
 * Returns self->flags & 8. */
#include "xmen2.h"

short SavageBio_ActToCentre(SavageBigBio *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_SavageBioAnim));
	SET_STATE(self, SavageBio_StateToCentre);
	return self->flags & 8;
}
