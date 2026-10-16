/* Barrier_ClassInit - per-level class setup of BARRIER (class slot +$5E): the empty
 * Barrier_NopStatic, then its sprite set, Art_BarrierSpr (palette 0), as g_BarrierSprDef. The
 * parent's isn't called. */
#include "xmen2.h"

void Barrier_ClassInit(void)
{
	Barrier_NopStatic();
	g_BarrierSprDef = SprDef_Add(CODEPTR(Art_BarrierSpr), 0);
}
