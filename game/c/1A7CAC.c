/* Nightcrawler_SelectSpriteDef - the sprite definition of Nightcrawler on the player-select
 * screen (Art_EvilNightcrawlerSpr, his first frame bank).
 *
 * Returns it; Nightcrawler_SelectFrame gives the frame. */
#include "xmen2.h"

void *Nightcrawler_SelectSpriteDef(void)
{
	return CODEPTR(Art_EvilNightcrawlerSpr);
}
