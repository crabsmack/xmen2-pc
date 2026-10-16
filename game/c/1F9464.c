/* Scene_Register - append a scene {name, function} to g_SceneTable.
 *
 * name: its name (a C string).  run: its function.
 * Returns its id (the index). No bounds check.
 * MPW: 4 bytes off (the ROM scales the index by 8 with asl.w and indexes with .w; this C gets the
 * .l forms); verified by shadow tests. */
#include "xmen2.h"

unsigned char Scene_Register(char *name, SceneFn run)
{
	register SceneEntry *t;
	register unsigned char i;

	t = g_SceneTable;
	i = g_SceneCount++;
	t[i].name = name;
	t[i].run = run;
	return i;
}
