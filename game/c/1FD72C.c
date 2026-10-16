/* Art_LoadNemToVram - decode a Nemesis art blob (type 4) to VRAM at a tile.
 *
 * art: the blob {word type 4, word rows, the code list, the packed data}.  tile: the first VRAM
 * tile. Other types do nothing. The code table (Nem_BuildCodeTable) is a 512-byte local. */
#include "xmen2.h"

void Art_LoadNemToVram(void *art, short tile)
{
	s16 table[256];
	register s16 *p = (s16 *)art;
	register short n = *p;

	switch (n) {
	case 4:
		n = p[1];
		p = (s16 *)Nem_BuildCodeTable(p + 2, (NemCodeTable *)table);
		Nem_Decode(p, (NemCodeTable *)table, n, 1, tile << 5);
		break;
	}
}
