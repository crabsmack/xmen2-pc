/* Cyclops_SelectSpriteDef - the sprite definition of Cyclops on the player-select screen
 * (Art_CyclopsSpr, his body sprite).
 *
 * Returns it; Cyclops_SelectFrame gives the frame. */
#include "xmen2.h"

void *Cyclops_SelectSpriteDef(void)
{
	return CODEPTR(Art_CyclopsSpr);
}
