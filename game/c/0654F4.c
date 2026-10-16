/* Cerebro_LoadSpriteTiles - register the Cerebro screens' sprite set (Tbl_CerebroVdp: the portrait
 * frames, cursor boxes and lives digits; palette 1, SprDef_Add) and keep its first VRAM tile in
 * g_SceneVar3B10 (PlayerSelect_DrawFrame, PlayerSelect_Run). */
#include "xmen2.h"

#define g_CerebroSprTiles	(*(s16 *)g_SceneVar3B10)	/* scene variable: first VRAM tile of the Cerebro sprite tiles */

void Cerebro_LoadSpriteTiles(void)
{
	register short def;
	register SprInst *s;

	def = SprDef_Add(CODEPTR(Tbl_CerebroVdp), 1);
	s = &g_SpriteDefs[def];
	g_CerebroSprTiles = s->tileBase;
}
