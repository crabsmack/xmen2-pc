/* CloneA2_LoadLevel - load Vertical Corridor 1 (CloneA2): its level data through Level_Load (FG
 * map and tiles Art_CloneA2Nem, no BG map, FG floor segments Lvl_CloneA2Segs, no BG segments,
 * flags 0 0 1 0, class-init argument $14, no music (-1), grid shifts 8 and 8, class list
 * Tbl_CloneA2ClassList); then palette colour 0 = $200, palette lines 2 (Pal_CloneA2Line2) and
 * 0 (Pal_CloneA2Line0), the HUD on, one-player stepping. */
#include "xmen2.h"

void CloneA2_LoadLevel(void)
{
	Level_Load(CODEPTR(Art_CloneA2Nem), CODEPTR(Art_CloneA2Nem), 0, 0, CODEPTR(Lvl_CloneA2Segs), 0,
		0, 0, 1, 0, 0x14, -1, 8, 8, 0, CODEPTR(Tbl_CloneA2ClassList));
	g_PalBase[0] = 0x200;
	{
		register s16 *src = (s16 *)CODEPTR(Pal_CloneA2Line2);
		register s16 *dst = &g_PalBase[0x20];
		register long i;

		for (i = 0; i < 16; i++)
			*dst++ = *src++;
	}
	{
		register s16 *src = (s16 *)CODEPTR(Pal_CloneA2Line0);
		register s16 *dst = g_PalBase;
		register long i;

		for (i = 0; i < 16; i++)
			*dst++ = *src++;
	}
	g_HudOn = 1;
	g_TwoPlayerStep = 0;
}
