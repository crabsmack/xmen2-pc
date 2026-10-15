/* Magneto_ShotOriginCrouch - MAGNETO: where a shot fired crouching starts and how it flies.
 *
 * dir:   aim direction; crouching only 2 (left) and 7 (right) are set.
 * dx/dy: the hand's offset from the position (pixels; dy counts up).
 * angle: flight angle in degrees, 8 fraction bits (180 left, 0 right).
 * type:  hit type of the shot (9 thrown left, 10 right: MagnetoBoss_TakeDamage pushes by it).
 * Any other dir leaves all four as they were. */
#include "xmen2.h"

void Magneto_ShotOriginCrouch(char dir, register short *dx, register short *dy,
	long *angle, char *type)
{
	register char d = dir;

	switch (d) {
	case 2:
		*dx = -0x59;
		*dy = 0x2E;
		*angle = 180L << 8;
		*type = 9;
		break;
	case 7:
		*dx = 0x59;
		*dy = 0x2E;
		*angle = 0;
		*type = 10;
		break;
	}
}
