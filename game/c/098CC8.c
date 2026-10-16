/* Clone_CharTypeOf - the character type of an object: a player of that character or its evil clone.
 *
 * obj: any object (its class decides).
 * Returns 1 Beast, 2 Cyclops, 3 Gambit, 4 Magneto, 5 Nightcrawler, 6 Psylocke, 7 Wolverine, else 0
 * (XClone_SpawnByType uses the same numbers). */
#include "xmen2.h"

#define CLS	(((Actor *)obj)->cls)

short Clone_CharTypeOf(void *obj)
{
	short type;

	type = 0;
	if (CLS == CLASS(Tbl_Beast_vtable) || CLS == CLASS(Tbl_EvilBeast_vtable))
		type = 1;
	else if (CLS == CLASS(Tbl_Cyclops_vtable) || CLS == CLASS(Tbl_EvilCyclops_vtable))
		type = 2;
	else if (CLS == CLASS(Tbl_Gambit_vtable) || CLS == CLASS(Tbl_EvilGambit_vtable))
		type = 3;
	else if (CLS == CLASS(Tbl_Magneto_vtable) || CLS == CLASS(Tbl_EvilMagneto_vtable))
		type = 4;
	else if (CLS == CLASS(Tbl_Nightcrawler_vtable) || CLS == CLASS(Tbl_EvilNightcrawler_vtable))
		type = 5;
	else if (CLS == CLASS(Tbl_Psylocke_vtable) || CLS == CLASS(Tbl_EvilPsylocke_vtable))
		type = 6;
	else if (CLS == CLASS(Tbl_Wolverine_vtable) || CLS == CLASS(Tbl_EvilWolverine_vtable))
		type = 7;
	return type;
}
