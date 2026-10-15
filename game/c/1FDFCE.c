/* VDP_ClearVsram - zero the 40 words of VSRAM.
 *
 * Hand-written 68000: command $40000010 (VSRAM write at 0), then 40 zero words. */
#include "xmen2.h"

void VDP_ClearVsram(void)
{
	short i;

	VDP_CTRL32 = 0x40000010;
	for (i = 0; i < 40; i++)
		VDP_DATA = 0;
}
