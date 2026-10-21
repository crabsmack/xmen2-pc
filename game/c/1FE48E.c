/* Z80_ResetRun - pulse the Z80's reset and give it the bus, so the loaded driver starts.
 *
 * With interrupts masked: reset low, a short wait (15 steps, timing only), bus released,
 * reset high; SR restored. Hand-written 68000. */
#include "xmen2.h"

void Z80_ResetRun(void)
{
	unsigned short sr = plat_sr();

	plat_set_sr(sr | 0x0700);
	Z80_RESET = 0;
	Z80_BUSREQ = 0;
	Z80_RESET = 0x100;
	plat_set_sr(sr);
}
