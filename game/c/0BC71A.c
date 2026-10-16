/* SavageBioBalls_ActLeap - BIOBALLS script action (the SAVAGEBIGBIO's balls form): leap $E0 px
 * to the other surface along Tbl_SavageBioBallsFormation (BioBalls_StartPath; up when on the
 * floor, down when v-flipped on the ceiling), with sound $EF (restarted); then wait for the path
 * (BioBalls_WaitPathDone from the next update).
 *
 * self: the balls (a step of their AI script).
 * Path flags: 2 (mirror x) when h-flipped, 4 (mirror y) when not v-flipped. Returns
 * self->flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short SavageBioBalls_ActLeap(register BioBalls *self)
{
	register unsigned char flags = 0;
	register short dx = 0;
	register short dy = 0xE0;

	if (!(self->spr.flags & 8))
		dy = -dy;
	if (self->spr.flags & 0x10)
		flags |= 2;
	if (!(self->spr.flags & 8))
		flags |= 4;
	BioBalls_StartPath(self, CODEPTR(Tbl_SavageBioBallsFormation), flags, dx, dy);
	Snd_Stop(0xEF);
	Snd_Play(0xEF);
	SET_STATE(self, BioBalls_WaitPathDone);
	return self->flags & 8;
}
