/* Spr_ReloadDefs - load a new set of sprite definitions into VRAM over the old one.
 *
 * load: the loader (calls SprDef_Add: BigAcolyte_LoadSprite, XClone_LoadResources...).
 * repack: non-zero to re-pack the objects' tiles even when the new set is no larger.
 * The definitions start again right after the players' tiles (g_PlayerTiles + g_P2VramTile, no
 * definitions). When the new set ends below the old cursor (and no repack), the cursor goes back
 * where it was. Else every live object but the players whose tiles started at or above the old
 * cursor gets new tiles at the cursor, which moves on by its tile count, and uploads its frame
 * again (Spr_InvalidateFrame). */
#include "xmen2.h"

void Spr_ReloadDefs(void (*load)(void), char repack)
{
	short old;
	short i;
	Actor *o;

	old = g_VramTileCursor;
	g_VramTileCursor = g_P2VramTile + g_PlayerTiles;
	g_SpriteDefCount = 0;
	(*load)();
	if (old >= g_VramTileCursor && !repack) {
		g_VramTileCursor = old;
		return;
	}
	for (i = 0, o = g_ObjTable[0]; i < g_ObjCount; o = g_ObjTable[++i]) {
		if (IS_ALIVE(o) && o != (Actor *)&g_Player1 && o != (Actor *)&g_Player2
		    && old <= o->spr.tileBase) {
			o->spr.tileBase = g_VramTileCursor;
			g_VramTileCursor += o->spr.tileCount;
			Spr_InvalidateFrame(&o->spr);
		}
	}
}
