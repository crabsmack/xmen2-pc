/* PlayerSelect_AllocCharBuffers - allocate the player select's scaler buffers, big enough for the
 * largest select-screen frame of the 7 characters (their <Char>_SelectSpriteDef /
 * _SelectFrame, index order Beast, Cyclops, Gambit, Nightcrawler, Psylocke, Wolverine, Magneto).
 *
 * Per character: the frame's bounding box (width rounded up to even); the largest area (w * h)
 * keeps its width and height, the largest tile count ((w + 7) / 8 * (h + 7) / 8) is kept apart
 * (the two can come from different characters), at least g_PlayerTiles. g_ScaleTileBuf gets
 * that many tiles (32 bytes each), g_ScaleSrcBitmap a bitmap of the largest area plus its $26-byte
 * header (box 0, 0, width, height). The cases are PlayerSelect_AllocCharBuffers_Cases, the box
 * measuring PlayerSelect_AllocCharBuffers_Measure (also the switch's default: no definition).
 * Returns the tiles reserved. */
#include "xmen2.h"

short PlayerSelect_AllocCharBuffers(void)
{
	short tiles, maxTiles;
	long area, maxArea;
	short maxH, maxW;
	register short i;
	register void *def;
	register short frame;
	register SprFrame *fr;
	register short w, h;
	register ScalePicture *pic;

	maxTiles = 0;
	maxArea = 0;
	maxH = 0;
	maxW = 0;
	for (i = 0; i < 7; ++i) {
		switch (i) {
		case 0:
			def = Beast_SelectSpriteDef();
			frame = Beast_SelectFrame();
			break;
		case 1:
			def = Cyclops_SelectSpriteDef();
			frame = Cyclops_SelectFrame();
			break;
		case 2:
			def = Gambit_SelectSpriteDef();
			frame = Gambit_SelectFrame();
			break;
		case 3:
			def = Nightcrawler_SelectSpriteDef();
			frame = Nightcrawler_SelectFrame();
			break;
		case 4:
			def = (void *)Psylocke_SelectSpriteDef();
			frame = (short)Psylocke_SelectFrame();
			break;
		case 5:
			def = Wolverine_SelectSpriteDef();
			frame = Wolverine_SelectFrame();
			break;
		case 6:
			def = Magneto_SelectSpriteDef();
			frame = Magneto_SelectFrame();
			break;
		default:
			def = 0;
			frame = 0xFF;
			break;
		}
		if (def) {
			fr = SPR_FRAME(def, frame);
			w = fr->right - fr->left;
			h = fr->bottom - fr->top;
			w += w & 1;
			area = h * w;
			if (area > maxArea) {
				maxArea = area;
				maxH = h;
				maxW = w;
			}
			w = (w + 7) >> 3;
			h = (h + 7) >> 3;
			tiles = w * h;
			if (tiles > maxTiles)
				maxTiles = tiles;
		}
	}
	if (g_PlayerTiles > maxTiles)
		maxTiles = g_PlayerTiles;
	g_ScaleTileBuf = Mem_StackAlloc(maxTiles << 5, STR(Str_Empty_0647CE, ""));
	pic = (ScalePicture *)(g_ScaleSrcBitmap = Mem_StackAlloc(maxArea + 0x26, STR(Str_Empty_0647CC, "")));
	pic->unk_00[0] = 0;
	pic->unk_00[1] = 0;
	pic->left = 0;
	pic->top = 0;
	pic->right = maxW;
	pic->bottom = maxH;
	return maxTiles;
}
