/* Math_LDivS - long division n / d, rounded toward zero (MPW C's runtime for long /).
 *
 * n, d: dividend and divisor, in d0 and d1 (a register convention). Returns the quotient.
 * Hand-written 68000: it jumps through a CPU-path selector (Math_LDivS_Sel) that a 68020 would
 * take into an embedded divs.l; this build always takes the 68000 path. */
#include "xmen2.h"

long Math_LDivS(long n, long d)
{
	return Math_LDivS_Sel(n, d);
}
