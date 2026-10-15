/* SavageBio_StateReloadArt10 - SAVAGEBIGBIO state: reload the tentacle tiles (Art_SavageBioSpr
 * from frame $10) into the class's VRAM slot, then go on with the script now.
 *
 * self: the boss (after SavageBio_ActReloadArt10's animation; never runs in the corpus).
 * Same as SavageBio_StateReloadArt14 but for the frame. Returns nothing in particular. */
#include "xmen2.h"

short SavageBio_StateReloadArt10(SavageBigBio *self)
{
	SprInst tmp;

	Spr_Init(&tmp, 1, CODEPTR(Art_SavageBioSpr), 0x10, 3,
		g_SpriteDefs[g_SavageBioSprDef].tileBase + 0x14, 1);
	AI_NEXT_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
