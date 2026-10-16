/* SavageBioBalls_ActOpen - BIOBALLS script action (the SAVAGEMEDIUMBIO's burst): the balls go
 * along Tbl_SavageBioBallsFormation from where they are (BioBalls_StartPath, no flags or offset),
 * then wait for the path (BioBalls_WaitPathDone from the next update).
 *
 * self: the balls (a step of their AI script). Returns self->flags & 8. */
#include "xmen2.h"

short SavageBioBalls_ActOpen(register BioBalls *self)
{
	BioBalls_StartPath(self, CODEPTR(Tbl_SavageBioBallsFormation), 0, 0, 0);
	SET_STATE(self, BioBalls_WaitPathDone);
	return self->flags & 8;
}
