/* Wolverine_SelectSpriteDef - the sprite definition of Wolverine on the player-select screen
 * (Art_EvilWolverineSpr, his first sprite bank: Wolverine_SetFrame).
 *
 * Returns it; Wolverine_SelectFrame gives the frame. */
#include "xmen2.h"

void *Wolverine_SelectSpriteDef(void)
{
	return CODEPTR(Art_EvilWolverineSpr);
}
