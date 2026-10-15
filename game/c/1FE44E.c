/* Z80_LoadDriver - hold the Z80 in reset, copy the sound driver into Z80 RAM, zero the rest.
 *
 * With interrupts masked: reset line held ($A11200 = $100), bus taken; Snd_Z80Driver (ROM, up
 * to Snd_Z80DriverEnd) is copied to Z80 RAM from 0 and the rest of the 8 KB is zeroed. SR is
 * restored; Z80_ResetRun starts the driver. Hand-written 68000. */
#include "xmen2.h"

void Z80_LoadDriver(void)
{
	unsigned short sr = CPU_GetSR();
	u8 *src = (u8 *)Snd_Z80Driver;
	unsigned short n = (unsigned short)((u8 *)Snd_Z80DriverEnd - src);
	unsigned short at = 0;

	CPU_SetSR(sr | 0x0700);
	Z80_RESET = 0x100;
	Z80_BusRequest();
	while (n) {
		Z80_RAM(at++) = *src++;
		n--;
	}
	while (at != 0x2000)
		Z80_RAM(at++) = 0;
	CPU_SetSR(sr);
}
