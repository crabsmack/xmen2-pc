/* TechWallGun_AimAngle - the direction of (dx, dy) to a sixteenth of a turn, from comparisons
 * only (dy against dx, 2 dx; 2 dy against dx): the angle from the +x axis (positive downward,
 * y points down) of the middle of its sixteenth, in quarter degrees.
 *
 * dx, dy: the vector (pixels).
 * Returns 45, 135, 225, 315 (11.25 .. 78.75 degrees) for dx, dy >= 0; 405 .. 675 for dx < 0,
 * dy >= 0; the negatives for dy < 0. The callers round it to degrees ((a + 2) >> 2). */
#include "xmen2.h"

short TechWallGun_AimAngle(short dx, short dy)
{
	if (dx >= 0) {
		if (dy >= 0) {
			if (dy >= dx) {
				if (dy >= dx + dx)
					return 0x13B;
				else
					return 0xE1;
			} else {
				if (dy + dy >= dx)
					return 0x87;
				else
					return 0x2D;
			}
		} else {
			if (dy >= -dx) {
				if (dy + dy >= -dx)
					return -0x2D;
				else
					return -0x87;
			} else {
				if (dy > -dx - dx)
					return -0xE1;
				else
					return -0x13B;
			}
		}
	} else {
		if (dy >= 0) {
			if (dy > -dx) {
				if (dy > -dx - dx)
					return 0x195;
				else
					return 0x1EF;
			} else {
				if (dy + dy > -dx)
					return 0x249;
				else
					return 0x2A3;
			}
		} else {
			if (dy > dx) {
				if (dy + dy > dx)
					return -0x2A3;
				else
					return -0x249;
			} else {
				if (dy > dx + dx)
					return -0x1EF;
				else
					return -0x195;
			}
		}
	}
}
