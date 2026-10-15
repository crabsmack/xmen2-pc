/* ApocBomb_DrawShadow - a floor shadow under an object (BaniMazaA2_DrawHook, for each player in
 * the APOCALYPSE fight): two hardware sprites of the shadow frame (Art_ApocBombNem_2: size, dx,
 * dy, tile) 7 px left and right of the point, with the APOCBOMB tiles (g_ApocBombSprDef).
 *
 * obj: the object.
 * The point is the object itself while it stands on a floor segment, else on the arena's curved
 * floor, y = 232 - |x - 256| / 16. Emitted only when on screen (x > -2 dx, x < 320, y > -dy,
 * y < 224); palette and high priority from the object's sprite flags. The object's box width is
 * computed and never used (left over).
 * (Not byte-matched: the ROM keeps a word copy of the flags between width and gy on the stack
 * and computes width in 32 bits; executed, so verified by the shadow test.) */
#include "xmen2.h"

void ApocBomb_DrawShadow(register Actor *obj)
{
	register ApocBombFrame *f = (ApocBombFrame *)CODEPTR(Art_ApocBombNem_2);
	register short dx = f->dx;
	register short dy = f->dy;
	register short px = obj->x >> 8;
	register short d = ABS(px - 256);
	char size;
	short left, right, width;
	short gy = obj->seg ? obj->y >> 8 : 0xE8 - (d >> 4);
	register short x = px - g_CameraX - dx;
	register short y = gy - g_CameraY - dy;

	if (x > -(dx << 1) && x < 320 && y > -dy && y < 224) {
		register short attr = g_SpriteDefs[g_ApocBombSprDef].tileBase + f->tile;
		register unsigned char flags;

		size = f->size;
		flags = obj->spr.flags;
		left = obj->boxLeft;
		right = obj->boxRight;
		width = right - left;
		attr |= (flags & 3) << 13;
		attr |= flags & 4 ? 0x8000 : 0;
		g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
		g_SprWritePtr->y = y + 0x80;
		g_SprWritePtr->size = size;
		g_SprWritePtr->attr = attr;
		g_SprWritePtr->x = x - 7 + 0x80;
		g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
		g_SprWritePtr->y = y + 0x80;
		g_SprWritePtr->size = size;
		g_SprWritePtr->attr = attr;
		g_SprWritePtr->x = x + 7 + 0x80;
	}
}
