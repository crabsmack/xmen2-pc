/* XClone_SpawnByType - create the evil clone a spawn record asks for: by its type (param[0]:
 * 1 Beast, 2 Cyclops, 3 Gambit, 4 Magneto, 5 Nightcrawler, 6 Psylocke, 7 Wolverine), or with
 * type 0 the clone of the player's character (Clone_CharTypeOf).
 *
 * place:  the spawn record: x, y in pixels (the new object gets the record as its placement).
 * player: whose character to clone for type 0 (may be 0; EvilDrop_ActRelease passes nothing).
 * Returns the new object, 0 for another type or when none could be made.
 * The cases are XClone_SpawnByType_Case1 .. _Case7 and the tail _Done in the disassembly. */
#include "xmen2.h"

XClone *XClone_SpawnByType(Placement *place, XPlayer *player)
{
	XClone *clone;
	short type;

	clone = 0;
	if (!(type = place->param[0]) && player)
		type = Clone_CharTypeOf(player);
	switch (type) {
	case 1:		/* XClone_SpawnByType_Case1 */
		clone = (XClone *)(*CLASS(Tbl_EvilBeast_vtable)->method[2])(0,		/* New */
			CLASS(Tbl_EvilBeast_vtable), place->x << 8, place->y << 8, place);
		break;
	case 2:
		clone = (XClone *)(*CLASS(Tbl_EvilCyclops_vtable)->method[2])(0,
			CLASS(Tbl_EvilCyclops_vtable), place->x << 8, place->y << 8, place);
		break;
	case 3:
		clone = (XClone *)(*CLASS(Tbl_EvilGambit_vtable)->method[2])(0,
			CLASS(Tbl_EvilGambit_vtable), place->x << 8, place->y << 8, place);
		break;
	case 4:
		clone = (XClone *)(*CLASS(Tbl_EvilMagneto_vtable)->method[2])(0,
			CLASS(Tbl_EvilMagneto_vtable), place->x << 8, place->y << 8, place);
		break;
	case 5:
		clone = (XClone *)(*CLASS(Tbl_EvilNightcrawler_vtable)->method[2])(0,
			CLASS(Tbl_EvilNightcrawler_vtable), place->x << 8, place->y << 8, place);
		break;
	case 6:
		clone = (XClone *)(*CLASS(Tbl_EvilPsylocke_vtable)->method[2])(0,
			CLASS(Tbl_EvilPsylocke_vtable), place->x << 8, place->y << 8, place);
		break;
	case 7:		/* XClone_SpawnByType_Case7 */
		clone = (XClone *)(*CLASS(Tbl_EvilWolverine_vtable)->method[2])(0,
			CLASS(Tbl_EvilWolverine_vtable), place->x << 8, place->y << 8, place);
		break;
	}
	/* XClone_SpawnByType_Done */
	return clone;
}
