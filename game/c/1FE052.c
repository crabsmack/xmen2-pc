/* Pad_IsP2Connected - whether a pad is plugged into port 2.
 *
 * Returns $FF when g_Pad2Raw's bit 15 is clear (Pad_ReadPort gives $8000 for no pad), else 0.
 * Hand-written 68000 (moveq #0 / tst.w / spl.b). */
#include "xmen2.h"

short Pad_IsP2Connected(void)
{
	if (g_Pad2Raw >= 0)
		return 0xFF;
	return 0;
}
