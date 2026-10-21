/* Z80_DriverResume - clear the sound driver's hold flag set by Z80_DriverPause.
 *
 * With interrupts masked and the bus held: Z80 RAM $1B20 = 0. Hand-written 68000. */
#include "xmen2.h"

void Z80_DriverResume(void)
{
	unsigned short sr = plat_sr();

	plat_set_sr(sr | 0x0700);
	Z80_BusRequest();
	Z80_RAM(0x1B20) = 0;
	Z80_BusFree();
	plat_set_sr(sr);
}
