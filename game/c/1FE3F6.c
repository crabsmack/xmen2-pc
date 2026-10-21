/* Z80_DriverPause - ask the Z80 sound driver to hold off, and wait until it does (before DMA).
 *
 * With interrupts masked: take the bus, set the hold flag (Z80 RAM $1B20), read the driver's
 * busy flag ($1B21), give the bus back; while the driver was busy, wait a moment (a $44-step
 * dbra, timing only) and ask again. SR is restored at the end. Hand-written 68000. */
#include "xmen2.h"

void Z80_DriverPause(void)
{
	unsigned short sr = plat_sr();
	unsigned char busy;

	plat_set_sr(sr | 0x0700);
	for (;;) {
		Z80_BUSREQ = 0x100;
		while (Z80_BUSREQ_B & 1)
			;
		Z80_RAM(0x1B20) = 1;
		busy = Z80_RAM(0x1B21);
		Z80_BUSREQ = 0;
		if (busy == 0)
			break;
	}
	plat_set_sr(sr);
}
