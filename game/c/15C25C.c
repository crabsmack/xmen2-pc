/* Math_DivModS - signed long division with remainder, as C rounds (toward zero).
 *
 * n, d: dividend and divisor, in d0 and d1.  q: receives the quotient (d1 on return).
 * Returns the remainder (d0), which has the sign of n. Divides the magnitudes with
 * Math_DivModU and negates the results the signs call for. */
#include "xmen2.h"

long Math_DivModS(long n, long d, long *q)
{
	unsigned long uq, ur;

	if (n >= 0) {
		if (d >= 0) {
			ur = Math_DivModU(n, d, &uq);
			*q = uq;
			return ur;
		}
		ur = Math_DivModU(n, -(unsigned long)d, &uq);
		*q = -uq;
		return ur;
	}
	if (d >= 0) {
		ur = Math_DivModU(-(unsigned long)n, d, &uq);
		*q = -uq;
		return -ur;
	}
	ur = Math_DivModU(-(unsigned long)n, -(unsigned long)d, &uq);
	*q = uq;
	return -ur;
}
