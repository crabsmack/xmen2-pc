/* SavageBio_ActReloadArt14 - SAVAGEBIGBIO script action: drop the tentacle and play the
 * animation Tbl_SavageBioAnim2; when it ends, reload the sprite tiles from frame $14
 * (SavageBio_StateReloadArt14).
 *
 * self: the boss (a step of its AI script).
 * Returns self->flags & 8. */
#include "xmen2.h"

short SavageBio_ActReloadArt14(SavageBigBio *self)
{
	self->tentacle = 0;
	self->tentacleLen = 0;
	Obj_SetAnim(self, CODEPTR(Tbl_SavageBioAnim2));
	SET_STATE_WAIT(self, SavageBio_StateReloadArt14, 0xFF);
	return self->flags & 8;
}
