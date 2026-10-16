/* SiberiaB1_IceTileHit - the Blizzard's breakable-tile hook (g_TileHooks[2]): a hit on an ice
 * block cracks the whole column of that block (the blocks above and below with the same
 * number): $6E -> $6F -> $70, crack sound $33. A hit on block $70 shatters the column: the
 * shards' floor is the segment below it (its higher end), each block throws 4 shards
 * (SiberiaB1_SpawnDebris) and loses its map bits $8000 and $3800 and becomes block 0; then the
 * type-$14 segment in the block is freed, with every live object standing on it detached first
 * (Obj_Detach), and sound $35. Any other block: nothing.
 *
 * col, row: the block hit (BG map cells). Returns 0, no value for a block that isn't ice. */
#include "xmen2.h"

#define g_IceFloorY	(*(s16 *)g_SceneVar3B0C)	/* scene variable: floor line the shards bounce on, pixels */

long SiberiaB1_IceTileHit(register short col, short row)
{
	short next;
	register short block = g_BgBlockMap[g_BgRowTable[row] + col] & 0x1FF;
	register unsigned char crack = 1;
	register short r;
	register Seg *seg;
	register Actor *o;

	if (block == 0x6E)
		next = 0x6F;
	else if (block == 0x6F)
		next = 0x70;
	else if (block == 0x70) {
		next = 0;
		crack = 0;
	} else
		RETURN_NO_VALUE;
	if (!crack) {
		if ((seg = Seg_FindBelow((col << 5) + 16, (row << 5) + 16, 1000)) != 0)
			g_IceFloorY = MIN(seg->y1, seg->y2);
	}
	for (r = row - 1; block == (g_BgBlockMap[g_BgRowTable[r] + col] & 0x1FF); r--) {
		if (!crack) {
			SiberiaB1_SpawnDebris((col << 5) + 16, (r << 5) + 16);
			g_BgBlockMap[g_BgRowTable[r] + col] &= 0x7FFF;
			g_BgBlockMap[g_BgRowTable[r] + col] &= 0xC7FF;
		}
		Map_SetBlockBG(col, r, next, crack);
	}
	for (r = row; block == (g_BgBlockMap[g_BgRowTable[r] + col] & 0x1FF); r++) {
		if (!crack) {
			SiberiaB1_SpawnDebris((col << 5) + 16, (r << 5) + 16);
			g_BgBlockMap[g_BgRowTable[r] + col] &= 0x7FFF;
			g_BgBlockMap[g_BgRowTable[r] + col] &= 0xC7FF;
		}
		Map_SetBlockBG(col, r, next, crack);
	}
	if (!crack) {
		if ((seg = Seg_FindInBlock(0x14, col, row)) != 0) {
			register long n;

			for (n = 0, o = g_ObjTable[0]; n < g_ObjCount; ++n, o = g_ObjTable[n])
				if (IS_ALIVE(o) && o->seg == seg)
					Obj_Detach(o);
			Seg_Free(seg);
		}
		Snd_Stop(0x35);
		Snd_Play(0x35);
	} else {
		Snd_Stop(0x33);
		Snd_Play(0x33);
	}
	return 0;
}
