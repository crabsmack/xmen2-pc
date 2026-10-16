/* CloneA1_InitSprites - the Factory Floor's three scene sprites (g_SceneSprites 0-2) from
 * Tbl_CloneA1Vdp, frames 0, 1, 2 (palettes 2, 2, 0) with their tiles uploaded at g_FgTilesEnd + 0,
 * $10, $20; and the FG block rows 1 and 2 get priority and palette 2 (Map_SetRowPriority,
 * Map_SetRowPalette on g_FgBlockDefs). */
#include "xmen2.h"

void CloneA1_InitSprites(void)
{
	CloneA1_NopStatic();
	Spr_Init(&g_SceneSprites[0], 1, CODEPTR(Tbl_CloneA1Vdp), 0, 2, g_FgTilesEnd, 1);
	Map_SetRowPriority(g_FgBlockDefs, g_FgBlockCount, 1);
	Map_SetRowPalette(g_FgBlockDefs, g_FgBlockCount, 1, 2);
	Spr_Init(&g_SceneSprites[1], 1, CODEPTR(Tbl_CloneA1Vdp), 1, 2, g_FgTilesEnd + 0x10, 1);
	Map_SetRowPriority(g_FgBlockDefs, g_FgBlockCount, 2);
	Map_SetRowPalette(g_FgBlockDefs, g_FgBlockCount, 2, 2);
	Spr_Init(&g_SceneSprites[2], 1, CODEPTR(Tbl_CloneA1Vdp), 2, 0, g_FgTilesEnd + 0x20, 1);
}
