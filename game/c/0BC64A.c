/* SavageBio_StateCrossSurface - SAVAGEBIGBIO state: hide, and send the BIOBALLS companion
 * across to the other surface in its place (SavageBio_StateWaitCrossing waits for it).
 *
 * self: the boss (after SavageBio_ActCrossIfUp; never runs in the corpus).
 * The companion takes the boss's position and flips, the formation
 * Tbl_SavageBioBallsFormation and the AI script Tbl_SavageBioAiScript from its first step.
 * Returns self->flags & 8. */
#include "xmen2.h"

short SavageBio_StateCrossSurface(SavageBigBio *self)
{
	register BioBalls *balls = self->balls;

	if (balls) {
		balls->x = self->x;
		balls->y = self->y;
		if (!(self->spr.flags & 8))
			balls->spr.flags &= ~8;
		else
			balls->spr.flags |= 8;
		if (!(self->spr.flags & 0x10))
			balls->spr.flags &= ~0x10;
		else
			balls->spr.flags |= 0x10;
		BioBalls_SetFormation(balls, CODEPTR(Tbl_SavageBioBallsFormation));
		AiScript_Set(&balls->ai, CODEPTR(Tbl_SavageBioAiScript));
		AI_FIRST_STEP(balls, balls->ai, 0);
	}
	self->anim = 0;
	(*self->cls->method[20])(self, 0xFF);		/* SetFrame: hide */
	SET_STATE(self, SavageBio_StateWaitCrossing);
	return self->flags & 8;
}
