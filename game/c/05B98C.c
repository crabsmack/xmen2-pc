/* Boulder_StatePiecesFall - a shattered BOULDER: each of its three pieces still on the screen
 * that can land (layer not 0) looks for a floor segment of its layer under it (Seg_FindAt,
 * 15 px); on one, it will next land on the layer below (0 stays 0) and hops up (vy -20) to the
 * left or right at random (vx -5 / 5). Once no piece is on the screen the boulder kills itself.
 *
 * self: the boulder.
 * Returns self->flags & 8 while a piece is on the screen; after the Kill, nothing. */
#include "xmen2.h"

short Boulder_StatePiecesFall(Boulder *self)
{
	register BoulderPiece *p = self->piece;
	register char gone = 1;
	register short right = g_CameraX + VIEW_R(320);
	register short bottom = g_CameraY + 224;
	register Seg *seg;
	register short i;

	for (i = 0; i < 3; i++, p++) {
		if (p->x >> 2 >= VIEW_L(g_CameraX) && p->x >> 2 <= right && p->y >> 2 >= g_CameraY
				&& p->y >> 2 <= bottom) {
			gone = 0;
			if (p->layer) {
				if (seg = Seg_FindAt(p->x >> 2, p->y >> 2, p->layer, 15)) {
					if (seg->layer > 0)
						p->layer = seg->layer - 1;
					else if (seg->layer == 0)
						p->layer = 0;
					p->vy = -20;
					if ((unsigned short)Rand_Next() & 1)
						p->vx = 5;
					else
						p->vx = -5;
				}
			}
		}
	}
	if (gone)
		(*self->cls->method[19])(self);		/* +$52 Kill */
	else {
		SET_STATE(self, Boulder_StatePiecesFall);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
