/* Magneto_BombOriginCrouch - MAGNETO: where a flux bomb thrown crouching starts and its angle.
 *
 * dir:   aim direction; crouching only 2 (left) and 7 (right) are set.
 * dx/dy: the hand's offset from the position (pixels; dy counts up).
 * angle: flight angle in degrees, 8 fraction bits (180 left, 0 right).
 * Any other dir leaves all three as they were. */
#include "xmen2.h"

void Magneto_BombOriginCrouch(char dir, register short *dx, register short *dy, long *angle)
{
	register char d = dir;

	switch (d) {
	case 2:
		*dx = -0x4C;
		*dy = 0x2C;
		*angle = 180L << 8;
		break;
	case 7:
		*dx = 0x4C;
		*dy = 0x2C;
		*angle = 0;
		break;
	}
}
