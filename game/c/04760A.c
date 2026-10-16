/* BaniMazaA2_SetPaletteStep - the room's colours after another hittable bomb was destroyed
 * (ApocBomb_Explode): colours 1-9 of line 0 from row (bombs destroyed - 1) of
 * Pal_BaniMazaA2Line0Steps (10 colours a row), uploaded at the next V-blank; on odd counts the
 * colour-$14 cycle gets one frame faster (g_A2CyclePeriod, not below 1). */
#include "xmen2.h"

#define g_ApocBombsDestroyed	(*(s16 *)g_SceneVar3B0C)	/* scene variable: hittable bombs destroyed (max 12) */
#define g_A2CyclePeriod	(*(s16 *)g_SceneVar3B38)	/* scene variable: frames between colour-$14 steps (6, faster with the bombs) */

typedef s16 PalRow[10];

void BaniMazaA2_SetPaletteStep(void)
{
	register PalRow *row = (PalRow *)CODEPTR(Pal_BaniMazaA2Line0Steps);
	register long i;

	row += g_ApocBombsDestroyed - 1;
	i = 9;
	do {
		g_PalBase[i] = (*row)[i];
		--i;
	} while (i);
	g_PalBaseDirty = 1;
	if (g_ApocBombsDestroyed & 1 && g_A2CyclePeriod > 1)
		--g_A2CyclePeriod;
}
