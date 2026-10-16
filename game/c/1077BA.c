/* Switch_ClassInit - per-level class setup of SWITCH (class slot +$5E; the parent's isn't called):
 * the switch board g_BBoard (BBOARD's New, 100 entries) unless there is one, the placement search
 * record g_SwitchSearchRec (16 bytes, Mem_StackAlloc), no search running. Its arguments (0, the
 * class's index) are unused. */
#include "xmen2.h"

void Switch_ClassInit(void)
{
	if (!g_BBoard)
		g_BBoard = (BBoard *)(*CLASS(Tbl_BBoard_vtable)->method[1])(CLASS(Tbl_BBoard_vtable), 100);	/* BBoard_New */
	g_SwitchSearchRec = (SwitchSearch *)Mem_StackAlloc(0x10, STR(Str_Empty_1077FE, ""));
	g_SwitchSearchBusy = 0;
}
