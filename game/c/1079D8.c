/* Switch_TileHit - map tile hook type 2 of the scenes with switches (SpaceB1/SpaceA1
 * InstallTileHooks): unless a search runs, start one for this tile's SWITCH placement: the search
 * record gets the block and the whole placement list, and a timer runs Switch_FindAndToggle on it
 * every frame.
 *
 * col, row: the tile's map block (the hook's other arguments are unused). Returns 0. */
#include "xmen2.h"

long Switch_TileHit(short col, short row)
{
	register SwitchSearch *s = g_SwitchSearchRec;
	register Timer *t;

	if (g_SwitchSearchBusy)
		return 0;
	g_SwitchSearchBusy = 1;
	s->col = col;
	s->row = row;
	s->cur = g_PlacementList;
	s->end = &g_PlacementList[g_SpawnRecCount];
	t = Timer_Create((TimerFn)Switch_FindAndToggle, 0, s, 0);
	Timer_Start(t);
	return 0;
}
