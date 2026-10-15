/* BabyBio_GetMoveLimit - BABYBIO class slot +$7A: how far it may be knocked in a direction
 * (XNpc_HurtReact / XNpc_DeathReact stop the knockback there).
 *
 * self: the object.
 * dir:  0 left, 1 right: the end of its patrol range (xMin / xMax), *kind = 0 if that end is a
 *       wall, 1 if it is an open edge; 2, 3 (up, down): its home y, *kind = 0.
 * Returns the limit in pixels; any other dir returns nothing in particular and leaves *kind. */
#include "xmen2.h"

short BabyBio_GetMoveLimit(BabyBio *self, char dir, u8 *kind)
{
	switch (dir) {
	case 0:				/* BabyBio_GetMoveLimit_Horiz */
		if (self->wallLeft)
			*kind = 0;
		else
			*kind = 1;
		return self->xMin;
	case 1:
		if (self->wallRight)
			*kind = 0;
		else
			*kind = 1;
		return self->xMax;
	case 2:				/* BabyBio_GetMoveLimit_Vert */
	case 3:
		*kind = 0;
		return self->homeY >> 8;
	}
	/* BabyBio_GetMoveLimit_Done */
	NO_RETURN_VALUE
}
