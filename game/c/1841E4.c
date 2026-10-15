/* Beast_SelectSpriteDef - the sprite definition of Beast on the player-select screen
 * (Art_BeastSpr, his first sprite bank: Beast_SetFrame).
 *
 * Returns it; Beast_SelectFrame gives the frame. */
#include "xmen2.h"

void *Beast_SelectSpriteDef(void)
{
	return CODEPTR(Art_BeastSpr);
}
