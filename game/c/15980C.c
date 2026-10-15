/* Magneto_BombOrigin - MAGNETO: where a flux bomb thrown standing starts and its angle.
 *
 * dir:   aim direction 1-8 (1/8 down-diagonal, 2/7 level, 3/6 up-diagonal, 4/5 up; 1-4 facing
 *        left, 5-8 right).
 * dx/dy: the hand's offset from the position (pixels; dy counts up).
 * angle: flight angle in degrees, 8 fraction bits (0 right, 90 up, 180 left).
 * Any other dir leaves all three as they were. Cases: Magneto_BombOrigin_Case1/2/4/7/8 (3, 5
 * and 6 are unnamed blocks), _Ret. */
#include "xmen2.h"

void Magneto_BombOrigin(char dir, short *dx, short *dy, long *angle)
{
	register char d = dir;
	register short *hy = dy;	/* the hand offset */
	register short *hx = dx;

	switch (d) {
	case 1:			/* Magneto_BombOrigin_Case1 */
		*hx = -0x45;
		*hy = 1;
		*angle = 225L << 8;
		break;
	case 2:			/* Magneto_BombOrigin_Case2 */
		*hx = -0x4D;
		*hy = 0x3B;
		*angle = 180L << 8;
		break;
	case 3:
		*hx = -0x3D;
		*hy = 0x67;
		*angle = 135L << 8;
		break;
	case 4:			/* Magneto_BombOrigin_Case4 */
		*hx = -0x10;
		*hy = 0x79;
		*angle = 90L << 8;
		break;
	case 5:
		*hx = 0x10;
		*hy = 0x79;
		*angle = 90L << 8;
		break;
	case 6:
		*hx = 0x3D;
		*hy = 0x67;
		*angle = 45L << 8;
		break;
	case 7:			/* Magneto_BombOrigin_Case7 */
		*hx = 0x4D;
		*hy = 0x3B;
		*angle = 0;
		break;
	case 8:			/* Magneto_BombOrigin_Case8 */
		*hx = 0x45;
		*hy = 1;
		*angle = 315L << 8;
		break;
	}
	/* Magneto_BombOrigin_Ret */
}
