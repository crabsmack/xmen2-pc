/* BaniMazaA1_UploadBlockTiles - queue the tiles of one animation frame of a group of four block
 * sprites: frame n of Art_BaniMazaA1Tiles2 ($200 bytes: 4 sprites x 4 tiles), each sprite's
 * four tiles to its own VRAM tiles (tileBase + 0, 4, 8, 12: VDP_QueueDma, 32 bytes each).
 *
 * slot: the group's first scene sprite.  frame: 0-7. */
#include "xmen2.h"

void BaniMazaA1_UploadBlockTiles(register SprInst *slot, short frame)
{
	register u8 *src = (u8 *)CODEPTR(Art_BaniMazaA1Tiles2) + frame * 0x200;
	register long n;
	register long tile;

	for (n = 4; n; n--, slot++) {
		tile = slot->tileBase;
		VDP_QueueDma(src, tile << 5, 0x20);
		tile += 4;
		src += 0x20;
		VDP_QueueDma(src, tile << 5, 0x20);
		tile += 4;
		src += 0x20;
		VDP_QueueDma(src, tile << 5, 0x20);
		tile += 4;
		src += 0x20;
		VDP_QueueDma(src, tile << 5, 0x20);
		tile += 4;
		src += 0x20;
	}
}
