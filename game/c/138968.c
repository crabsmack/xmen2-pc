/* LineSegKiller_KillAll - kill (class slot +$52) every present LINESEGKILLER of g_ObjTable: each
 * removes the floor segments in its box (BroodClaw_Swing, when the claw smashes the floor). */
#include "xmen2.h"

void LineSegKiller_KillAll(void)
{
	register short i = 0;
	register Actor *o;

	for (i = 0, o = g_ObjTable[0]; i < g_ObjCount; o = g_ObjTable[++i]) {
		if (o->flags & 0x8000 && o->cls == CLASS(Tbl_LineSegKiller_vtable))
			(*o->cls->method[19])(o);		/* +$52 Kill */
	}
}
