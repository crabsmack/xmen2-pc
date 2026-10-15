/* Eni_Ops - the Enigma decoder's commands: run the command Eni_Decode read, then read and run
 * the following ones until the end mark.
 *
 * op, count: the command: its top 3 bits, and its low 4 (count + 1 words).
 * src, bits, nbits: the bit stream: the next byte, the buffer (a long) and the number of its
 * low bits still unread.  dest: the output words.  inc, common: the incrementing and the common
 * word from the Enigma header.  flags: the flag bits (11-15: priority, palette, flips) added to
 * inline words.  base: added to every inline word.  width: the bits of an inline tile number.
 * Commands (the ones starting %0 are 6 bits long, the others 7):
 *   %00 n     n + 1 copies of the incrementing word, which goes up by one after each
 *   %01 n     n + 1 copies of the common word
 *   %100 n    an inline word (Eni_ReadInline), n + 1 times
 *   %101 n    an inline word, n + 1 times, going up by one; %110 n: going down by one
 *   %111 n    n + 1 inline words (n < 14)
 *   %1111110  5 bits: new flag bits, which also replace the flags of the incrementing and the
 *             common word
 *   %1111111  the end.
 * Hand-written 68000 entered by Eni_Decode's computed jump into the branch table at its start
 * (the entry for op; game/runtime/asm_conv.cpp adapts it). Each command's code branches back
 * to Eni_Decode's command loop ($15F628), which reads the next command and jumps here again:
 * that loop is the tail of this one in C. The end mark returns from Eni_Decode itself. The
 * refill after the flag bits shifts only the buffer's low word (the others shift the long):
 * the high word keeps stale bits, below the ones any later read reaches. */
#include "xmen2.h"

void Eni_Ops(short op, short count, u8 *src, unsigned long bits, short nbits, u16 *dest, short inc,
	     short common, short flags, short base, short width)
{
	unsigned short cmd;
	unsigned short v;
	short shift;	/* Eni_ReadInline's bit position: unused */

	for (;;) {
		switch (op) {
		case 0:		/* %00 */
		case 1:
			do
				*dest++ = inc++;
			while (count-- != 0);
			break;
		case 2:		/* %01 */
		case 3:
			do
				*dest++ = common;
			while (count-- != 0);
			break;
		case 4:		/* %100 */
			v = Eni_ReadInline(&src, &bits, &nbits, width, flags, base, &shift);
			do
				*dest++ = v;
			while (count-- != 0);
			break;
		case 5:		/* %101 */
			v = Eni_ReadInline(&src, &bits, &nbits, width, flags, base, &shift);
			do
				*dest++ = v++;
			while (count-- != 0);
			break;
		case 6:		/* %110 */
			v = Eni_ReadInline(&src, &bits, &nbits, width, flags, base, &shift);
			do
				*dest++ = v--;
			while (count-- != 0);
			break;
		case 7:		/* %111 */
			if (count == 14) {
				nbits -= 5;
				flags = (short)(((unsigned short)bits >> nbits) << 11);
				if (nbits < 9) {
					nbits += 8;
					bits = (bits & 0xFFFF0000) | (unsigned short)((unsigned short)bits << 8 | *src++);
				}
				inc = (short)((inc & 0x7FF) + flags);
				common = (short)((common & 0x7FF) + flags);
			} else if (count == 15) {
				return;
			} else {
				do
					*dest++ = Eni_ReadInline(&src, &bits, &nbits, width, flags, base, &shift);
				while (count-- != 0);
			}
			break;
		}
		/* the next command (Eni_Decode, $15F628): 7 bits peeked */
		cmd = (unsigned short)bits >> (nbits - 7) & 0x7F;
		if (cmd >= 0x40) {
			nbits -= 7;
			count = cmd & 0xF;
		} else {
			nbits -= 6;
			count = cmd >> 1 & 0xF;
		}
		if (nbits < 9) {
			nbits += 8;
			bits = bits << 8 | *src++;
		}
		op = cmd >> 4;
	}
}
