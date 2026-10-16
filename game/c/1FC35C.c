/* Spr_RenderToBitmap - render one frame of a raw-tile sprite definition into a byte-per-pixel
 * buffer (Cerebro_PlayIntro, PlayerSelect_LoadCharFrame_part1).
 *
 * def: the definition (type $18, 8-byte pieces).  frame: the frame.  buf: the buffer: +2/+3 0,
 * +4 width, +5 height, pixels from +$26.
 * The bitmap is the frame's bounding box (width rounded up to even), cleared first (width *
 * height bytes, the product's low 16 bits). Each piece's 4bpp tiles (column by column, 8 rows of
 * a long each) are expanded two pixels a byte through Tbl_PixelPairExpand; 0 pixels are left
 * alone, an all-0 row is skipped. A flipped piece (attr $800 / $1000) is written from its far
 * edge (Tbl_SizeCodePixels) backwards / upwards. Hand-written 68000 in an MPW frame (dbra, rol);
 * the inner loop is Spr_RenderToBitmap_Loop in the disassembly. */
#include "xmen2.h"

/* One byte of a tile row: two pixels (high nibble first in the direction of writing); 0 pixels
   are transparent. */
#define PUT_PAIR(dst, b) \
	do { \
		if (b) { \
			pair = expand[b]; \
			if ((u8)(pair >> 8)) \
				*(dst) = (u8)(pair >> 8); \
			(dst)++; \
			if ((u8)pair) \
				*(dst) = (u8)pair; \
			(dst)++; \
		} else \
			(dst) += 2; \
	} while (0)
#define PUT_PAIR_BACK(dst, b) \
	do { \
		if (b) { \
			pair = expand[b]; \
			(dst)--; \
			if ((u8)(pair >> 8)) \
				*(dst) = (u8)(pair >> 8); \
			(dst)--; \
			if ((u8)pair) \
				*(dst) = (u8)pair; \
		} else \
			(dst) -= 2; \
	} while (0)

void Spr_RenderToBitmap(SprDef *def, short frame, u8 *buf)
{
	SprFrame *f = (SprFrame *)((u8 *)def->frame + (short)(frame * 20));
	s16 *expand = (s16 *)CODEPTR(Tbl_PixelPairExpand);
	u8 *sizePx = (u8 *)CODEPTR(Tbl_SizeCodePixels);
	short count = (u8)f->pieceCount;
	short left = f->left;
	short top = f->top;
	short w, h;
	unsigned short n;
	u8 *pix, *dst, *col;
	SprPiece8 *pc;
	s32 *src;
	unsigned long bits;
	long stride, step;
	short x, y, rows, cols, r, c, line, i;
	char hflip, vflip;
	u8 b;
	unsigned short pair;

	w = f->right - left;
	w += w & 1;
	h = f->bottom - top;
	buf[2] = 0;
	buf[3] = 0;
	buf[4] = (u8)w;
	buf[5] = (u8)h;
	n = (unsigned short)((unsigned short)h * (unsigned short)w);
	pix = buf + 0x26;
	dst = pix;
	for (i = n >> 2; i; i--) {
		*(s32 *)dst = 0;
		dst += 4;
	}
	for (i = n & 3; i; i--)
		*dst++ = 0;
	pc = (SprPiece8 *)((u8 *)def + (s16)f->pieces);
	for (; count; count--, pc++) {
		rows = pc->attr & 3;
		cols = pc->attr >> 2 & 3;
		hflip = (pc->attr & 0x800) != 0;
		vflip = (pc->attr & 0x1000) != 0;
		x = pc->x - left;
		y = pc->y - top;
		stride = (unsigned short)w;
		step = -8;
		if (hflip) {
			x += sizePx[cols];
			step = 8;
		}
		if (vflip) {
			y += sizePx[rows] - 1;
			stride = -stride;
		}
		stride += step;
		dst = col = pix + (unsigned long)((unsigned short)y * (unsigned short)w) + x;
		src = (s32 *)((u8 *)def + (s16)pc->tiles);
		for (c = cols; c >= 0; c--) {	/* Spr_RenderToBitmap_Loop */
			for (r = rows; r >= 0; r--) {
				for (line = 7; line >= 0; line--) {
					bits = *src++;
					if (!bits) {
						dst += vflip ? -(long)(unsigned short)w : (long)(unsigned short)w;
						continue;
					}
					if (!hflip) {
						b = (u8)(bits >> 24);
						PUT_PAIR(dst, b);
						b = (u8)(bits >> 16);
						PUT_PAIR(dst, b);
						b = (u8)(bits >> 8);
						PUT_PAIR(dst, b);
						b = (u8)bits;
						PUT_PAIR(dst, b);
					} else {
						b = (u8)(bits >> 24);
						PUT_PAIR_BACK(dst, b);
						b = (u8)(bits >> 16);
						PUT_PAIR_BACK(dst, b);
						b = (u8)(bits >> 8);
						PUT_PAIR_BACK(dst, b);
						b = (u8)bits;
						PUT_PAIR_BACK(dst, b);
					}
					dst += stride;
				}
			}
			dst = col = hflip ? col - 8 : col + 8;
		}
	}
}
