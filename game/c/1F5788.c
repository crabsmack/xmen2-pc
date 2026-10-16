/* Level_LoadMaps - set up the two planes (Level_Load): the camera at the start, its limits from the
 * FG map, the initial scroll, the block layouts and definitions decoded, the used tiles loaded
 * to VRAM from tile $80 (FG then BG), the special-block slots, the row-offset tables, the tile
 * attributes, the first scroll values written, and the visible rows drawn.
 *
 * fgMap, fgTiles: the FG map and its tileset (0: the map's own).  bgMap, bgTiles: the same for
 * BG (bgMap 0: no BG).  x, y: the start position (the camera at x, y less g_StartCamOffsetX/Y).
 * fgPrio, bgPrio: the planes' tiles get priority ($8000).  bgPal2: the BG tiles use palette 2
 * (+$4000).  bgFixed: the BG doesn't scroll (wraps, starts at 0).
 * A map whose packing nibble is 0 has raw tiles (VDP_WriteWords) and its layout used in place;
 * else everything is unpacked onto the heap (Eni_Decode) and the tiles loaded masked
 * (Level_LoadUsedTiles; a tileset shared by both planes is loaded once, with all its tiles).
 * The heap gets tagged marks between the parts ("A PLANE TILES", "AB TILES -- SHARING", "A PLANE
 * TILES -- NO B"); g_MapsHeapEnd keeps the heap top at the end. With g_CamSetPosHook Cam_SetPosScaled, the
 * scroll follows the camera through the plane factors. The unpacked path writes its heap mark
 * through a local never set (kept: on PC skipped). */
#include "xmen2.h"

