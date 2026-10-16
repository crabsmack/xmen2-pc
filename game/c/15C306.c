/* Pause_LoadSprite - load the tiles of the PAUSE text sprite (Art_PauseSpr) into VRAM at tile
 * $794 (Pause_Check, on pausing); the sprite instance itself is a throwaway local. */
#include "xmen2.h"

void Pause_LoadSprite(void)
{
	SprInst spr;

	Spr_Init(&spr, 1, CODEPTR(Art_PauseSpr), 0, 1, 0x794, 1);
}
