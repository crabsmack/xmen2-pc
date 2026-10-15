/* Magneto_ShotOrigin - MAGNETO: where a shot fired standing starts and how it flies.
 *
 * dir:   aim direction 1-8 (1/8 down-diagonal, 2/7 level, 3/6 up-diagonal, 4/5 up; 1-4 facing
 *        left, 5-8 right).
 * dx/dy: the hand's offset from the position (pixels; dy counts up).
 * angle: flight angle in degrees, 8 fraction bits (0 right, 90 up, 180 left).
 * type:  hit type of the shot: 9 facing left, 10 right (straight up too, unlike in the air).
 * Any other dir leaves all four as they were. Cases: Magneto_ShotOrigin_Case1/7/8 (2-6 are
 * unnamed blocks), _Ret. */
#include "xmen2.h"

void Magneto_ShotOrigin(char dir, short *dx, short *dy, long *angle, char *type)
{
	register char d = dir;
	register short *hy = dy;	/* the hand offset */
	register short *hx = dx;

	switch (d) {
	case 1:			/* Magneto_ShotOrigin_Case1 */
		*hx = -0x45;
		*hy = 1;
		*angle = 225L << 8;
		*type = 9;
		break;
	case 2:
		*hx = -0x63;
		*hy = 0x3B;
		*angle = 180L << 8;
		*type = 9;
		break;
	case 3:
		*hx = -0x45;
		*hy = 0x6F;
		*angle = 135L << 8;
		*type = 9;
		break;
	case 4:
		*hx = -0x0E;
		*hy = 0x8F;
		*angle = 90L << 8;
		*type = 9;
		break;
	case 5:
		*hx = 0x0E;
		*hy = 0x8F;
		*angle = 90L << 8;
		*type = 10;
		break;
	case 6:
		*hx = 0x45;
		*hy = 0x6F;
		*angle = 45L << 8;
		*type = 10;
		break;
	case 7:			/* Magneto_ShotOrigin_Case7 */
		*hx = 0x63;
		*hy = 0x3B;
		*angle = 0;
		*type = 10;
		break;
	case 8:			/* Magneto_ShotOrigin_Case8 */
		*hx = 0x45;
		*hy = 1;
		*angle = 315L << 8;
		*type = 10;
		break;
	}
	/* Magneto_ShotOrigin_Ret */
}
