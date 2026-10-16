/* LineSegKiller_Kill - LINESEGKILLER Kill (class slot +$52): remove the floor segments of any type
 * inside its box (LineSegKiller_RemoveSegsInBox from its position to right / bottom), then the
 * parent's (ACTOR) Kill.
 *
 * self: the object. Its segType isn't used: the type passed is $3E9 (any). */
#include "xmen2.h"

void LineSegKiller_Kill(LineSegKiller *self)
{
	LineSegKiller_RemoveSegsInBox(0x3E9, self->x >> 8, self->y >> 8, self->right, self->bottom);
	(*CLASS(Tbl_LineSegKiller_vtable)->parent->method[19])(self);	/* ACTOR Kill */
}
