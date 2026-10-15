/* VDP_ClearCram - zero all 64 palette entries in CRAM.
 *
 * Hand-written 68000: command $C0000000 (CRAM write at 0), then 64 zero words. Called once, by
 * Sys_InitHardware. */
#include "xmen2.h"

void VDP_ClearCram(void)
{
	short i;

	VDP_CTRL32 = 0xC0000000;
	for (i = 0; i < 64; i++)
		VDP_DATA = 0;
}
