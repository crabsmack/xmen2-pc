/* Eni_Decode - decompress an Enigma tilemap (Sega's plane-map compression) into tile words
 * (Level_LoadMaps: the block definitions and maps).
 *
 * src:  the compressed data: a header {byte: bits of an inline tile number; byte: which flag bits
 *       inline words carry (not read: none until a %1111110 command sets them); word: the
 *       incrementing word; word: the common word}, then the bit stream of commands (Eni_Ops).
 * dest: the output words.
 * base: added to every inline word (its low word; base tile and palette).
 * Hand-written 68000 with a C frame: it reads the header, primes the bit buffer with 16 bits and
 * the first command (7 bits peeked: one starting %0 is 6 bits, its count bits 1-4), then jumps
 * into Eni_Ops' branch table for the command, which loops over the rest and returns from here. */
#include "xmen2.h"

void Eni_Decode(u8 *src, u16 *dest, long base)
{
	short width;
	short inc;
	short common;
	unsigned long bits;
	short nbits;
	unsigned short cmd;
	short count;

	width = (s8)*src++;
	src++;				/* the flag-bit mask: read and dropped */
	inc = *(s16 *)src;
	src += 2;
	common = *(s16 *)src;
	src += 2;
	bits = (unsigned long)src[0] << 8 | src[1];
	src += 2;
	nbits = 16;
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
	Eni_Ops(cmd >> 4, count, src, bits, nbits, dest, inc, common, 0, (short)base, width);
}
