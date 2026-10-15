/* Pad_ReadPort - read the 3- or 6-button joypad on a port with the TH-toggle protocol.
 *
 * port: 0 or 1 (in d0, hand-written 68000); a larger value is returned at once, untouched.
 * Returns the buttons active-high: bits 0-3 Up/Down/Left/Right, 4-7 B/C/A/Start, and for a
 * 6-button pad bits 8-11 Z/Y/X/Mode plus $4000; $8000 when no pad answers. Holds the Z80 bus
 * (and its reset line) meanwhile. TH high reads ?1CBRLDU, TH low ?0SA00DU; the third TH-low
 * read of a 6-button pad has the low nibble clear, and the fourth TH-high read is ?1CBMXYZ.
 * The second pair must repeat the first, and a third pair equal to it means a 3-button pad.
 * The nops between TH writes and reads (settling time) have no C equivalent. */
#include "xmen2.h"

long Pad_ReadPort(short port)
{
	unsigned char r0, r1, r2, r3, r4, r5, r6, r7;
	short buttons;

	if (port > 1)
		return port;
	Z80_BUSREQ = 0x100;
	Z80_RESET = 0x100;
	while (Z80_BUSREQ_B & 1)
		;
	IO_CTRL(port) = 0x40;
	IO_DATA(port) = 0x40;
	r0 = IO_DATA(port);
	if (r0 == 0x70)
		goto no_pad;
	IO_DATA(port) = 0;
	r1 = IO_DATA(port);
	if (r1 == 0x3F)
		goto no_pad;
	IO_DATA(port) = 0x40;
	r2 = IO_DATA(port);
	IO_DATA(port) = 0;
	r3 = IO_DATA(port);
	IO_DATA(port) = 0x40;
	r4 = IO_DATA(port);
	IO_DATA(port) = 0;
	r5 = IO_DATA(port);
	IO_DATA(port) = 0x40;
	r6 = IO_DATA(port);
	IO_DATA(port) = 0;
	r7 = IO_DATA(port);	/* ?0SA1111: nothing new, but the read happens */
	IO_DATA(port) = 0x40;
	if (r2 != r0 || r3 != r1)
		goto no_pad;
	/* active low: Up Down Left Right B C from the TH-high read, A Start from the TH-low one */
	buttons = (r0 & 0x3F) | (r1 & 0x30) << 2;
	if (r4 == r2 && r5 == r3) {
		Z80_BUSREQ = 0;
		return ~buttons & 0xFF;
	}
	if (r5 & 0xF)
		goto no_pad;
	buttons |= (r6 & 0xF) << 8;
	Z80_BUSREQ = 0;
	return ~buttons & 0x4FFF;

no_pad:
	Z80_BUSREQ = 0;
	return 0x8000;
}
