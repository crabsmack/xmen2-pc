/* BigAcolyte_LoadSprite - sprite-reload callback of the BIGACOLYTE debris pool (registered with
 * Spr_ReloadDefs by BigAcolyte_ActAppear): register the debris sprite definition
 * (Art_BigAcolyteSpr2, palette 0) and give the pool its slot. */
#include "xmen2.h"

void BigAcolyte_LoadSprite(void)
{
	g_BigAcolyteDebris->sprDef = SprDef_Add(CODEPTR(Art_BigAcolyteSpr2), 0);
}
