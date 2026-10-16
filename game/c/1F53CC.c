/* Level_AssignBlockSlots - give each special block of a map (its special-block list) 16 VRAM tiles
 * of its own: the layout cell then shows a "slot" block (Level_FillBlockSlot) whose tiles a scene
 * can overwrite (animated blocks).
 *
 * map: the map blob.  layout: its decoded block layout.  defs: the block definitions (the new
 * slot blocks follow the used ones, up to g_HeapTop).  firstTile: the slots' first VRAM tile.
 * Slot s (1-12) is block (heap top - defs) / 32 + s - 1, filled when first used; the cell keeps
 * its flag bits ($FE00). Returns the highest slot used + 1 (0 with none): the 16-tile groups the
 * caller must leave room for. g_HeapTop moves past the last slot block. */
#include "xmen2.h"

short Level_AssignBlockSlots(void *map, void *layout, void *defs, short firstTile)
{
	s16 *end;
	short x;
	short y;
	short w;
	short count;
	s16 *cell;
	char slotUsed[12];
	char used;
	register short i;
	register short base = 0;
	register short v;	/* the map's height in blocks, then a cell's block (both unused), then the highest slot used */
	register short slot;
	register s16 *p;

	base = (short)((MapBlock *)g_HeapTop - (MapBlock *)defs);
	for (i = 0; i < 12; i++)
		slotUsed[i] = 0;
	w = (short)((MapHeader *)map)->width >> 2;
	v = (short)((MapHeader *)map)->height >> 2;
	p = (s16 *)((char *)map + ((MapHeader *)map)->specials);
	if ((count = *p) > 0) {
		p++;
		for (i = 0; i < count; p += 3, i++) {	/* MapSpecial entries */
			slot = ((MapSpecial *)p)->slot;
			x = ((MapSpecial *)p)->x;
			y = ((MapSpecial *)p)->y;
			cell = (s16 *)layout + y * w + x;
			v = *cell;
			if (slot) {
				slot--;
				used = slotUsed[slot];
				slotUsed[slot] = 1;
				slot += base;
				if (!used) {
					end = (s16 *)defs + (((slot & 0x1FF) + 1) << 4);
					if (g_HeapTop < end)
						g_HeapTop = end;
					Level_FillBlockSlot(defs, firstTile, slot, base, 0);
				}
				*cell = slot | *cell & ~0x1FF;
			}
		}
	}
	v = -1;
	for (i = 0; i < 12; i++)
		if (slotUsed[i])
			v = i;
	return v + 1;
}