void Level_LoadMaps(void *fgMapp, void *fgTilesp, void *bgMap, void *bgTiles, short x, short y,
	unsigned char fgPrio, unsigned char bgPrio, unsigned char bgPal2, unsigned char bgFixed)
{
	MemBlock *hdr;
	char shared;
	register MapHeader *fgTiles = (MapHeader *)fgTilesp;
	register MapHeader *fgMap = (MapHeader *)fgMapp;
	register short tiles;
	register short bgCount;
	register short row;	/* also the special-block slot count */
	register short off;
	register short w;
	register short h;

	if (fgTiles && bgTiles && fgTiles == bgTiles)
		shared = 1;
	else
		shared = 0;
	Scroll_Init();
	{
		register char wrap;

		g_BgWrapX = wrap = bgFixed;
		g_BgWrapY = wrap;
	}
	if (fgMap)
		g_FgMapPresent = 1;
	if (!fgTiles)
		fgTiles = fgMap;
	g_CameraX = x - g_StartCamOffsetX;
	g_CameraY = y - g_StartCamOffsetY;
	if (g_FgMapPresent) {
		/* the map's size in cells */
#ifdef XMEN2_MPW
		w = ((MapHeader *)((long)fgMap + 0))->width;	/* through the address as a number, as the original */
		h = ((MapHeader *)((long)fgMap + 0))->height;
#else
		w = fgMap->width;
		h = fgMap->height;
#endif
		g_FgMapExtraW = (w - 0x40) << 3;
		g_FgMapExtraH = (h - 0x20) << 3;
		g_CamMaxX = (w - 0x28) << 3;
		g_CamMinX = 0;
		g_CamMaxY = (h - 0x1C) << 3;
		g_CamMinY = 0;
		g_CamMaxXTarget = g_CamMaxX;
		g_CamMinXTarget = 0;
		g_CamMaxYTarget = g_CamMaxY;
		g_CamMinYTarget = 0;
	}
	if (g_CameraX > g_CamMaxX)
		g_CameraX = g_CamMaxX;
	if (g_CameraX < g_CamMinX)
		g_CameraX = g_CamMinX;
	if (g_CameraY > g_CamMaxY)
		g_CameraY = g_CamMaxY;
	if (g_CameraY < g_CamMinY)
		g_CameraY = g_CamMinY;
	if (g_CamSetPosHook == Cam_SetPosScaled) {
		g_FgScrollX = g_FgScrollBaseX + (g_CameraX * g_FgScrollFactorX >> 8);
		g_FgScrollY = g_FgScrollBaseY + (g_CameraY * g_FgScrollFactorY >> 8);
	}
	if (g_BgLockToObj) {
		g_BgScrollX = g_BgOffsetX - ((((Actor *)g_BgLockObj)->x >> 8) - g_CameraX);
		g_BgScrollY = g_BgOffsetY - ((((Actor *)g_BgLockObj)->y >> 8) - g_CameraY);
	} else {
		g_BgScrollX = (short)Math_MulFix8(g_CameraX, g_BgScrollFactorX);
		if (g_CamSetPosHook == Cam_SetPosScaled)
			g_BgScrollY = g_BgScrollBaseY + (g_CameraY * g_BgScrollFactorY >> 8);
		else
			g_BgScrollY = g_BgScrollBaseY + ((g_CameraY - g_FgScrollBaseY) * g_BgScrollFactorY >> 8);
	}
	if (g_BgWrapX)
		g_BgScrollX &= 0x1FF;
	if (g_BgWrapY)
		g_BgScrollY &= 0xFF;
	if ((g_FgStreamX = (g_FgScrollX & 0xFFE0) - 0x60) < 0)
		g_FgStreamX = 0;
	if ((g_FgStreamY = (g_FgScrollY & 0xFFF8) - 8) < 0)
		g_FgStreamY = 0;
	if ((g_BgStreamX = (g_BgScrollX & 0xFFE0) - 0x60) < 0)
		g_BgStreamX = 0;
	if ((g_BgStreamY = (g_BgScrollY & 0xFFF8) - 8) < 0)
		g_BgStreamY = 0;
	if (fgMap) {
		g_MapRowHook = Map_StreamRows;
		g_MapColHook = Map_StreamColumns;
		g_LoadedFgMap = fgMap;
		g_FgMapW = (short)fgMap->width >> 2;
		g_FgMapH = (short)fgMap->height >> 2;
		tiles = fgTiles->tileCount;
		if ((fgMap->packing & 0xF0) == 0) {
			g_FgBlockMap = (s16 *)((char *)fgMap + fgMap->layout);
			g_FgBlockDefs = (char *)fgTiles + fgTiles->blockDefs;
			VDP_CTRL32 = 0x40000000;
			VDP_WriteWords((char *)fgTiles + fgTiles->tiles, (long)tiles << 4);
		} else {
			register unsigned char fromMap;

			g_FgBlockMap = (s16 *)Mem_StackAlloc(g_FgMapW * 2 * g_FgMapH, STR(Str_MapBufTagFg, ""));
			Eni_Decode((u8 *)fgMap + fgMap->layout, (u16 *)(s16 *)g_FgBlockMap, 0);
			if (bgMap) {
				g_MapFirstTile = 0x80;
				g_BgMapPresent = 1;
				g_BgMapW = (short)((MapHeader *)bgMap)->width >> 2;
				if (g_BgMapW < 0x10)
					g_BgNarrow = 1;
				g_BgMapH = (short)((MapHeader *)bgMap)->height >> 2;
				if (!bgTiles)
					bgTiles = bgMap;
				g_BgBlockMap = (s16 *)Mem_StackAlloc(g_BgMapW * 2 * g_BgMapH, STR(Str_MapBufTagBg, ""));
				Eni_Decode((u8 *)bgMap + ((MapHeader *)bgMap)->layout, (u16 *)(s16 *)g_BgBlockMap, 0);
			} else {
				g_MapFirstTile = 0x80;
				g_BgMapPresent = 0;
			}
			fromMap = fgMap->flags & 0x20 ? 0 : 1;
			g_FgBlockCount = fgTiles->blockCount;
			hdr = g_HeapTop;
			Mem_StackAlloc(0, STR(Str_MapBufTags, ""));
			g_FgBlockDefs = g_HeapTop;
			Eni_Decode((u8 *)fgTiles + fgTiles->blockDefs, g_FgBlockDefs, 0);
			tiles = Level_LoadUsedTiles(fgMap, fgTiles, g_MapFirstTile, g_FgBlockMap, g_FgBlockDefs,
				shared ? 0 : fromMap);
			g_FgBlockCount = g_UsedBlockCount;
			g_FgTilesEnd = tiles + g_MapFirstTile;
			row = Level_AssignBlockSlots(fgMap, g_FgBlockMap, g_FgBlockDefs, tiles);
			tiles += row << 4;
			if (bgMap) {
				if (shared) {
					g_BgBlockCount = g_FgBlockCount;
					g_BgBlockDefs = g_FgBlockDefs;
					bgCount = off = tiles;
					g_BgTilesEnd = off + g_MapFirstTile;
					hdr->tag = STR(Str_AbTilesSharing, "AB TILES -- SHARING");
				} else {
					hdr->next = g_HeapTop;
					hdr->tag = STR(Str_APlaneTiles, "A PLANE TILES");
					hdr = hdr->next;
					Mem_StackAlloc(0, STR(Str_BTilesTag, ""));
					g_BgBlockCount = ((MapHeader *)bgTiles)->blockCount;
					g_BgBlockDefs = g_HeapTop;
					Eni_Decode((u8 *)bgTiles + ((MapHeader *)bgTiles)->blockDefs, g_BgBlockDefs, 0);
					bgCount = Level_LoadUsedTiles(bgMap, bgTiles, tiles + g_MapFirstTile, g_BgBlockMap,
						g_BgBlockDefs, 1);
					g_BgBlockCount = g_UsedBlockCount;
					g_BgTilesEnd = (tiles + bgCount) + g_MapFirstTile;
				}
				row = Level_AssignBlockSlots(bgMap, g_BgBlockMap, g_BgBlockDefs, bgCount);
				bgCount += row << 4;
			} else {
				g_BgBlockCount = 0;
				bgCount = 0;
				hdr->tag = STR(Str_APlaneTilesNoB, "A PLANE TILES -- NO B");
			}
		}
#ifndef XMEN2_MPW
		if ((fgMap->packing & 0xF0) != 0)	/* unpacked: hdr was never set (the 68000 writes through it) */
#endif
		hdr->next = g_HeapTop;
		if (g_FgMapH > 7)
			g_FgStreamY += 0x100;
		else
			g_FgStreamY += 0xE0;
		g_FgRowTable = (s16 *)Mem_StackAlloc(g_FgMapH * 2, STR(Str_RowTableTagFg, ""));
		off = 0;
		for (row = 0; row < g_FgMapH; row++) {
			g_FgRowTable[row] = off;
			off += g_FgMapW;
		}
	} else
		g_MapFirstTile = 0x80;
	if (!bgMap) {
		g_BgMapPresent = 0;
		g_MapTilesEnd = tiles + g_MapFirstTile;
		g_BgScrollX = 0;
		g_BgScrollY = 0;
	} else {
		if (!bgTiles)
			bgTiles = bgMap;
		g_BgMapPresent = 1;
		g_BgMapW = (short)((MapHeader *)bgMap)->width >> 2;
		g_BgMapH = (short)((MapHeader *)bgMap)->height >> 2;
		if (shared) {
			g_BgFirstTile = g_MapFirstTile;
			g_MapTilesEnd = bgCount + g_MapFirstTile;
		} else
			g_MapTilesEnd = (g_BgFirstTile = tiles + g_MapFirstTile) + bgCount;
		if (g_BgMapH > 7)
			g_BgStreamY += 0x100;
		else
			g_BgStreamY += 0xE0;
		g_BgRowTable = (s16 *)Mem_StackAlloc(g_BgMapH * 2, STR(Str_RowTableTags, ""));
		off = 0;
		for (row = 0; row < g_BgMapH; row++) {
			g_BgRowTable[row] = off;
			off += g_BgMapW;
		}
	}
	g_FgTileAttr = g_MapFirstTile;
	if (g_FgMapPresent && !shared)
		g_BgTileAttr = tiles + g_MapFirstTile;
	else
		g_BgTileAttr = g_MapFirstTile;
	if (fgPrio)
		g_FgTileAttr |= 0x8000;
	if (bgPrio)
		g_BgTileAttr |= 0x8000;
	if (bgPal2)
		g_BgTileAttr += 0x4000;
	VDP_CTRL32 = 0x74000003;
	VDP_DATA = -(g_FgScrollX & 0x1FF);
	VDP_CTRL32 = 0x40000010;
	VDP_DATA = g_FgScrollY & 0xFF;
	VDP_CTRL32 = 0x74020003;
	VDP_DATA = -(g_BgScrollX & 0x1FF);
	VDP_CTRL32 = 0x40020010;
	VDP_DATA = g_BgScrollY & 0xFF;
	g_BgWrapX = 0;
	g_BgWrapY = 0;
	if (bgFixed) {
		g_BgStreamX = 0;
		g_BgStreamY = 0x100;
		g_BgScrollX = 0;
		g_BgScrollY = 0;
	}
	Irq_Disable();
	(*g_MapRowHook)();
	Irq_Enable();
	{
		register char wrap;

		g_BgWrapX = wrap = bgFixed;
		g_BgWrapY = wrap;
	}
	g_MapsHeapEnd = g_HeapTop;
}
