/* BaniMazaA1_InitSprites - set up the Moving Blocks' twelve block sprites: scene sprites 0-11 for
 * Tbl_BaniMazaA1Vdp (frame 0, palette 2) at VRAM tiles g_BgTilesEnd + $10 n, uploaded now (three
 * groups of four: BaniMazaA1_AnimateBlocks, timers 0, 2 and 4, frames 0); palette line 2 from
 * Art_BaniMazaA1Tiles; then the twelve BG blocks after the level's repeat their first row of
 * tiles in all four rows.
 * The block count is set three times (4, 8, 12): only the last one is used. */
#include "xmen2.h"

void BaniMazaA1_InitSprites(void)
{
	register short pal = 2;
	register s16 *dst;
	register s16 *src;
	register long n;
	register long i;
	register long j;
	register s16 *blk;

	if (1) {	/* group 0 (each group a block: MPW pops its four calls' arguments at its end) */
		Spr_Init(&g_SceneSprites[0], 1, CODEPTR(Tbl_BaniMazaA1Vdp), 0, pal, g_BgTilesEnd, 1);
		Spr_Init(&g_SceneSprites[1], 1, CODEPTR(Tbl_BaniMazaA1Vdp), 0, pal, g_BgTilesEnd + 0x10, 1);
		Spr_Init(&g_SceneSprites[2], 1, CODEPTR(Tbl_BaniMazaA1Vdp), 0, pal, g_BgTilesEnd + 0x20, 1);
		Spr_Init(&g_SceneSprites[3], 1, CODEPTR(Tbl_BaniMazaA1Vdp), 0, pal, g_BgTilesEnd + 0x30, 1);
	}
	if (1) {	/* group 1 */
		Spr_Init(&g_SceneSprites[4], 1, CODEPTR(Tbl_BaniMazaA1Vdp), 0, pal, g_BgTilesEnd + 0x40, 1);
		Spr_Init(&g_SceneSprites[5], 1, CODEPTR(Tbl_BaniMazaA1Vdp), 0, pal, g_BgTilesEnd + 0x50, 1);
		Spr_Init(&g_SceneSprites[6], 1, CODEPTR(Tbl_BaniMazaA1Vdp), 0, pal, g_BgTilesEnd + 0x60, 1);
		Spr_Init(&g_SceneSprites[7], 1, CODEPTR(Tbl_BaniMazaA1Vdp), 0, pal, g_BgTilesEnd + 0x70, 1);
	}
	if (1) {	/* group 2 */
		Spr_Init(&g_SceneSprites[8], 1, CODEPTR(Tbl_BaniMazaA1Vdp), 0, pal, g_BgTilesEnd + 0x80, 1);
		Spr_Init(&g_SceneSprites[9], 1, CODEPTR(Tbl_BaniMazaA1Vdp), 0, pal, g_BgTilesEnd + 0x90, 1);
		Spr_Init(&g_SceneSprites[10], 1, CODEPTR(Tbl_BaniMazaA1Vdp), 0, pal, g_BgTilesEnd + 0xA0, 1);
		Spr_Init(&g_SceneSprites[11], 1, CODEPTR(Tbl_BaniMazaA1Vdp), 0, pal, g_BgTilesEnd + 0xB0, 1);
	}
	g_BmA1BlockTimer0 = 0;
	g_BmA1BlockTimer1 = 2;
	g_BmA1BlockTimer2 = 4;
	g_BmA1BlockFrame0 = 0;
	g_BmA1BlockFrame1 = 0;
	g_BmA1BlockFrame2 = 0;
	dst = (s16 *)CODEPTR(Art_BaniMazaA1Tiles);
	src = dst;
	dst = &g_PalBase[pal << 4];
	for (i = 0; i < 16; i++)
		*dst++ = *src++;
	if (1)		/* (blocks: the ROM keeps the overwritten stores) */
		n = 4;
	if (1)
		n = 8;
	n = 12;
	for (i = 1; i <= n; i++) {
		blk = (s16 *)g_BgBlockDefs + ((g_BgBlockCount + i - 1) << 4);
		for (j = 0; j < 4; j++) {
			*(blk + 4 + j) = *(blk + j);
			*(blk + 8 + j) = *(blk + j);
			*(blk + 12 + j) = *(blk + j);
		}
	}
}
