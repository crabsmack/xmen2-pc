/* Z80_BusRelease - give the Z80 its bus back and restore SR, after Z80_BusAcquire.
 *
 * sr: SR as Z80_BusAcquire found it.
 * Hand-written 68000 reached by jmp: it also pops the frame Z80_BusAcquire built and returns
 * from the Snd_* function (game/runtime/asm_conv.cpp adapts it). */
#include "xmen2.h"

void Z80_BusRelease(unsigned short sr)
{
	Z80_BUSREQ = 0;
	plat_set_sr(sr);
}
