/* Scene_Register - append a scene {name, function} to g_SceneTable.
 *
 * name: its name (a C string).  run: its function.
 * Returns its id (the index). No bounds check. */
#include "xmen2.h"

/* The original's file defined g_SceneTable: MPW C keeps the address of an array defined in the
   compile unit in a register (lea g_SceneTable(a5),a4) and indexes it with .w, copied here. */
#ifdef XMEN2_MPW
SceneEntry g_SceneTable[52];
#endif

unsigned char Scene_Register(char *name, SceneFn run)
{
	register unsigned char i;

	i = g_SceneCount++;
	g_SceneTable[i].name = name;
	g_SceneTable[i].run = run;
	return i;
}
