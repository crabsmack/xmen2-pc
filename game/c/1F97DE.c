/* Pad_P1StartDown - is Start held on pad 1 right now? A fresh read of the pad (Pad_GetP1Clean,
 * with the Z80 off the bus).
 *
 * Returns $80 (the Start bit) when it is held, else 0. */
#include "xmen2.h"

long Pad_P1StartDown(void)
{
	register long pad;

	Z80_BusRequest();
	pad = Pad_GetP1Clean();
	Z80_BusFree();
	return pad & 0x80;
}
