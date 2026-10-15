/* BanimazaDoor_Draw - BANIMAZADOOR Draw (class slot +$1A; the parent's isn't called). While
 * solid, its floor segment and both boxes follow the column: x - 5 .. x + $25, from its home y
 * (py) down to y. Then by look: 1 and 2 the column (BanimazaDoor_DrawColumn), 3 the shards
 * (BanimazaDoor_DrawDebris), 0 nothing.
 *
 * self: the door. */
#include "xmen2.h"

void BanimazaDoor_Draw(register BanimazaDoor *self)
{
	register Seg *seg;

	if (self->solid) {
		seg = &g_Segs[self->segIndex];
		self->box2Top = self->py;
		self->boxTop = self->py;
		seg->y1 = self->py;
		seg->y2 = self->boxBottom = self->box2Bottom = self->y >> 8;
		seg->x1 = self->box2Left = self->boxLeft = self->px - 5;
		seg->x2 = self->box2Right = self->boxRight = self->px + 0x25;
	}
	switch (self->look) {
	default:
	case 0:
		break;
	case 1:		/* BanimazaDoor_Draw_part1 */
		BanimazaDoor_DrawColumn(self);
		break;
	case 2:
		BanimazaDoor_DrawColumn(self);
		break;
	case 3:
		BanimazaDoor_DrawDebris(self->shards, self->shardCount, self->tileBase);
		break;
	}
	/* BanimazaDoor_Draw_part2 */
}
