/* Math_LMul - long multiply: the low 32 bits of a * b (MPW C's runtime for long *).
 *
 * a, b: the factors, in d0 and d1 (a register convention, not MPW's slots).
 * Hand-written 68000 with three mulu.w partial products (a.lo*b.lo, a.hi*b.lo, b.hi*a.lo; the
 * high*high one can't reach the low 32 bits). Signed or unsigned, the low 32 bits are the same. */
#include "xmen2.h"

long Math_LMul(long a, long b)
{
	return (long)((unsigned long)a * (unsigned long)b);
}
