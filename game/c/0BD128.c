/* SavageMediumBio_StateBurst - SAVAGEMEDIUMBIO state (after its death, SavageMediumBio_OnDeath):
 * burst into a BIOBALLS at its position, which gathers into the burst formation
 * (Tbl_SavageMediumBioBurstFormation) with sound $F2 and runs the script that makes it the
 * SAVAGEBIGBIO (Tbl_SavageMediumBio_Script4: SavageBioBalls_* steps) from its first step now;
 * then it removes itself (slot +$52).
 *
 * self: the object. Returns nothing in particular.
 * (The do/while (0), as a macro would be: its loop end keeps the last pop, and the loop makes
 * MPW C flush the pending pops before each branch, as the ROM.) */
#include "xmen2.h"

short SavageMediumBio_StateBurst(SavageMediumBio *self)
{
	register BioBalls *balls;

	if ((balls = (BioBalls *)(*CLASS(Tbl_BioBalls_vtable)->method[2])(0, CLASS(Tbl_BioBalls_vtable),
	    self->x, self->y, 0))) {	/* New */
		BioBalls_SetFormation(balls, CODEPTR(Tbl_SavageMediumBioBurstFormation));
		Snd_Stop(0xF2);
		Snd_Play(0xF2);
		AiScript_Set(&balls->ai, CODEPTR(Tbl_SavageMediumBio_Script4));
		AI_FIRST_STEP(balls, balls->ai, 0);
	}
	do {
		(*self->cls->method[19])(self);		/* +$52 Kill */
	} while (0);
	NO_RETURN_VALUE
}
