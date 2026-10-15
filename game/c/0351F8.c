/* AvalonA4_GlobCrackBlockHook - map tile hook 2 of the Throne Room: a block hit by a MAGNETICGLOB
 * (Magneto's globe) cracks one stage ($8D -> $AD -> $AE -> $AF -> $B0); a $B0 block explodes
 * (Explosion_Spawn at its centre) and becomes $8E. Sound $1A for any block the globe hits.
 *
 * col, row: the map block (blocks).  obj: the object that hit it.
 * Returns 0. Other objects do nothing. */
#include "xmen2.h"

long AvalonA4_GlobCrackBlockHook(register short col, register short row, Actor *obj)
{
	register short tile;

	if (obj->cls != CLASS(Tbl_MagneticGlob_vtable))
		return 0;
	tile = g_FgBlockMap[g_FgRowTable[row] + col] & 0x1FF;
	if (tile == 0x8D)
		Map_SetBlockFG(col, row, 0xAD, 1);
	else if (tile == 0xAD)
		Map_SetBlockFG(col, row, 0xAE, 1);
	else if (tile == 0xAE)
		Map_SetBlockFG(col, row, 0xAF, 1);
	else if (tile == 0xAF)
		Map_SetBlockFG(col, row, 0xB0, 1);
	else if (tile == 0xB0) {
		register long x, y;

		x = ((col << 5) + 16) << 8;
		y = ((row << 5) + 16) << 8;
		Explosion_Spawn(x, y, 0);
		Map_SetBlockFG(col, row, 0x8E, 0);
	}
	Snd_Stop(0x1A);
	Snd_Play(0x1A);
	return 0;
}
