/* Gambit_SelectSpriteDef - the sprite definition of Gambit on the player-select screen
 * (Art_EvilGambitSpr, his first sprite bank: Gambit_SetFrame).
 *
 * Returns it; Gambit_SelectFrame gives the frame. */
#include "xmen2.h"

void *Gambit_SelectSpriteDef(void)
{
	return CODEPTR(Art_EvilGambitSpr);
}
