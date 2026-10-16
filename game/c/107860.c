/* Switch_FindAndToggle - the switch search timer's callback (Switch_TileHit): look at the next 10
 * placement entries for the SWITCH whose map block is the hit tile's. Past the last entry: free
 * the timer, no search running. Found: toggle bit $200 of the BBoard entry its placement param[1]
 * names (sound $38, then $10 turning on, $16 off), the block becomes the next (on) or previous
 * (off) block id (Map_SetBlockFG), and the timer goes on as Switch_DebounceWait from this frame.
 *
 * s: the search record (the timer's argument). */
#include "xmen2.h"

void Switch_FindAndToggle(register SwitchSearch *s)
{
	register short col = s->col;
	register short row = s->row;
	register short n;
	register Placement *pl;
	register short on;
	s16 *flags;
	short block;

	for (n = 10; n; --n) {
		pl = s->cur++;
		if (pl > s->end) {
			Timer_Free((Timer *)g_TimerCurrent);
			g_SwitchSearchBusy = 0;
			return;
		}
		if (((ClassList *)g_ClassList)->cls[pl->classIndex] == CLASS(Tbl_Switch_vtable)
		    && pl->x >> 5 == col && pl->y >> 5 == row) {
			block = g_FgBlockMap[g_FgRowTable[row] + col] & 0x1FF;
			on = g_BBoard->entry[pl->param[1]] & 0x200;
			on = !on;
			Snd_Stop(0x38);
			Snd_Play(0x38);
			if (on) {
				flags = &g_BBoard->entry[pl->param[1]];
				*flags |= 0x200;
				Snd_Stop(0x10);
				Snd_Play(0x10);
				Map_SetBlockFG(col, row, block + 1, 1);
			} else {
				flags = &g_BBoard->entry[pl->param[1]];
				*flags &= ~0x200;
				Snd_Stop(0x16);
				Snd_Play(0x16);
				Map_SetBlockFG(col, row, block - 1, 1);
			}
			s->frame = g_FrameCounter;
			((Timer *)g_TimerCurrent)->func = (TimerFn)Switch_DebounceWait;
			return;
		}
	}
}
