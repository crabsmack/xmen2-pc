/* XClone_TileHookOpenWall - clone-maker map tile hook type 3: once the reward was given (clone
 * flags bit 1) and no player is Magneto, an object touching the tile that a clone of its own
 * character was killed by (clone flags bit 8 + type) blasts open the wall at map columns
 * $5A-$5B: blocks of rows $19-$1C replaced (4, $4A, $4B, $4B; row $1D block 4 flipped), debris
 * from the rows $1A-$1C blocks (landing on the floor below the tile) with their solid and hook
 * bits cleared, sound $35, and the solid wall segment (type $14) in the last block freed.
 *
 * col, row: the tile hit (block column and row).
 * obj:      the object touching it.
 * Returns 0; nothing in particular before the reward (and 0 then by accident). The block value
 * read first (BG block & $1FF) and the 0x4B kept in a variable are never used (dead stores the
 * original had). */
#include "xmen2.h"

#define g_CloneFlags	(*(s16 *)g_SceneVar3B08)	/* scene variable: 1 arena locked, 2 reward given, $100 << type killed by its own character */
#define g_CloneDebris	(*(PTR(DebrisPool) *)g_SceneVar3B1C)	/* scene variable: the clone-maker debris pool */

long XClone_TileHookOpenWall(short col, short row, Actor *obj)
{
	short type;
	char ownKill;
	short n;
	Seg *seg;
	s16 *block;
	Seg *wall;

	n = g_BgBlockMap[g_BgRowTable[row] + col] & 0x1FF;
	n = 0;
	ownKill = 0;
	if (g_CloneFlags & 2) {
		if (IS_ALIVE(&g_Player1) && g_Player1.cls == CLASS(Tbl_Magneto_vtable))
			n++;
		if (IS_ALIVE(&g_Player2) && g_Player2.cls == CLASS(Tbl_Magneto_vtable))
			n++;
		type = Clone_CharTypeOf(obj);
		if (g_CloneFlags & 1L << ((char)type + 8))
			ownKill = 1;
		if (n || !ownKill)
			return 0;
		ownKill = 0x4B;
		if (seg = Seg_FindBelow((col << 5) + 16, (row << 5) + 16, 1000)) {
			n = MIN(seg->y1, seg->y2);
			Debris_SetFloorY(g_CloneDebris, n);
		}
		n = 0x19;
		Map_SetBlockFG(0x5A, n, 4, 0);
		Map_SetBlockFG(0x5B, n, 4, 0);
		n++;
		Map_SetBlockFG(0x5A, n, 0x4A, 0);
		Map_SetBlockFG(0x5B, n, 0x4A, 0);
		n++;
		Map_SetBlockFG(0x5A, n, 0x4B, 0);
		Map_SetBlockFG(0x5B, n, 0x4B, 0);
		n++;
		Map_SetBlockFG(0x5A, n, 0x4B, 0);
		Map_SetBlockFG(0x5B, n, 0x4B, 0);
		n++;
		Map_SetBlockFGFlip(0x5A, n, 4, 0, 0, 1);
		Map_SetBlockFGFlip(0x5B, n, 4, 0, 0, 1);
		for (n = 0; n < 3; n++) {
			col = 0x5A;
			row = n + 0x1A;
			Debris_Spawn((col << 5) + 16, (row << 5) + 16, g_CloneDebris, 3);
			block = &g_FgBlockMap[g_FgRowTable[row] + col];
			*block &= 0x47FF;
			col++;
			Debris_Spawn((col << 5) + 16, (row << 5) + 16, g_CloneDebris, 3);
			block = &g_FgBlockMap[g_FgRowTable[row] + col];
			*block &= 0x47FF;
		}
		Snd_Stop(0x35);
		Snd_Play(0x35);
		if (wall = Seg_FindInBlock(0x14, col, row))
			Seg_Free(wall);
		return 0;
	}
	NO_RETURN_VALUE
}
