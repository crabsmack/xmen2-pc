/* Art_LoadToVramRecolor - load a tile art blob to VRAM at a tile: raw tiles (type 1) as they are,
 * Nemesis-packed ones (type 4) with colours 0, $E and $F changed to c0, cE and cF.
 *
 * art: the blob {word type, ...: type 1 word +6 = tiles, the tiles from +8; type 4 word +2 =
 * rows, the code list from +4}.  tile: the first VRAM tile.  c0, cE, cF: the colours that
 * replace 0, $E and $F (Cerebro_LoadFonts makes the font's two colours other palette entries).
 * The recolouring changes the low nibble of each value in the decoder's code table; the decode
 * (Nem_Decode to VRAM) runs with interrupts off. Other types do nothing. */
#include "xmen2.h"

#define VRAM_WRITE(a)	((((a) & 0x3FFF) << 16) + 0x40000000 + ((unsigned long)((a) & 0xC000) >> 14))

void Art_LoadToVramRecolor(register void *art, short tile, register short c0, register short cE,
	short cF)
{
	NemCodeTable table;
	short rows;
	register s16 *hdr = (s16 *)art;
	register short type = *hdr;
	register short i;

	switch (type) {
	case 1:
		type = ((s16 *)art)[3];
		VDP_CTRL32 = VRAM_WRITE((long)tile << 5);
		VDP_WriteWords((char *)art + 8, (long)type << 4);
		break;
	case 4:
		rows = hdr[1];
		hdr = (s16 *)Nem_BuildCodeTable(hdr + 2, &table);
		{
			register s16 *p = table.code;

			for (i = 0; i < 0x100; p++, i++) {
				type = *p & 0xF;
				switch (type) {
				case 0:
					*p = *p & 0xFFF0 | c0;
					break;
				case 0xE:
					*p = *p & 0xFFF0 | cE;
					break;
				case 0xF:
					*p = *p & 0xFFF0 | cF;
					break;
				}
			}
		}
		Irq_Disable();
		Nem_Decode(hdr, &table, rows, 1, tile << 5);
		Irq_Enable();
		break;
	}
}
