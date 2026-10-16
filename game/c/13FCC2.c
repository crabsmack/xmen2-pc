/* Spark_Draw - SPARK Draw (class slot +$1A): with a preloaded sprite definition (type $20) one
 * 16x16 hardware sprite, 4 tiles a frame from the instance's first tile, its palette and
 * priority, the corner 4 px left of and 8 px above the position; any other definition, the
 * parent's (ACTOR) Draw.
 *
 * self: the object. No screen test. Never runs in the corpus. */
#include "xmen2.h"

void Spark_Draw(register Spark *self)
{
	register short attr;
	register short sx = (self->x >> 8) - g_CameraX;
	register short sy = (self->y >> 8) - g_CameraY;

	if (((SprDef *)self->spr.def)->type != 0x20) {
		(*CLASS(Tbl_Spark_vtable)->parent->method[5])(self);	/* ACTOR Draw */
		return;
	}
	sx -= 4;
	sy -= 8;
	attr = self->spr.tileBase + (self->spr.frame << 2) | (self->spr.flags & 3) << 13;
	if (self->spr.flags & 4 ? 1 : 0)
		attr |= 0x8000;
	g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
	g_SprWritePtr->y = sy + 0x80;
	g_SprWritePtr->size = 5;
	g_SprWritePtr->attr = attr;
	g_SprWritePtr->x = sx + 0x80;
}
