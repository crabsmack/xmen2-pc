/* Seg_LoadList - create floor segments from a level's segment list (ROM), then build the grid.
 *
 * list: entries until a type byte 0; append: 0 for the level's first (FG) list, else a list
 * appended after it (BG).
 * Each entry is set up (Seg_Setup) in the next g_Segs record from g_SegFirstFree on. An entry
 * with type bit 6 has no links (10 bytes): no prev/next. Else prev/next are list indexes
 * ($FF none), offset by the first record's index when appending; appended segments get flags
 * bit 2. Then g_SegLevelCount (first list only), g_SegFirstFree and g_SegHighWater are set, an
 * empty heap block is opened (Mem_StackAlloc), Seg_BuildGrid builds the grid on the heap and the
 * block's header (its next-top long) and g_SegHeapEnd get the new heap top. */
#include "xmen2.h"

void Seg_LoadList(SegListEntry *list, register char append)
{
	register Seg *s = g_Segs + g_SegFirstFree;
	short n = g_SegFirstFree;
	short first = n;
	register SegListEntry *p = list;
	register PTR(void) *hdr;

	for (; p->type; s++, n++) {
		Seg_Setup(s, p->type & 0x3F, p->layer, p->x1, p->y1, p->x2, p->y2);
		if (p->type & 0x40) {
			s->prev = 0xFF;
			s->next = 0xFF;
			p++;
			p = AT68(SegListEntry, ADDR68(p) - 2);	/* 10 bytes: no links */
		} else {
			if (!append) {
				s->prev = p->prev == 0xFF ? -1 : p->prev;
				s->next = p->next == 0xFF ? -1 : p->next;
			} else {
				s->prev = p->prev == 0xFF ? -1 : p->prev + first;
				s->next = p->next == 0xFF ? -1 : p->next + first;
			}
			p++;
		}
		if (append)
			s->flags |= 4;
	}
	if (!append)
		g_SegLevelCount = n;
	g_SegHighWater = (g_SegFirstFree = n) - 1;
	hdr = (PTR(void) *)g_HeapTop;
	Mem_StackAlloc(0, STR(Str_Empty_1F6D74, ""));
	Seg_BuildGrid();
	*hdr = g_HeapTop;
	g_SegHeapEnd = g_HeapTop;
}
