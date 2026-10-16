/* Cerebro_PlayIntro - the Cerebro intro (the first Cerebro screen of a game, and LogoAndDemo).
 *
 * The V-blank game hook becomes Cerebro_IntroPalHook (frame countdown from 10000); the scaler
 * gets its parameters and two heap buffers (a $2A56-byte source bitmap, a $1560-byte tile
 * buffer); palette lines 1-3 black. Unless Start was seen before (progress flag 2): a logo
 * (Art_CerebroNem6 with the 12x4 map Art_CerebroNem5 at VRAM $051C) through the SEGA flash
 * (Intro_SegaLogoFlash), then the first picture (Art_CerebroIntroPic1, map Map_CerebroIntroPic1
 * 16x12 at $0318) with its title glowing in and out over 180 frames (Pal_CerebroTitleGlow, 8
 * stages), then six lines of the copyright text (Str_XMenAndAllRelated, $20 bytes each) for 180
 * frames and up to 180 more (a button ends them). Then music $A, the second picture
 * (Art_CerebroIntroPic2, map Map_CerebroIntroPic2 11x14 at $01AC, palette line 1
 * Pal_CerebroIntroLine1) and the Cerebro logo (Art_CerebroSpr) as a big sprite: frame 0 flies
 * in 12 steps from (160, 112), growing from nothing, to (60, 64), its last frame adding the
 * logo's small sprite; after 90 frames frame 1 grows from a 2-px line at (160, 149) to (72,
 * 149) in 8 steps (palette line 2 Pal_CerebroIntroLine2) and then opens to its full height up
 * to (72, 143) in 8 more, frame 0 drawn as a sprite meanwhile; "DEVELOPED BY  HEADGAMES, INC."
 * under the window, 120 frames. LogoAndDemo: once the title glow is at its stage with colour
 * $3A = $40, "PRESS" and "START" in sprites for up to 600 frames (a button: progress flag 2);
 * otherwise 600 frames (a button ends them). Then an empty sprite list, the VRAM tiles, hook and
 * buffers given back, and the pads' pressed/latched bits cleared.
 * Each animation frame waits for the frame tick (g_FrameTick) cleared; the flight frames wait for
 * 3 V-blanks (g_VBlankCount).
 * PC: the tilemap buffer, the big sprite and the sprite instance are game memory (generated code
 * and the scaler read them): placed on the 68000 stack where the original's frame had them
 * (a6 = A7 at entry - 4; -$1A6, -$1E8, -$14), the frame ($1F0 bytes, 7 saved registers) reserved. */
#include "xmen2.h"

#define g_TextState	(*(s16 *)g_SceneVar3B02)	/* scene variable: Cerebro text state */
#define g_TextCol	(*(s16 *)g_SceneVar3B04)	/* scene variable: Cerebro text cursor column */
#define g_TextRow	(*(s16 *)g_SceneVar3B06)	/* scene variable: Cerebro text cursor row */
#define g_CerebroIntro	(*(s16 *)g_SceneVar3B0C)	/* scene variable: the intro's frame countdown (Cerebro_IntroPalHook) */
#define g_CerebroSprTiles	(*(s16 *)g_SceneVar3B10)	/* scene variable: first VRAM tile of the Cerebro sprite tiles */

/* The next hardware sprite of the list (as the sprite emitters take it). */
#define NEXT_SPRITE()	(g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link))

