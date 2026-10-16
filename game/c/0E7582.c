/* SentinelsA1_InitZapLights - set up the Coolant Core's five zap lights: scene sprites 0-4 for
 * Art_ZapLightSpr (frame 7, palette 2), 16 VRAM tiles apart from g_FgTilesEnd, uploaded now
 * (Spr_Init); FG block rows 1-5 to palette 1 (Map_SetRowPalette); all five idle (frame -1:
 * CoreShields_FireZap starts the chain, SentinelsA1_StepZapLights runs it). */
#include "xmen2.h"

#define g_ZapLight0	(*(s16 *)g_SceneVar3B02)	/* scene variable: frame of zap light 0 (-1 idle) */
#define g_ZapLight1	(*(s16 *)g_SceneVar3B04)	/* scene variable: frame of zap light 1 */
#define g_ZapLight2	(*(s16 *)g_SceneVar3B06)	/* scene variable: frame of zap light 2 */
#define g_ZapLight3	(*(s16 *)g_SceneVar3B08)	/* scene variable: frame of zap light 3 */
#define g_ZapLight4	(*(s16 *)g_SceneVar3B0A)	/* scene variable: frame of zap light 4 */

void SentinelsA1_InitZapLights(void)
{
	register void *def = CODEPTR(Art_ZapLightSpr);
	register short tile = g_FgTilesEnd;

	Spr_Init(&g_SceneSprites[0], 1, def, 7, 2, tile, 1);
	Map_SetRowPalette(g_FgBlockDefs, g_FgBlockCount, 1, 1);
	tile += 0x10;
	Spr_Init(&g_SceneSprites[1], 1, def, 7, 2, tile, 1);
	Map_SetRowPalette(g_FgBlockDefs, g_FgBlockCount, 2, 1);
	tile += 0x10;
	Spr_Init(&g_SceneSprites[2], 1, def, 7, 2, tile, 1);
	Map_SetRowPalette(g_FgBlockDefs, g_FgBlockCount, 3, 1);
	tile += 0x10;
	Spr_Init(&g_SceneSprites[3], 1, def, 7, 2, tile, 1);
	Map_SetRowPalette(g_FgBlockDefs, g_FgBlockCount, 4, 1);
	tile += 0x10;
	Spr_Init(&g_SceneSprites[4], 1, def, 7, 2, tile, 1);
	Map_SetRowPalette(g_FgBlockDefs, g_FgBlockCount, 5, 1);
	g_ZapLight0 = -1;
	g_ZapLight1 = -1;
	g_ZapLight2 = -1;
	g_ZapLight3 = -1;
	g_ZapLight4 = -1;
}
