/* RetractFloor_Draw - RETRACTFLOOR Draw (class slot +$1A; the parent's isn't called): the floor
 * as two halves, each slid offset px away from its position (the centre), drawn straight into
 * the sprite table as 32x16 hardware sprites of the g_RetractFloorSprDef tiles (palette 0).
 *
 * self: the object.
 * Nothing once the halves are more than $80 apart. Each half: an inner piece (the definition's
 * first tiles), then up to two outer pieces (the next 8 tiles), 32 px further out each; the
 * right half mirrored. The outer pieces stop when the right one would start right of screen x
 * 320; no other screen test (pieces far off screen still take sprite slots). The position on
 * screen is animDist behind for a distance-driven animation, as ACTOR Draw. */
#include "xmen2.h"

#define DRAW_X(o)	((o)->anim && (*(u8 *)(o)->anim & 0x30) == 0x20 \
			? (((o)->x - (o)->animDist) >> 8) - g_CameraX : ((o)->x >> 8) - g_CameraX)
#define DRAW_Y(o)	((o)->anim && (*(u8 *)(o)->anim & 0x30) == 0x30 \
			? (((o)->y - (o)->animDist) >> 8) - g_CameraY : ((o)->y >> 8) - g_CameraY)

void RetractFloor_Draw(register RetractFloor *self)
{
	register short off = self->offset;
	register short sx = DRAW_X(self);
	register short sy = DRAW_Y(self);
	short tile = g_SpriteDefs[g_RetractFloorSprDef].tileBase | 0;
	long left = sx - off - 32;	/* screen x of the left half's piece */
	register long right = sx + off;	/* of the right half's (mirrored) piece */
	register long y = sy;
	register long attr = tile;
	register long i;

	if (off > 0x80)
		return;
	g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
	g_SprWritePtr->y = y + 0x80;
	g_SprWritePtr->size = 0xD;
	g_SprWritePtr->attr = attr;
	g_SprWritePtr->x = left + 0x80;
	g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
	g_SprWritePtr->y = y + 0x80;
	g_SprWritePtr->size = 0xD;
	g_SprWritePtr->attr = attr | 0x800;
	g_SprWritePtr->x = right + 0x80;
	attr += 8;
	for (i = 1; i < 3; i++) {
		left -= 32;
		right += 32;
		if (right > VIEW_R(0x140))
			return;
		g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
		g_SprWritePtr->y = y + 0x80;
		g_SprWritePtr->size = 0xD;
		g_SprWritePtr->attr = attr;
		g_SprWritePtr->x = left + 0x80;
		g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
		g_SprWritePtr->y = y + 0x80;
		g_SprWritePtr->size = 0xD;
		g_SprWritePtr->attr = attr | 0x800;
		g_SprWritePtr->x = right + 0x80;
	}
}
