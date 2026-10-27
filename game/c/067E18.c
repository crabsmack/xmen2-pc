/* BoxInputActor_Draw - BOXINPUTACTOR class slot +$1A (the parent's isn't called): the player's
 * cursor box (PlayerSelect_DrawFrame of its slot's frameSpr, palette 3, in front) on the portrait
 * frame of its character, unless it has none.
 *
 * self: the cursor. */
#include "xmen2.h"

void BoxInputActor_Draw(BoxInputActor *self)
{
	register BigSprite *slot = self->slot;

	if (slot->charIndex != -1)
#if defined(XMEN2_NATIVE)
		if (g_opt.players > 2 && slot->player >= 2)	/* phase 7: the player's own box, digit on its side */
			P34_DrawBox(slot);
		else
#endif
		PlayerSelect_DrawFrame(slot->frameSpr, slot->cursorX, slot->cursorY, 3, 1);
}
