/* Boulder_SplitIntoPieces - set up the three pieces a BOULDER shatters into (Boulder_StateShatter):
 * at (-$30, -$30), (+$40, -$C) and (+4, +$30) quarter pixels from its position, moving
 * (vx/64 - 4, $10), (vx/64 + $C, 4) and (vx/64 - 8, 8), each landing on the boulder's layer.
 *
 * self: the boulder.
 * (The loop's default case, never reached, would leave a piece still.) */
#include "xmen2.h"

void Boulder_SplitIntoPieces(register Boulder *self)
{
	register BoulderPiece *p;
	register long x, y;
	long v;
	register short i;

	p = self->piece;
	x = self->x >> 6;
	y = self->y >> 6;
	v = self->vx >> 6;
	for (i = 0; i < 3; i++, p++) {
		p->layer = self->layer;
		switch (i) {
		case 0:
			p->x = x - 0x30;
			p->y = y - 0x30;
			p->vx = v - 4;
			p->vy = 0x10;
			break;
		case 1:
			p->x = x + 0x40;
			p->y = y - 0xC;
			p->vx = v + 0xC;
			p->vy = 4;
			break;
		case 2:
			p->x = x + 4;
			p->y = y + 0x30;
			p->vx = v - 8;
			p->vy = 8;
			break;
		default:
			p->vx = v;
			p->vx = 0;
			p->vy = 0;
			break;
		}
	}
}
