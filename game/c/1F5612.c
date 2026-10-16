/* Level_LoadUsedTiles - load a packed tileset's tiles to VRAM, only those a map uses: the blocks
 * its layout uses are kept (renumbered in the layout, moved down in the definitions), then the
 * tiles those blocks use (renumbered in the definitions), and the tiles are Nemesis-decoded with
 * the used-tile mask.
 *
 * map: the map blob.  tileset: the tileset blob (packed tiles).  vram: the first VRAM tile.
 * layout: the decoded block layout.  defs: the decoded block definitions.  fromMap: keep only what
 * the map uses (else every block and tile, as numbered).
 * g_UsedBlockCount gets the blocks kept, g_HeapTop moves to the end of their definitions.
 * Returns the tiles loaded. */
#include "xmen2.h"

short Level_LoadUsedTiles(void *map, void *tileset, short vram, s16 *layout, void *defs, unsigned char fromMap)
{
	u8 bits[0x100];
	s16 remap[0x800];
	s16 *p;
	register short tiles = ((MapHeader *)tileset)->tileCount;
	register short blocks = ((MapHeader *)tileset)->blockCount;
	register short i;

	{
		register long cells = ((short)((MapHeader *)map)->width >> 2) * ((short)((MapHeader *)map)->height >> 2);

		if (fromMap) {
			Bitmap_FromTileWords(bits, layout, cells, 0x1FF);
			Bitmap_Set(bits, 0);
		} else
			for (i = 0; i < blocks; i++)
				Bitmap_Set(bits, i);
		g_UsedBlockCount = blocks = (short)Level_CompactUsedItems(bits, blocks, remap, 1, (s16 *)defs, 4);
		Tile_RemapWords(layout, remap, 0x1FF, cells);
	}
	g_HeapTop = (s16 *)defs + (blocks << 4);
	{
		register short words = (short)((long)blocks << 4);

		if (fromMap) {
			Bitmap_FromTileWords(bits, defs, words, 0x7FF);
			Bitmap_Set(bits, 0);
		} else
			for (i = 0; i < tiles; i++)
				Bitmap_Set(bits, i);
		tiles = (short)Level_CompactUsedItems(bits, tiles, remap, 0, 0, -1);
		Tile_RemapWords(defs, remap, 0x7FF, words);
	}
	{
#ifdef XMEN2_MPW
		long ts = (long)tileset;	/* the blob's address as a number, as the original */

		p = (s16 *)((short)((MapHeader *)ts)->tiles + ts);
#else
		p = (s16 *)((char *)tileset + (short)((MapHeader *)tileset)->tiles);
#endif
		i = *p++;
	}
	p = (s16 *)Nem_BuildCodeTable(p, remap);
	/* Mode 2: masked, with the used-tile mask (bits) as a sixth argument, which Nem_Decode reads
	   ($1C(a6)). MPW sees its five-argument prototype (the mode-1 callers'), so this call goes
	   without one, as the original's. */
#ifdef XMEN2_MPW
	((void (*)())Nem_Decode)(p, remap, i, 2, (long)vram << 5, bits);
#else
	Nem_Decode(p, (NemCodeTable *)remap, i, 2, (long)vram << 5, (NemTileMask *)bits);
#endif
	return tiles;
}
