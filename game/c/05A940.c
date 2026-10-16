/* BaniMazaB2_CrackFloorHook - map tile hook 2 of BaniMazaB2 (Exterior): a cracked floor tile (8,
 * 9, 10) touched at (col, row) advances one stage (9, 10, 11) along its whole floor segment (the
 * type $14 one in the block, else type $13): every block column from the segment's x1 + 8 to
 * x2 - 8 on that row becomes the next stage ("animated"). Stage 11 breaks it, column by column:
 * the block below becomes $67, the broken block's hook-type bits ($3800) are cleared, every live
 * object standing on the segment is detached (Obj_Detach), a climbing player holding on to it
 * gets the empty BaniMazaB2_NopStatic, the segment is freed (again for every column: the
 * original's loop does all of it per column), boulder debris falls from the block's centre to
 * the floor below (the screen y of the first floor segment below the touched block, 32000 with
 * none) and sound $E3 plays.
 *
 * col, row: the block touched (blocks).
 * Returns 0; any other tile, or no segment, does nothing. */
#include "xmen2.h"

long BaniMazaB2_CrackFloorHook(register short col, register short row)
{
	short first, last, floorY;
	register short tile, stage;
	register Seg *seg;
	register Seg *below;
	register long c;
	register long i;
	register Actor *o;

	tile = g_FgBlockMap[g_FgRowTable[row] + col] & 0x1FF;
	floorY = 0x7D00;
	if (tile == 8)
		stage = 9;
	else if (tile == 9)
		stage = 10;
	else if (tile == 10)
		stage = 11;
	else
		return 0;
	if ((seg = Seg_FindInBlock(0x14, col, row)) != 0 || (seg = Seg_FindInBlock(0x13, col, row)) != 0) {
		first = (seg->x1 + 8) >> 5;
		last = (seg->x2 - 8) >> 5;
		if (stage == 11) {
			if ((below = Seg_FindBelow((col << 5) + 16, (row << 5) + 32, 1000)) != 0)
				floorY = MIN(below->y1, below->y2) - g_CameraY;
		}
		for (c = first; c <= last; c++) {
			Map_SetBlockFG(c, row, stage, 1);
			if (stage == 11) {
				Map_SetBlockFG(c, row + 1, 0x67, 1);
				g_FgBlockMap[g_FgRowTable[row] + c] &= 0xC7FF;
				for (i = 0, o = g_ObjTable[0]; i < g_ObjCount; o = g_ObjTable[++i]) {
					if (IS_ALIVE(o)) {
						if (o->seg == seg)
							Obj_Detach(o);
						if (Player_IsClimber(o) && ((XClimber *)o)->climbSeg == seg)
							BaniMazaB2_NopStatic(o);
					}
				}
				Seg_Free(seg);
				Boulder_SpawnDebris((c << 5) + 16, (row << 5) + 16, floorY);
				Snd_Stop(0xE3);
				Snd_Play(0xE3);
			}
		}
	}
	return 0;
}