void Cerebro_PlayIntro(void)
{
	void *def;
#ifdef XMEN2_MPW
	BigSprite bs;
	s16 buf[0xC0];
#define BS	(&bs)
#endif
	short x0, y0, w0;
	short x1, y1, w1, h1;
	short offX;
	short savedTiles;
#ifdef XMEN2_MPW
	SprInst spr;
#define SPR	(&spr)
#else
	unsigned long a7 = CPU_GetSP();
	unsigned long fp = xm::sp_reserve(4 + 0x1F0) + 0x1F0;	/* the original's a6 */
	BigSprite *BS = AT68(BigSprite, fp - 0x1E8);
	s16 *buf = AT68(s16, fp - 0x1A6);
	SprInst *SPR = AT68(SprInst, fp - 0x14);
#endif
	register s16 *map = buf;
	register short i;
	register short n;
	register short tile;
	register short count;
	register s16 *p;
	register long j;
	register short h0;
	register short offY;

#ifndef XMEN2_MPW
	xm::sp_reserve(28);
#endif
	savedTiles = g_VramTileCursor;
	g_TextState = 1;
	g_CerebroIntro = 10000;
	g_VBlankGameHook = FN_DATA(Cerebro_IntroPalHook);
	Scaler_InitParams();
	g_ScaleSrcBitmap = Mem_StackAlloc(0x2A56, STR(Str_Empty_066B24, ""));
	g_ScaleTileBuf = Mem_StackAlloc(0x1560, STR(Str_Empty_066B22, ""));
	for (i = 0, p = &g_PalBase[0x10]; i < 0x30; i++, p++)
		*p = 0;
	if (!(g_ProgressFlags & 2)) {
		char *text;

		Art_LoadNemToVram(CODEPTR(Art_CerebroNem6), g_VramTileCursor);
		tile = g_VramTileCursor | 0x4000;
		count = 0x30;
		Mem_Copy(map, CODEPTR(Art_CerebroNem5), count << 1);
		for (i = 0, p = &map[0]; i < count; i++, p++)
			*p += tile;
		VDP_WriteTileRect(0x51C, map, 0xC, 4, 0x40);
		Intro_SegaLogoFlash(0, 2);
		Cerebro_ClearScreen();
		for (i = 0, p = &g_PalBase[0x10]; i < 0x30; i++, p++)
			*p = 0;
		g_PalBaseDirty = 1;
		VDP_WaitVBlank();
		Art_LoadNemToVram(CODEPTR(Art_CerebroIntroPic1), g_VramTileCursor);
		tile = g_VramTileCursor | 0x4000;
		count = 0xC0;
		Mem_Copy(map, CODEPTR(Map_CerebroIntroPic1), count << 1);
		for (i = 0, p = &map[0]; i < count; i++, p++)
			*p += tile;
		VDP_WriteTileRect(0x318, map, 0x10, 0xC, 0x40);
		n = 0xB4;
		p = (s16 *)CODEPTR(Pal_CerebroTitleGlow);
		while (n) {
			g_FrameTick = 0;
			i = (0xB4 - n) >> 2;
			tile = n >> 2;
			i = MIN(i, tile);
			if (i > 7)
				i = 7;
			g_PalBase[0x2E] = p[i];
			g_PalBase[0x2D] = p[i + 8];
			g_PalBaseDirty = 1;
			--n;
			VDP_WaitVBlank();
		}
		text = (char *)CODEPTR(Str_XMenAndAllRelated);
		Cerebro_ClearScreen();
		Cerebro_ClearTextWindow();
		g_PalBase[0x3F] = 0xCC;
		g_PalBase[0x3E] = 0xAA;
		g_PalBaseDirty = 1;
		g_TextCol = g_TextWinLeft;
		g_TextRow = g_TextWinTop + 4;
		Cerebro_PrintText(text, 0);
		++g_TextRow;
		text += 0x20;
		Cerebro_PrintText(text, 0);
		++g_TextRow;
		text += 0x20;
		Cerebro_PrintText(text, 0);
		++g_TextRow;
		text += 0x20;
		Cerebro_PrintText(text, 0);
		g_TextRow += 3;
		text += 0x20;
		Cerebro_PrintText(text, 0);
		++g_TextRow;
		text += 0x20;
		Cerebro_PrintText(text, 0);
		n = 0xB4;
		while (n) {
			--n;
			VDP_WaitVBlank();
		}
		n = 0xB4;
		while (n) {
			g_FrameTick = 0;
			--n;
			VDP_WaitVBlank();
			if ((g_Pad1Raw | g_Pad2Raw) & 0xF0)
				n = 0;
		}
		Cerebro_ClearTextWindow();
	}
	Snd_Stop(0xA);
	Snd_Play(0xA);
	Cerebro_ClearScreen();
	for (i = 0, p = &g_PalBase[0x10]; i < 0x30; i++, p++)
		*p = 0;
	g_PalBaseDirty = 1;
	VDP_WaitVBlank();
	Art_LoadNemToVram(CODEPTR(Art_CerebroIntroPic2), g_VramTileCursor);
	tile = g_VramTileCursor | 0x4000;
	count = 0x9A;
	Mem_Copy(map, CODEPTR(Map_CerebroIntroPic2), count << 1);
	for (i = 0, p = &map[0]; i < count; i++, p++)
		*p += tile;
	VDP_WriteTileRect(0x1AC, map, 0xB, 0xE, 0x40);
	g_VramTileCursor += 0xC0;
	BS->params = g_ScaleParamsPtr;
	BS->tiles = 0xAB;
	BS->tileBase = g_VramTileCursor;
	BS->unk_0C = 1;
	g_VramTileCursor += 0xAB;
	{
		register s16 *dst;

		p = (s16 *)CODEPTR(Pal_CerebroIntroLine1);
		dst = &g_PalBase[0x10];
		for (j = 0; j < 16; j++)
			*dst++ = *p++;
	}
	g_PalBase[0x3F] = 0xA0;
	g_PalBase[0x3E] = 0x80;
	g_PalBase[0x3D] = 0x660;
	g_PalBase[0x3C] = 0x862;
	g_PalBase[0x3B] = 0xA0;
	g_PalBase[0x3A] = 0x80;
	g_PalBase[0x39] = 0xA;
	g_PalBase[0x38] = 8;
	g_PalBase[0x31] = 0;
	g_PalBaseDirty = 1;

	/* The logo flies in, growing from nothing. */
	Spr_RenderToBitmap((SprDef *)CODEPTR(Art_CerebroSpr), 0, g_ScaleSrcBitmap);
	Cerebro_BigSpriteInit(BS, g_ScaleSrcBitmap);
	x1 = 0x3C;
	y1 = 0x40;
	x0 = 0xA0;
	y0 = 0x70;
	w0 = 0;
	h0 = 0;
	w1 = BS->params->srcW;
	h1 = BS->params->srcH;
	n = 12;
	BS->vw = ((long)(w1 - w0) << 16) / n;
	BS->vh = ((long)(h1 - h0) << 16) / n;
	BS->vx = ((long)(x1 - x0) << 16) / n;
	BS->vy = ((long)(y1 - y0) << 16) / n;
	BS->x = (long)x0 << 16;
	BS->y = (long)y0 << 16;
	BS->w = (long)w0 << 16;
	BS->h = (long)h0 << 16;
	g_CerebroIntro = 0x112;
	while (n) {
		g_VBlankCount = 0;
		Spr_BeginFrame();
		NEXT_SPRITE();
		g_SprWritePtr->y = 0x1C;
		g_SprWritePtr->size = 0;
		g_SprWritePtr->attr = 0;
		g_SprWritePtr->x = 0x1C;
		g_FrameTick = 0;
		BS->x += BS->vx;
		BS->y += BS->vy;
		BS->w += BS->vw;
		BS->h += BS->vh;
		BS->params->outW = BS->w >> 16;
		BS->params->outH = BS->h >> 16;
		Cerebro_BigSpriteDecode(BS);
		Cerebro_BigSpriteDraw(BS);
		--n;
		if (!n) {
			NEXT_SPRITE();
			g_SprWritePtr->y = 0xC0;
			g_SprWritePtr->size = 4;
			g_SprWritePtr->attr = g_CerebroSprTiles + 0x2074;
			g_SprWritePtr->x = 0x145;
		}
		while (g_VBlankCount < 3)
			VDP_WaitVBlank();
		Spr_EndFrame();
		g_DmaFlushReq = 1;
		VDP_WaitVBlank();
	}

	/* Frame 0 stays as a sprite; frame 1 grows from a line, then opens. */
	Spr_Init(SPR, 1, CODEPTR(Art_CerebroSpr), 0, 1, BS->tileBase, 0);
	{
		register SprFrame *fr;

		def = CODEPTR(Art_CerebroSpr);
		fr = ((SprDef *)def)->frame;
		offX = -fr->left;
		offY = -fr->top;
	}
	BS->params = g_ScaleParamsPtr;
	BS->tiles = 0x2C;
	BS->tileBase = g_VramTileCursor;
	BS->unk_0C = 2;
	g_VramTileCursor += 0x2C;
	n = 0x5A;
	while (n) {
		g_FrameTick = 0;
		--n;
		VDP_WaitVBlank();
	}
	Spr_RenderToBitmap((SprDef *)CODEPTR(Art_CerebroSpr), 1, g_ScaleSrcBitmap);
	Cerebro_BigSpriteInit(BS, g_ScaleSrcBitmap);
	x1 = 0x48;
	y1 = 0x95;
	x0 = 0xA0;
	y0 = 0x95;
	w0 = 0;
	h0 = 2;
	w1 = BS->params->srcW;
	h1 = 2;
	n = 8;
	BS->vw = ((long)(w1 - w0) << 16) / n;
	BS->vh = ((long)(h1 - h0) << 16) / n;
	BS->vx = ((long)(x1 - x0) << 16) / n;
	BS->vy = ((long)(y1 - y0) << 16) / n;
	BS->x = (long)x0 << 16;
	BS->y = (long)y0 << 16;
	BS->w = (long)w0 << 16;
	BS->h = (long)h0 << 16;
	{
		register s16 *src = (s16 *)CODEPTR(Pal_CerebroIntroLine2);

		p = &g_PalBase[0x20];
		for (j = 0; j < 16; j++)
			*p++ = *src++;
	}
	g_PalBaseDirty = 1;
	while (n) {
		Spr_BeginFrame();
		NEXT_SPRITE();
		g_SprWritePtr->y = 0xC0;
		g_SprWritePtr->size = 4;
		g_SprWritePtr->attr = g_CerebroSprTiles + 0x2074;
		g_SprWritePtr->x = 0x145;
		g_FrameTick = 0;
		Spr_Draw(SPR, offX + 0x3C, offY + 0x40);
		BS->x += BS->vx;
		BS->y += BS->vy;
		BS->w += BS->vw;
		BS->h += BS->vh;
		BS->params->outW = BS->w >> 16;
		BS->params->outH = BS->h >> 16;
		Cerebro_BigSpriteDecode(BS);
		Cerebro_BigSpriteDraw(BS);
		--n;
		Spr_EndFrame();
		g_DmaFlushReq = 1;
		VDP_WaitVBlank();
	}
	x1 = 0x48;
	y1 = 0x8F;
	x0 = 0x48;
	y0 = 0x95;
	w0 = BS->params->srcW;
	h0 = 2;
	w1 = BS->params->srcW;
	h1 = BS->params->srcH;
	n = 8;
	BS->vw = ((long)(w1 - w0) << 16) / n;
	BS->vh = ((long)(h1 - h0) << 16) / n;
	BS->vx = ((long)(x1 - x0) << 16) / n;
	BS->vy = ((long)(y1 - y0) << 16) / n;
	BS->x = (long)x0 << 16;
	BS->y = (long)y0 << 16;
	BS->w = (long)w0 << 16;
	BS->h = (long)h0 << 16;
	while (n) {
		Spr_BeginFrame();
		NEXT_SPRITE();
		g_SprWritePtr->y = 0xC0;
		g_SprWritePtr->size = 4;
		g_SprWritePtr->attr = g_CerebroSprTiles + 0x2074;
		g_SprWritePtr->x = 0x145;
		g_FrameTick = 0;
		Spr_Draw(SPR, offX + 0x3C, offY + 0x40);
		BS->x += BS->vx;
		BS->y += BS->vy;
		BS->w += BS->vw;
		BS->h += BS->vh;
		BS->params->outW = BS->w >> 16;
		BS->params->outH = BS->h >> 16;
		Cerebro_BigSpriteDecode(BS);
		Cerebro_BigSpriteDraw(BS);
		--n;
		Spr_EndFrame();
		g_DmaFlushReq = 1;
		VDP_WaitVBlank();
	}
	g_PalBase[0x3F] = 0xCC;
	g_PalBase[0x3E] = 0xAA;
	g_PalBaseDirty = 1;
	g_TextCol = g_TextWinLeft + 1;
	g_TextRow = g_TextWinBottom - 1;
	Cerebro_PrintText(STR(Str_DevelopedByHeadgamesInc, "DEVELOPED BY  HEADGAMES, INC."), 0);
	n = 0x78;
	while (n) {
		--n;
		VDP_WaitVBlank();
	}
	if (g_ScnLogoAndDemo == g_SceneIndex) {
		register char *s;
		register short x;
		register char c;
		short y;

		while (g_PalBase[0x3A] != 0x40) {
			g_FrameTick = 0;
			VDP_WaitVBlank();
		}
		Spr_BeginFrame();
		NEXT_SPRITE();
		g_SprWritePtr->y = 0xC0;
		g_SprWritePtr->size = 4;
		g_SprWritePtr->attr = g_CerebroSprTiles + 0x2074;
		g_SprWritePtr->x = 0x145;
		s = STR(Str_Press, "PRESS");
		x = 0x4A;
		y = 0xC6;
		while ((c = *s)) {
			NEXT_SPRITE();
			g_SprWritePtr->y = y + 0x80;
			g_SprWritePtr->size = 0;
			g_SprWritePtr->attr = c - 0x20 + 0x6640;
			g_SprWritePtr->x = x + 0x80;
			s++;
			x += 8;
		}
		s = STR(Str_Start, "START");
		x = 0xD2;
		y = 0xC6;
		while ((c = *s)) {
			NEXT_SPRITE();
			g_SprWritePtr->y = y + 0x80;
			g_SprWritePtr->size = 0;
			g_SprWritePtr->attr = c - 0x20 + 0x6640;
			g_SprWritePtr->x = x + 0x80;
			s++;
			x += 8;
		}
		Spr_Draw(SPR, offX + 0x3C, offY + 0x40);
		Cerebro_BigSpriteDraw(BS);
		Spr_EndFrame();
		g_DmaFlushReq = 1;
		n = 0x258;
		while (n) {
			g_FrameTick = 0;
			--n;
			VDP_WaitVBlank();
			if ((g_Pad1Raw | g_Pad2Raw) & 0xF0) {
				g_ProgressFlags |= 2;
				n = 0;
			}
		}
	} else {
		n = 0x258;
		while (n) {
			g_FrameTick = 0;
			--n;
			VDP_WaitVBlank();
			if ((g_Pad1Raw | g_Pad2Raw) & 0xF0)
				n = 0;
		}
	}
	Spr_BeginFrame();
	NEXT_SPRITE();
	g_SprWritePtr->y = 0x1C;
	g_SprWritePtr->size = 0;
	g_SprWritePtr->attr = 0;
	g_SprWritePtr->x = 0x1C;
	g_FrameTick = 0;
	Spr_EndFrame();
	g_VramTileCursor = savedTiles;
	g_VBlankGameHook = 0;
	Mem_StackFree(g_ScaleTileBuf);
	Mem_StackFree(g_ScaleSrcBitmap);
	g_Pad1Pressed = 0;
	g_Pad2Pressed = 0;
	g_Pad1Latch = 0;
	g_Pad2Latch = 0;
#ifndef XMEN2_MPW
	xm::sp_restore(a7);
#endif
}
