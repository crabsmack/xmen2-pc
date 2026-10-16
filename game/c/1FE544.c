/* Snd_Init - start the sound driver: load it into the Z80 and run it (Z80_LoadDriver,
 * Z80_ResetRun), then send it $FF, $0B and the 68000 addresses (24 bits each) of its four data
 * blocks.
 *
 * a, b, c, d: the blocks' bus addresses (Game_Init passes Snd_1618BC .. Snd_164xxx). */
#include "xmen2.h"

void Snd_Init(long a, long b, long c, long d)
{
	Z80_LoadDriver();
	Z80_ResetRun();
	Snd_SendByte(-1);
	Snd_SendByte(0xB);
	Snd_SendAddr24(a);
	Snd_SendAddr24(b);
	Snd_SendAddr24(c);
	Snd_SendAddr24(d);
}
