/* Psylocke_SelectSpriteDef - the sprite definition of Psylocke on the player-select screen
 * (Art_EvilPsylockeSpr, her first sprite bank: Psylocke_SetFrame).
 *
 * Returns it; Psylocke_SelectFrame gives the frame. */
#include "xmen2.h"

void *Psylocke_SelectSpriteDef(void)
{
	return CODEPTR(Art_EvilPsylockeSpr);
}
