/* Z80_BusAcquire - stop the Z80 to write its sound command ring: mask interrupts, take the bus.
 *
 * sr: receives SR as it was (Z80_BusRelease puts it back).
 * Returns the ring's write index (Z80 RAM $36, sign-extended), where the next command byte
 * goes (Snd_PutCmd, Snd_PutByte). Hand-written 68000 with a convention of its own: it builds a
 * frame in its caller (saved d1/a1 and SR) and hands back a0/a1 = the index cell and the ring;
 * the Snd_* functions end with a jmp to Z80_BusRelease, which unwinds it
 * (game/runtime/asm_conv.cpp adapts it). */
#include "xmen2.h"

short Z80_BusAcquire(unsigned short *sr)
{
	*sr = plat_sr();
	plat_set_sr(*sr | 0x0700);
	Z80_BUSREQ = 0x100;
	while (Z80_BUSREQ_B & 1)
		;
	return (signed char)Z80_RAM(0x36);
}
