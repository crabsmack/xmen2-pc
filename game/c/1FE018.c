/* Pad_GetP1Clean - pad 1's raw buttons with the impossible Up+Down and Left+Right pairs cleared.
 *
 * Returns g_Pad1Raw, where both of bits 0-1 (Up, Down) set clears them, and the same for bits
 * 2-3 (Left, Right). Hand-written 68000; it shares its tail with Pad_GetP2Clean. */
#include "xmen2.h"

short Pad_GetP1Clean(void)
{
	short b = g_Pad1Raw;

	if ((b & 3) == 3)
		b ^= 3;
	if ((b & 0xC) == 0xC)
		b ^= 0xC;
	return b;
}
