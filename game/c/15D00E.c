/* Map_RedrawBlock - draw map block (col, row), 32 x 32 px = 4 x 4 tiles, again into its plane's
 * name table in VRAM (after the map changed: Map_SetBlockFG...), if it is near the screen.
 *
 * col, row: the block in the map (FG map, plane A at $E000; or BG map, plane B at 0).
 * bg:       1: the BG map, 0: the FG map.
 * Nothing is drawn for a block more than the streaming margin (32 or 192 px, the narrow flag of
 * the map) left or right of the screen, or more than 32 px above it or below its 256 lines; of the
 * block's tiles only those inside the rows and columns the streamer keeps in VRAM (32 rows, 64
 * columns from g_*StreamY/X) are written. The block's flips (map bits $200, $400) mirror its 4 x 4
 * tile words and flip each tile; its priority bit ($8000) and the map's palette bits are xored in.
 * Each tile word is one VDP write with interrupts off. */
#include "xmen2.h"

/* How far left and right of the screen a block is still drawn: what the streamer keeps. */
#if defined(XMEN2_NATIVE)	/* phase 6 R7: with the 16:9 view the streamer keeps the whole plane */
#define MARGIN(narrow)	((narrow) && !WS ? 0x20 : 0xC0)
#else
#define MARGIN(narrow)	((narrow) ? 0x20 : 0xC0)
#endif
/* MIN of the block's last tile (unsigned: bx, by are) and the streamed window's: MPW compares
   them unsigned (shi), so a window edge below 0 never wins; PC compares the same way. */
#ifdef XMEN2_MPW
#define UMIN(a, b)	MIN(a, b)
#else
#define UMIN(a, b)	((unsigned long)(a) < (unsigned long)(b) ? (a) : (b))
#endif
/* The VDP command writing VRAM at a. */
#define VRAM_WRITE(a)	((((a) & 0x3FFF) << 16) + 0x40000000 + (((a) & 0xC000) >> 14))

void Map_RedrawBlock(register short col, register short row, char bg)
{
	short dc;
	short dr;
	unsigned short bx;
	unsigned short by;
	unsigned short vr;
	unsigned short leftT;
	unsigned short rightT;
	unsigned short ty;
	unsigned short top;
	unsigned short bottom;
	unsigned short flips;
	unsigned short prio;
	unsigned short pal;
	unsigned short xorv;
	unsigned short blk;
	unsigned short attr;
	short vram;
	short scrollX;
	short scrollY;
	short left;
	register short right;
	register unsigned short cell;
	register u16 *defs;
	register unsigned short vc;
	register unsigned short a;
	register unsigned short tile;
	register unsigned short tx;

	bx = col << 2;
	by = row << 2;
	if (bg) {
		left = g_CameraX - MARGIN(g_BgNarrow);
		right = g_CameraX + 0x140 + MARGIN(g_BgNarrow);
		attr = g_BgTileAttr;
		vram = 0;
		scrollX = g_BgScrollAdjX >> 3;
		scrollY = g_BgScrollAdjY >> 3;
		top = MAX(by, g_BgStreamY >> 3);
		bottom = UMIN(by + 3, (g_BgStreamY >> 3) + 31);
		leftT = MAX(bx, g_BgStreamX >> 3);
		rightT = UMIN(bx + 3, (g_BgStreamX >> 3) + 63);
		cell = g_BgBlockMap[g_BgRowTable[row] + col];
		defs = (u16 *)g_BgBlockDefs + ((cell & 0x1FF) << 4);
	} else {
		left = g_CameraX - MARGIN(g_MapStreamNarrow);
		right = g_CameraX + 0x140 + MARGIN(g_MapStreamNarrow);
		attr = g_FgTileAttr;
		vram = (short)0xE000;
		scrollX = g_FgScrollAdjX >> 3;
		scrollY = g_FgScrollAdjY >> 3;
		top = MAX(by, g_FgStreamY >> 3);
		bottom = UMIN(by + 3, (g_FgStreamY >> 3) + 31);
		leftT = MAX(bx, g_FgStreamX >> 3);
		rightT = UMIN(bx + 3, (g_FgStreamX >> 3) + 63);
		cell = g_FgBlockMap[g_FgRowTable[row] + col];
		defs = (u16 *)g_FgBlockDefs + ((cell & 0x1FF) << 4);
	}
	if (col << 5 < left || col << 5 > right || row << 5 < g_CameraY - 32 || row << 5 > g_CameraY + 256)
		return;
	pal = attr & 0xE000;
	attr &= 0x7FF;
	flips = cell & 0x600;
	prio = cell & 0x8000;
	blk = cell & 0x1FF;
	xorv = (flips << 2) ^ prio ^ pal;
	if (flips & 0x200) {
		if (flips & 0x400) {
			/* both flips: rows and columns of the block backwards */
			for (ty = top, dr = 3; ty <= bottom; ty++, dr -= 2) {
				vr = (ty + scrollY) & 0x1F;
				for (tx = leftT, dc = 3; tx <= rightT; tx++, dc -= 2) {
					vc = (tx + scrollX) & 0x3F;
					a = vram + (((vr << 6) + vc) << 1);
					tile = (defs[((ty - by + dr) << 2) + (tx - bx + dc)] + attr) ^ xorv;
					Irq_Disable();
					VDP_CTRL32 = VRAM_WRITE(a);
					VDP_DATA = tile;
					Irq_Enable();
				}
			}
		} else {
			/* h-flip: columns backwards */
			for (ty = top; ty <= bottom; ty++) {
				vr = (ty + scrollY) & 0x1F;
				for (tx = leftT, dc = 3; tx <= rightT; tx++, dc -= 2) {
					vc = (tx + scrollX) & 0x3F;
					a = vram + (((vr << 6) + vc) << 1);
					tile = (defs[((ty - by) << 2) + (tx - bx + dc)] + attr) ^ xorv;
					Irq_Disable();
					VDP_CTRL32 = VRAM_WRITE(a);
					VDP_DATA = tile;
					Irq_Enable();
				}
			}
		}
	} else {
		/* (a compound statement here, as the original: MPW puts its stack temporary at the same
		   place as in the h-flip part) */
		if (flips & 0x400) {
			/* v-flip: rows backwards */
			for (ty = top, dr = 3; ty <= bottom; ty++, dr -= 2) {
				vr = (ty + scrollY) & 0x1F;
				for (tx = leftT; tx <= rightT; tx++) {
					vc = (tx + scrollX) & 0x3F;
					a = vram + (((vr << 6) + vc) << 1);
					tile = (defs[((ty - by + dr) << 2) + (tx - bx)] + attr) ^ xorv;
					Irq_Disable();
					VDP_CTRL32 = VRAM_WRITE(a);
					VDP_DATA = tile;
					Irq_Enable();
				}
			}
		} else {
			for (ty = top; ty <= bottom; ty++) {
				vr = (ty + scrollY) & 0x1F;
				for (tx = leftT; tx <= rightT; tx++) {
					vc = (tx + scrollX) & 0x3F;
					a = vram + (((vr << 6) + vc) << 1);
					tile = (defs[((ty - by) << 2) + (tx - bx)] + attr) ^ (prio ^ pal);
					Irq_Disable();
					VDP_CTRL32 = VRAM_WRITE(a);
					VDP_DATA = tile;
					Irq_Enable();
				}
			}
		}
	}
}
