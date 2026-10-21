/* ThronePhone_Draw - THRONEPHONE Draw (class slot +$1A; the parent's isn't called): the beam as
 * `rows` rows of two 8x8 hardware sprites (high priority, tiles 0-3 of its sprite definition by
 * row), mirrored about its x by sin(angleX) >> shiftX and moved in y by cos(angleY) >> shiftY.
 * The first row is `height` px above it, each next one rowStep lower; both phases step by
 * angle*Step every frame (wrapped above 360 and below 0) and by angle*RowStep every row.
 * A sprite is drawn only with its x and y inside -8 .. $140 / $E0 (screen pixels, exclusive).
 *
 * self: the beam.
 * MPW: the phases are stored back twice (ax = self->angleX = ...: the ROM's add, reload, store)
 * and their wrap stores the field from the local and the local from that; the ROM does both. */
#include "xmen2.h"

void ThronePhone_Draw(register ThronePhone *self)
{
	short base = g_SpriteDefs[g_ThronePhoneSprDef].tileBase | 0x8000;
	short rowY = (self->y >> 8) - g_CameraY - self->height;
	short cx = (self->x >> 8) - g_CameraX;
	long ax = self->angleX = (self->angleX += self->angleXStep);
	long ay = self->angleY = (self->angleY += self->angleYStep);
	register long x1, x2;
	register long i;
	register long y;

	if (ax > 360) {
		ax -= 360;
		ax = self->angleX = ax;
	} else if (ax < 0) {
		ax += 360;
		ax = self->angleX = ax;
	}
	if (ay > 360) {
		ay -= 360;
		ay = self->angleY = ay;
	} else if (ay < 0) {
		ay += 360;
		ay = self->angleY = ay;
	}
	for (i = 0; i < self->rows; i++, rowY += self->rowStep) {
		short attr = base + (i & 3);
		long off = Math_SinDeg(ax) >> self->shiftX;

		y = Math_CosDeg(ay) >> self->shiftY;
		y += rowY;
		x1 = cx + off;
		x2 = cx - off;
		if (y > -8 && y < 0xE0) {
			if (x1 > VIEW_L(-8) && x1 < VIEW_R(0x140)) {
				g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
				g_SprWritePtr->y = y + 0x80;
				g_SprWritePtr->size = 0;
				g_SprWritePtr->attr = attr;
				g_SprWritePtr->x = x1 + 0x80;
			}
			if (x2 > VIEW_L(-8) && x2 < VIEW_R(0x140)) {
				g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
				g_SprWritePtr->y = y + 0x80;
				g_SprWritePtr->size = 0;
				g_SprWritePtr->attr = attr;
				g_SprWritePtr->x = x2 + 0x80;
			}
		}
		ax += self->angleXRowStep;
		ay += self->angleYRowStep;
	}
}
