/* unk_Boot_005458 - set g_PadLayout to 2 (Boot_StartApp, once at boot): the row of
 * Pad_ConfigButtonMask's 8-byte table it uses. What the rows stand for is not known. */
#include "xmen2.h"

void unk_Boot_005458(void)
{
	g_PadLayout = 2;
}
