/* Math_LDivS_Sel - Math_LDivS's CPU-path selector: the 68000 path, n / d toward zero.
 *
 * n, d: dividend and divisor, in d0 and d1. Returns the quotient.
 * The selector is a bra to the 68000 path (Math_LDivS_68000: Math_DivModS, then the quotient
 * from d1 to d0); the 68020 divs.l after it is never reached. */
#include "xmen2.h"

long Math_LDivS_Sel(long n, long d)
{
	long q;

	/* Math_LDivS_68000 */
	Math_DivModS(n, d, &q);
	return q;
}
