/* Nem_DecodeLoop - the Nemesis decoder proper: turn codes from the bit stream into runs of
 * pixels, build 8-pixel rows from them and hand each full row to the writer.
 *
 * src: the compressed data, from its third byte (the first two are in bits).
 * codes: Nem_BuildCodeTable's table: for each 8-bit peek at the stream, the length of the code
 * it starts with << 8 | the code's value.  rows: rows to decode (8 per tile).
 * mode, dest, mask: the writer, as Nem_Decode chose it: 0 Nem_PutRowRam (rows to dest),
 * 1 Nem_PutRowVdp, 2 Nem_PutRowMasked (only the tiles mask selects).
 * bits, nbits: the bit buffer; its low nbits bits are unread (Nem_Decode starts with 16).
 * nibbles, row: the row being built: pixels still free, the pixels so far (8, 0 at the start).
 * fill: the pixel pattern register (d7): in, and out as the last coloured run left it.
 * A value is (count - 1) << 4 | colour: count pixels of that colour, filled into the row from
 * its left (high) end; a run may complete the row and go on in the next one. Codes from $FC on
 * are an escape: 6 bits, then the value itself in 8. The buffer is refilled a byte at a time
 * when fewer than 9 bits are left, so the decoder reads ahead of what it uses.
 * Hand-written 68000 with all of this in registers (game/runtime/asm_conv.cpp adapts it): the
 * writer is the code address Nem_Decode put in a3 (Nem_PutRowMasked changes it to choose
 * between writing and skipping a tile), and on the last row the writer returns past this loop,
 * straight to Nem_Decode; here the writers return 1 then. The original leaves the pattern of
 * the last run in d7 for nobody: fill hands it back because bridge.py's clobbers() can't name
 * d7. */
#include "xmen2.h"

/* jsr (a3): the row goes to the writer; 1 when it was the last */
#define PUT_ROW()	(mode == 0 ? Nem_PutRowRam(row, &rows, &dest) \
			 : mode == 1 ? Nem_PutRowVdp(row, &rows) \
			 : Nem_PutRowMasked(row, &rows, &skip, mask))

void Nem_DecodeLoop(u8 *src, u16 *codes, short rows, char mode, u32 *dest, u8 *mask,
		    unsigned short bits, short nbits, short nibbles, unsigned long row, unsigned long *fill)
{
	unsigned short code;
	unsigned short v;	/* the run: (count - 1) << 4 | colour */
	short colour;
	short count;
	unsigned long m;
	char skip = 0;		/* Nem_PutRowMasked: this tile is left out */

	for (;;) {
		code = bits >> (nbits - 8) & 0xFF;
		if (code >= 0xFC) {
			/* Nem_DecodeLoop_Inline: the escape, then the value inline */
			nbits -= 6;
			if (nbits < 9) {
				nbits += 8;
				bits = (unsigned short)(bits << 8 | *src++);
			}
			nbits -= 8;
			v = bits >> nbits;
			if (nbits < 9) {
				nbits += 8;
				bits = (unsigned short)(bits << 8 | *src++);
			}
		} else {
			nbits -= codes[code] >> 8;
			if (nbits < 9) {
				nbits += 8;
				bits = (unsigned short)(bits << 8 | *src++);
			}
			v = codes[code] & 0xFF;
		}
		colour = v & 0xF;
		count = (v >> 4 & 0xF) + 1;
		if (colour == 0) {
			/* the row starts clear: only move along it (Nem_DecodeLoop_Row) */
			while (count >= nibbles) {
				if (PUT_ROW())
					return;
				count -= nibbles;
				row = 0;
				nibbles = 8;
			}
			nibbles -= count;
		} else if (count == 1) {
			/* one pixel (Nem_DecodeLoop_Row) */
			*fill = ((u32 *)Tbl_NibbleFill)[colour];
			row |= ((u32 *)Tbl_NibbleMaskAt)[nibbles] & *fill;
			if (--nibbles == 0) {
				if (PUT_ROW())
					return;
				row = 0;
				nibbles = 8;
			}
		} else {
			*fill = ((u32 *)Tbl_NibbleFill)[colour];
			for (;;) {
				m = ((u32 *)Tbl_NibbleMaskLow)[nibbles];	/* the pixels still free */
				if (count < nibbles) {
					nibbles -= count;
					row |= (m - ((u32 *)Tbl_NibbleMaskLow)[nibbles]) & *fill;
					break;
				}
				/* the run fills the row (Nem_DecodeLoop_Row) */
				row |= m & *fill;
				if (PUT_ROW())
					return;
				count -= nibbles;
				row = 0;
				nibbles = 8;
				if (count == 0)
					break;
			}
		}
	}
}
