/* Pad_GetP2Clean - pad 2's raw buttons with the impossible Up+Down and Left+Right pairs cleared.
 *
 * As Pad_GetP1Clean, on g_Pad2Raw. */
#include "xmen2.h"

short Pad_GetP2Clean(void)
{
	short b = g_Pad2Raw;

	if ((b & 3) == 3)
		b ^= 3;
	if ((b & 0xC) == 0xC)
		b ^= 0xC;
	return b;
}
