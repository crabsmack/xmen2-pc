/* Scene_PostLoadVram - the scene-load post hook (g_ScenePostLoadHook, set by Game_SceneLoop;
 * Level_Load runs it last): the common sprite graphics (Scene_AllocCommonVram), palette lines 1 and
 * 3 the default ones (Pal_SceneDefaultLine1 / 3), then each player's VRAM tiles (g_PlayerTiles each:
 * g_P1VramTile, g_P2VramTile) and the magneto shots reset. */
#include "xmen2.h"

void Scene_PostLoadVram(void)
{
	SprInst unused;			/* reserved in the frame, never used */
	register s16 *b, *a;
	register long i;

	Scene_AllocCommonVram();
	a = (s16 *)CODEPTR(Pal_SceneDefaultLine1);
	b = &g_PalBase[16];
	for (i = 0; i < 16; i++)
		*b++ = *a++;
	b = (s16 *)CODEPTR(Pal_SceneDefaultLine3);
	a = &g_PalBase[48];
	for (i = 0; i < 16; i++)
		*a++ = *b++;
	g_P1VramTile = g_VramTileCursor;
	g_P2VramTile = g_P1VramTile + g_PlayerTiles;
	g_VramTileCursor = g_P2VramTile + g_PlayerTiles;
	MagnetoShots_Reset();
	if (0)
		(void)&unused;
}
