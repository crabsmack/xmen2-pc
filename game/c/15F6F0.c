/* Eni_ReadInline - Enigma: read an inline tile word from the bit stream.
 *
 * src, bits, nbits: the bit stream: the next byte, the buffer (a long) and the number of its
 * low bits still unread; all three move on.  width: the bits of a tile number (the Enigma
 * header's first byte).  flags: the current flag bits (bits 11-15: priority, palette, flips).
 * base: added to every tile word (Eni_Decode's third argument).
 * shift: receives the bit position the number was read from (d7, which nobody reads: here
 * because bridge.py's clobbers() can't name d7).
 * Returns the width-bit number + flags + base. The buffer is first topped up a byte at a time
 * until it holds width bits, and refilled by a byte after the read when fewer than 9 are left.
 * Hand-written 68000 called by Eni_Ops with registers. The mask comes from a word table at
 * $15F720 holding (1 << n) - 1 for n = 1-16, which covers the widths of the game's maps and
 * block tables (a width of 0 would read the rts before the table). */
#include "xmen2.h"

unsigned short Eni_ReadInline(u8 **src, unsigned long *bits, short *nbits, short width, short flags,
			      short base, short *shift)
{
	unsigned short v;

	while (*nbits < width) {
		*nbits += 8;
		*bits = *bits << 8 | *(*src)++;
	}
	*shift = *nbits - width;
	v = (unsigned short)(*bits >> *shift);
	*nbits -= width;
	if (*nbits < 9) {
		*nbits += 8;
		*bits = *bits << 8 | *(*src)++;
	}
	return (unsigned short)((v & ((1 << width) - 1)) + flags + base);
}
