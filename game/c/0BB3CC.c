/* SavageBio_StateReloadArt14 - SAVAGEBIGBIO state: reload the tentacle tiles (Art_SavageBioSpr
 * from frame $14) into the class's VRAM slot, then go on with the script on the next update.
 *
 * self: the boss (after SavageBio_ActReloadArt14's animation).
 * The tiles go $14 past the first tile of the sprite definition SavageA4 loaded
 * (g_SavageBioSprDef). Returns nothing in particular. */
#include "xmen2.h"

short SavageBio_StateReloadArt14(SavageBigBio *self)
{
	SprInst tmp;

	Spr_Init(&tmp, 1, CODEPTR(Art_SavageBioSpr), 0x14, 3,
		g_SpriteDefs[g_SavageBioSprDef].tileBase + 0x14, 1);
	AI_NEXT_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
