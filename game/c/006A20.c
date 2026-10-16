/* Scene_AllocCommonVram - the sprite graphics every scene has, from the next free VRAM tile
 * (g_VramTileCursor, advanced past each; the first is g_FxTileBase): the preloaded definition
 * Tbl_BioNpcVdp (through a temporary instance, palette 1), then the hit spark (Art_Spr_182690,
 * g_HitSparkSpr) and the flux bomb (Art_Spr_14DB16, g_FluxBombSpr), palette 1, no frame shown. */
#include "xmen2.h"

void Scene_AllocCommonVram(void)
{
	SprInst tmp;

	g_FxTileBase = g_VramTileCursor;
	Spr_Init(&tmp, 1, CODEPTR(Tbl_BioNpcVdp), 0, 1, g_VramTileCursor, 0);
	g_VramTileCursor = Spr_PreloadToHeap(&tmp, g_VramTileCursor);
	g_VramTileCursor = Spr_Init(&g_HitSparkSpr, 1, CODEPTR(Art_Spr_182690), -1, 1, g_VramTileCursor, 0);
	g_VramTileCursor = Spr_Init(&g_FluxBombSpr, 1, CODEPTR(Art_Spr_14DB16), -1, 1, g_VramTileCursor, 0);
}
