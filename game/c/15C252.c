/* Math_LModS_68000 - the 68000 path of Math_LModS: n % d, with the sign of n.
 *
 * n, d: dividend and divisor, in d0 and d1. Returns the remainder (a bra to Math_DivModS,
 * which leaves it in d0). */
#include "xmen2.h"

long Math_LModS_68000(long n, long d)
{
	long q;

	return Math_DivModS(n, d, &q);
}
