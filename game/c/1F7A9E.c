/* Seg_InitDefaults - initialise a floor-segment record: in use, type and layer, no neighbours.
 *
 * seg: the record; type: its handler (g_SegTypeHooks); layer: its layer.
 * flags $A0 (in use), unk_03 and unk_04 0, no prev/next ($FF), placeholder endpoints
 * (-100, -100)-(-90, -90), owner $FE, no next owned segment ($FF). Seg_Setup sets the rest. */
#include "xmen2.h"

void Seg_InitDefaults(Seg *seg, char type, char layer)
{
	seg->flags = (s8)0xA0;
	seg->type = type;
	seg->layer = layer;
	seg->unk_03 = 0;
	seg->unk_04 = 0;
	seg->prev = 0xFF;
	seg->next = 0xFF;
	seg->x1 = -100;
	seg->y1 = -100;
	seg->x2 = -90;
	seg->y2 = -90;
	seg->owner = (s8)0xFE;
	seg->unk_13 = 0xFF;
}
