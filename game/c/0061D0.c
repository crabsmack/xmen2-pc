/* Player_CharIndexOf - the character index of a player class: 0 Beast, 1 Cyclops, 2 Gambit,
 * 3 Nightcrawler, 4 Psylocke, 5 Wolverine, 6 Magneto (g_P1CharIndex...).
 *
 * cls: a class descriptor (0: none).
 * Returns the index, -1 for none or any other class. */
#include "xmen2.h"

short Player_CharIndexOf(register Class *cls)
{
	register short i = -1;

	if (cls) {
		if (cls == CLASS(Tbl_Beast_vtable))
			i = 0;
		if (cls == CLASS(Tbl_Cyclops_vtable))
			i = 1;
		if (cls == CLASS(Tbl_Gambit_vtable))
			i = 2;
		if (cls == CLASS(Tbl_Nightcrawler_vtable))
			i = 3;
		if (cls == CLASS(Tbl_Psylocke_vtable))
			i = 4;
		if (cls == CLASS(Tbl_Wolverine_vtable))
			i = 5;
		if (cls == CLASS(Tbl_Magneto_vtable))
			i = 6;
	}
	return i;
}
