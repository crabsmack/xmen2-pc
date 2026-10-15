/* Math_DivModU - unsigned long division with remainder.
 *
 * n, d: dividend and divisor, in d0 and d1.  q: receives the quotient (d1 on return).
 * Returns the remainder (d0). A divisor under $10000 takes two divu.w steps (high word, then
 * the remainder with the low word): exact, and a 0 divisor traps as divu does. A larger one
 * takes 16 shift-subtract steps on a 32-bit remainder, which loses its top bit when the
 * divisor is over $80000000 (a bug signed division can't reach). */
#include "xmen2.h"

unsigned long Math_DivModU(unsigned long n, unsigned long d, unsigned long *q)
{
	unsigned long rem, quot;
	unsigned short lo;
	short i;

	if (d < 0x10000) {
		*q = n / d;
		return n % d;
	}
	rem = n >> 16;
	if (rem == 0) {
		*q = 0;
		return n;
	}
	lo = (unsigned short)n;
	quot = 0;
	for (i = 0; i < 16; i++) {
		rem = rem << 1 | lo >> 15;	/* addx.l: a carry out of bit 31 is lost */
		lo <<= 1;
		quot <<= 1;
		if (rem >= d) {
			rem -= d;
			quot |= 1;
		}
	}
	*q = quot;
	return rem;
}
