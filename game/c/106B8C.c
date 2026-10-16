/* Platform1_ClassInit - per-level class setup of PLATFORM1 (class slot +$5E): register its sprite
 * set, Art_Platform1Spr (palette 0), as g_Platform1SprDef (unless that is -1: Platform1_Init then
 * uses the definition directly), then the parent's (SYNCHRONIZEDACTOR: the switch board; given 0)
 * and the empty Platform1_EmptyStub. Its own arguments (0, the class's index) are unused. */
#include "xmen2.h"

void Platform1_ClassInit(void)
{
	if (g_Platform1SprDef != -1)
		g_Platform1SprDef = SprDef_Add(CODEPTR(Art_Platform1Spr), 0);
	(*CLASS(Tbl_Platform1_vtable)->parent->method[22])(0);	/* SYNCHRONIZEDACTOR ClassInit */
	Platform1_EmptyStub();
}
