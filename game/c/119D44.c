/* SyncActor_ClassInit - per-level class setup of SYNCHRONIZEDACTOR (class slot +$5E; the parent's
 * isn't called): the switch board g_BBoard (BBOARD's New, 100 entries) unless there is one. Its
 * arguments (0, the class's index) are unused. */
#include "xmen2.h"

void SyncActor_ClassInit(void)
{
	if (!g_BBoard)
		g_BBoard = (BBoard *)(*CLASS(Tbl_BBoard_vtable)->method[1])(CLASS(Tbl_BBoard_vtable), 100);	/* BBoard_New */
}
