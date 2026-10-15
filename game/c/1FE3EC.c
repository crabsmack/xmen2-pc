/* Z80_BusFree - give the bus back so the Z80 runs again.
 *
 * Hand-written 68000. */
#include "xmen2.h"

void Z80_BusFree(void)
{
	Z80_BUSREQ = 0;
}
