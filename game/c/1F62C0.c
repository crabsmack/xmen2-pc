/* SprDef_Add - add a sprite definition to the scene's preloaded ones (g_SpriteDefs): a sprite
 * instance of it in palette pal at the VRAM tile cursor, its frames loaded there now
 * (Spr_PreloadToHeap), the cursor moved past them.
 *
 * def: the sprite definition.  pal: the palette line.
 * Returns nothing as such: d0 is left holding the entry's index (the count before it), which the
 * callers keep (Obj_SetSpriteDef copies the entry into an object later). */
#include "xmen2.h"

short SprDef_Add(void *def, short pal)
{
	Spr_Init(&g_SpriteDefs[g_SpriteDefCount], 1, def, 0, pal, g_VramTileCursor, 0);
	g_VramTileCursor = Spr_PreloadToHeap(&g_SpriteDefs[g_SpriteDefCount], g_VramTileCursor);
#ifdef XMEN2_MPW
	g_SpriteDefCount++;
#else
	return g_SpriteDefCount++;	/* what MPW leaves in d0 */
#endif
}
