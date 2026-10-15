/* Spr_PreloadToHeap - Spr_PreloadTiles with its RAM copy kept on the heap.
 *
 * spr: the sprite instance.  tile: its first VRAM tile.
 * A mark block (Mem_StackAlloc of 0 bytes) is opened at the heap top; Spr_PreloadTiles builds the
 * definition's RAM copy above it, and the block's header then holds the new heap top, so the
 * copy stays allocated with the block. Returns the next free VRAM tile. */
#include "xmen2.h"

short Spr_PreloadToHeap(SprInst *spr, short tile)
{
	register short next;
	register PTR(void) *blk = (PTR(void) *)g_HeapTop;

	Mem_StackAlloc(0, STR(Str_EmptyTag, ""));
	next = Spr_PreloadTiles(spr, tile);
	*blk = g_HeapTop;
	return next;
}
