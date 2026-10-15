/* Z80_BusRequest - request the Z80 bus and wait until it is granted (SR untouched).
 *
 * Hand-written 68000. */
#include "xmen2.h"

void Z80_BusRequest(void)
{
	Z80_BUSREQ = 0x100;
	while (Z80_BUSREQ_B & 1)
		;
}
