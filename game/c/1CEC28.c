/* Magneto_SelectSpriteDef - the sprite definition of Magneto on the player-select screen
 * (Art_EvilMagnetoSpr, his first frame bank: Magneto_FrameBank).
 *
 * Returns it; Magneto_SelectFrame gives the frame. */
#include "xmen2.h"

void *Magneto_SelectSpriteDef(void)
{
	return CODEPTR(Art_EvilMagnetoSpr);
}
