/* Math_SinDeg - sine of an angle in whole degrees, in 16.16 fixed point.
 *
 * angle: degrees, any range (wrapped into 0..359).
 * Returns sin(angle) * $10000: exact at 0, 90, 180 and 270, otherwise from the 90-entry
 * quarter-wave table Tbl_SineQuarter folded into the right quadrant. Hand-written 68000;
 * Math_CosDeg branches into it with 90 - angle. */
#include "xmen2.h"

long Math_SinDeg(short angle)
{
	u16 *quarter = (u16 *)Tbl_SineQuarter;
	short a = angle;

	if (a < 0) {
		do
			a += 360;
		while (a < 0);
	}
	if (a >= 360) {
		do
			a -= 360;
		while (a >= 0);
		a += 360;
	}
	if (a == 0 || a == 180)
		return 0;
	if (a == 90)
		return 0x10000;
	if (a == 270)
		return -0x10000;
	if (a < 90)
		return quarter[a];
	if (a < 180)
		return quarter[180 - a];
	if (a < 270)
		return -(long)quarter[a - 180];
	return -(long)quarter[360 - a];
}
