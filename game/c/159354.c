/* Magneto_ShotOriginAir - MAGNETO: where a shot fired in the air starts and how it flies.
 *
 * dir:   aim direction 0-9 (0/9 down, 1/8 down-diagonal, 2/7 level, 3/6 up-diagonal, 4/5 up;
 *        0-4 facing left, 5-9 right).
 * dx/dy: the hand's offset from the position (pixels; dy counts up).
 * angle: flight angle in degrees, 8 fraction bits (0 right, 90 up, 180 left, 270 down).
 * type:  hit type of the shot: 9 thrown left, 10 right, 11 down, 12 up.
 * Any other dir leaves all four as they were. Cases: Magneto_ShotOriginAir_Case0/9/1/2/3/7 (in
 * that order; 4, 5, 6 and 8 are unnamed blocks), _Ret. */
#include "xmen2.h"

void Magneto_ShotOriginAir(char dir, short *dx, short *dy, long *angle,
	char *type)
{
	register char d = dir;
	register short *hy = dy;	/* the hand offset */
	register short *hx = dx;

	switch (d) {
	case 0:			/* Magneto_ShotOriginAir_Case0 */
		*hx = -0x14;
		*hy = -0x12;
		*angle = 270L << 8;
		*type = 11;
		break;
	case 9:			/* Magneto_ShotOriginAir_Case9 */
		*hx = 0x14;
		*hy = -0x12;
		*angle = 270L << 8;
		*type = 11;
		break;
	case 1:			/* Magneto_ShotOriginAir_Case1 */
		*hx = -0x45;
		*hy = 1;
		*angle = 225L << 8;
		*type = 9;
		break;
	case 2:			/* Magneto_ShotOriginAir_Case2 */
		*hx = -0x63;
		*hy = 0x3B;
		*angle = 180L << 8;
		*type = 9;
		break;
	case 3:			/* Magneto_ShotOriginAir_Case3 */
		*hx = -0x45;
		*hy = 0x6F;
		*angle = 135L << 8;
		*type = 9;
		break;
	case 4:
		*hx = -0x10;
		*hy = 0x84;
		*angle = 90L << 8;
		*type = 12;
		break;
	case 5:
		*hx = 0x10;
		*hy = 0x84;
		*angle = 90L << 8;
		*type = 12;
		break;
	case 6:
		*hx = 0x45;
		*hy = 0x6F;
		*angle = 45L << 8;
		*type = 10;
		break;
	case 7:			/* Magneto_ShotOriginAir_Case7 */
		*hx = 0x63;
		*hy = 0x3B;
		*angle = 0;
		*type = 10;
		break;
	case 8:
		*hx = 0x45;
		*hy = 1;
		*angle = 315L << 8;
		*type = 10;
		break;
	}
	/* Magneto_ShotOriginAir_Ret */
}
