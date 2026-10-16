/* Pad_ConfigButtonMask - the pad button(s) that do action n in the current control layout: entry
 * [g_PadLayout][n] of Tbl_ButtonConfigs (8 signed bytes per layout).
 *
 * n: the action, 0-7.
 * Returns the mask (Game_Main fills g_ActionButtonMasks with it). */
#include "xmen2.h"

short Pad_ConfigButtonMask(short n)
{
	return *((s8 *)CODEPTR(Tbl_ButtonConfigs) + n + (g_PadLayout << 3));
}
