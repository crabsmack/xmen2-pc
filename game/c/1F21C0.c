/* XPlayer_InitSpawnRec - XPLAYER class slot +$0A: a player start in the level: the parent's
 * spawn record, which never spawns by itself, and the placement remembered as player 2's start
 * (param[0] 2) or player 1's (anything else; Players_PlaceAtStart).
 *
 * rec:   the spawn record.
 * place: its placement entry. */
#include "xmen2.h"

void XPlayer_InitSpawnRec(SpawnRec *rec, Placement *place)
{
	(*CLASS(Tbl_XPlayer_vtable)->parent->method[1])(rec, place);	/* ACTOR InitSpawnRec */
	rec->flags &= ~1;
	switch (place->param[0]) {
	case 2:
		g_P2StartRec = place;
		break;
	case 1:
	default:
		g_P1StartRec = place;
		break;
	}
}
