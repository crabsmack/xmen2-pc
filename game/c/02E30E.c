/* ExodusBlast_ClassInit - per-level class setup of EXODUSBLAST (class slot +$5E; the parent's
 * isn't called): its empty hook (ExodusBlast_NopStatic) and its sprite definition
 * (Art_ExodusBlastNem, palette 1, g_ExodusBlastSprDef).
 *
 * self: passed on to ExodusBlast_NopStatic (the class's index is the second argument, ignored). */
#include "xmen2.h"

void ExodusBlast_ClassInit(ExodusBlast *self)
{
	ExodusBlast_NopStatic(self);
	g_ExodusBlastSprDef = SprDef_Add(CODEPTR(Art_ExodusBlastNem), 1);
}
