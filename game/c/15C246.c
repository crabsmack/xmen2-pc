/* Math_LModS - long remainder n % d, with the sign of n (MPW C's runtime for long %).
 *
 * n, d: dividend and divisor, in d0 and d1 (a register convention). Returns the remainder.
 * Hand-written 68000: the same CPU-path jump as Math_LDivS, here to Math_LModS_68000. */
#include "xmen2.h"

long Math_LModS(long n, long d)
{
	return Math_LModS_68000(n, d);
}
