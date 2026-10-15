/* Math_CosDeg - cosine of an angle in whole degrees, in 16.16 fixed point.
 *
 * angle: degrees, any range.
 * Returns cos(angle) * $10000, as Math_SinDeg(90 - angle) (the original branches into its
 * body). Hand-written 68000. */
#include "xmen2.h"

long Math_CosDeg(short angle)
{
	return Math_SinDeg((short)(90 - angle));
}
