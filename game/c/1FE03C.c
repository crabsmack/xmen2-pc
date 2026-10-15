/* Pad_ReadBoth - read both joypads into g_Pad1Raw and g_Pad2Raw (V-int, every frame).
 *
 * Hand-written 68000. */
#include "xmen2.h"

void Pad_ReadBoth(void)
{
	g_Pad1Raw = (short)Pad_ReadPort(0);
	g_Pad2Raw = (short)Pad_ReadPort(1);
}
