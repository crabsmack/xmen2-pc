/* SavageBio_ActReloadArt10 - SAVAGEBIGBIO script action: drop the tentacle and play the
 * animation Tbl_SavageBioAnim15; when it ends, reload the sprite tiles from frame $10
 * (SavageBio_StateReloadArt10).
 *
 * self: the boss (a step of its AI script; never runs in the corpus).
 * Returns self->flags & 8. */
#include "xmen2.h"

short SavageBio_ActReloadArt10(SavageBigBio *self)
{
	self->tentacle = 0;
	self->tentacleLen = 0;
	Obj_SetAnim(self, CODEPTR(Tbl_SavageBioAnim15));
	SET_STATE_WAIT(self, SavageBio_StateReloadArt10, 0xFF);
	return self->flags & 8;
}
