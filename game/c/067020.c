/* Failure_RunScene - the scene 'CerebroScreen Failure' (game over): the Cerebro screen shows the
 * failure text (Str_FailureText, no intro, no player select); then the screen and the whole
 * plane are cleared, every colour black, the "Game Over" art (Art_FailureNem2, Nemesis) loaded
 * at the free VRAM tiles with palette line 2 = Pal_SceneDefaultLine1, and its 14x2 tilemap
 * (Art_FailureNem, tiles offset by the art's first tile, palette line 2) written at VRAM $059A;
 * the palette fades in (Tbl_FadeBlack), stays $4B0 frames (20 s) or until a button (A, B, C,
 * Start on either pad), fades out; then a new game (Game_ResetPlayers, progress flags 0).
 * PC: the tilemap buffer is game memory (generated code reads it): placed on the 68000 stack
 * where the original's frame had it (-$38(a6), a6 = A7 at entry - 4), the rest of the frame (8
 * bytes of locals, 6 saved registers) reserved below it. */
#include "xmen2.h"

#define g_CerebroIntro		(*(s16 *)g_SceneVar3B0C)	/* scene variable: Cerebro_Run plays the intro first */
#define g_CerebroSelect		(*(s16 *)g_SceneVar3B0A)	/* scene variable: Cerebro_Run runs the player select after the text */
#define g_CerebroScript		(*(PTR(void) *)g_SceneVar3B14)	/* scene variable: the text script Cerebro_Run runs (0: none) */

void Failure_RunScene(void)
{
	s16 *src;
	s16 *dst;
#ifdef XMEN2_MPW
	s16 buf[28];
#else
	unsigned long a7 = CPU_GetSP();
	s16 *buf = AT68(s16, xm::sp_reserve(4 + 0x38));
#endif
	register s16 *map = buf;
	register short i;
	register s16 *p;
	register long j;
	register short tile;
	register short n;

#ifndef XMEN2_MPW
	xm::sp_reserve(8 + 24);
#endif
	g_CerebroIntro = 0;
	g_CerebroSelect = 0;
	g_CerebroScript = CODEPTR(Str_FailureText);
	Cerebro_Run();
	Cerebro_ClearScreen();
	g_TextWinTop = 0;
	g_TextWinLeft = 0;
	g_TextWinBottom = 0x1F;
	g_TextWinRight = 0x27;
	Cerebro_ClearTextWindow();
	for (i = 0, p = g_PalBase; i < 64; i++, p++)
		*p = 0;
	g_PalBaseDirty = 1;
	VDP_WaitVBlank();
	Art_LoadNemToVram(CODEPTR(Art_FailureNem2), g_VramTileCursor);
	src = (s16 *)CODEPTR(Pal_SceneDefaultLine1);
	dst = &g_PalBase[0x20];
	for (j = 0; j < 16; j++)
		*dst++ = *src++;
	tile = g_VramTileCursor | 0x4000;
	n = 0x1C;
	Mem_Copy(map, CODEPTR(Art_FailureNem), n << 1);
	for (i = 0, p = &map[0]; i < n; i++, p++)
		*p += tile;
	VDP_WriteTileRect(0x59A, map, 0xE, 2, 0x40);
	Pal_StartFade(0, -1, CODEPTR(Tbl_FadeBlack));
	i = 0x4B0;
	while (i) {
		--i;
		if ((g_Pad1Raw | g_Pad2Raw) & 0xF0)
			i = 0;
		VDP_WaitVBlank();
	}
	Pal_StartFade(7, -1, CODEPTR(Tbl_FadeBlack));
	while (g_PalFadeActive)
		VDP_WaitVBlank();
	Game_ResetPlayers();
	g_ProgressFlags = 0;
#ifndef XMEN2_MPW
	xm::sp_restore(a7);
#endif
}
