/* Barbarian_FindPlatforms - find BARBARIAN's own platform, the in-use type-$1A floor segment
 * whose rectangle holds its placement point, and list (neighbours, up to 12, $FF-terminated)
 * the in-use segments whose rectangles touch the platform's: every type but $1A, slopes only
 * when flat enough (|dx| > 4 |dy|; types $13-$1C always).
 *
 * self: the object.  pl: its placement entry (x, y in pixels).
 * No platform found: platform 0 and no neighbours.
 * Scans g_Segs up to g_SegHighWater; the rectangle test takes x1/y1 as the low corner. */
#include "xmen2.h"

void Barbarian_FindPlatforms(Barbarian *self, Placement *pl)
{
	short left, top;
	register Seg *seg;
	short x, y;
	char found;
	short right, bottom;
	register Seg *other;
	long n;
	long i;

	x = pl->x;
	y = pl->y;
	found = 0;
	for (i = 0; i < 12; i++)
		self->neighbours[i] = 0xFF;
	self->platform = 0;
	for (seg = g_Segs, i = 0; i <= g_SegHighWater; i++, seg++) {
		if (seg->flags & 0x80 && seg->type == 0x1A && x >= seg->x1 && x <= seg->x2
		    && y >= seg->y1 && y <= seg->y2) {
			found = 1;
			break;
		}
	}
	if (found) {
		left = seg->x1;
		top = seg->y1;
		right = seg->x2;
		bottom = seg->y2;
		n = 0;
		self->platform = seg;
		for (other = g_Segs, i = 0; i <= g_SegHighWater; i++, other++) {
			if (other->flags & 0x80
			    && (other->type >= 0x13 && other->type <= 0x1C
				|| ABS(other->x1 - other->x2) > ABS(other->y1 - other->y2) << 2)
			    && other->type != 0x1A && left <= other->x2 && right >= other->x1
			    && top <= other->y2 && bottom >= other->y1) {
				self->neighbours[n] = i;
				if (++n >= 12)
					break;
			}
		}
	}
}
