/* Math_MulFix8 - a * b without its 8 fraction bits: the unsigned 32-bit product of two words,
 * shifted right 8.
 *
 * a, b: words, taken unsigned.
 * Returns (a * b) >> 8 (Scroll_SetParallax scales its band factors with it). */
#include "xmen2.h"

unsigned long Math_MulFix8(unsigned short a, unsigned short b)
{
	return (unsigned long)a * b >> 8;
}
