/* SentinelsB2_FrameHook - the Sentinels escape's scene hook (g_SceneFrameHook), once per frame.
 *
 * Counts the frame (twice in two-player step mode); every 6 counted frames the escape countdown
 * (tenths of a second) goes down by 1, not below 0, and the timer starts showing (mode 1).
 * Unless a fade runs: the warning light (SentinelsD1_AnimLight) and the alarm flash (twice in
 * two-player step mode); while showing, the countdown is drawn at screen y $148 - 128 as five
 * 8x8 sprites straight into the sprite table (digit tiles from g_DigitTile, palette 1, high
 * priority): thousands (only when not 0), hundreds (when it or the thousands digit isn't 0, but
 * the thousands digit drawn doesn't count), tens, a point (tile 10), tenths. At 0: mode 1 starts
 * a fade to inverted white (Tbl_FadeInvertWhite, mode 2); mode 2 turns all 64 palette entries
 * white, fades to black (Tbl_FadeBlack) with the fade timer $3C and delay 8, the scene ends in
 * $78 frames and both players lose their health (mode 3); mode 3 clears the palette and ends the
 * scene at once.
 * The frame counter itself is kept below 6. Returns nothing (no caller reads d0). */
#include "xmen2.h"

#define g_EscapeTime	(*(s16 *)g_SceneVar3B02)	/* scene variable: escape countdown, tenths of a second */
#define g_DigitTile	(*(s16 *)g_SceneVar3B06)	/* scene variable: VRAM tile of the digit 0 (SentinelsB2_LoadDigits) */
#define g_EscapeMode	(*(s16 *)g_SceneVar3B08)	/* scene variable: 0 counting, 1 showing, 2 white fade, 3 black fade */

/* One 8x8 digit sprite (tile t from the digit tiles, palette 1, priority) at screen x X. */
#define EMIT_DIGIT(t, X) \
	attr = g_DigitTile + (t) | 0x2000 | 0x8000; \
	g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link); \
	g_SprWritePtr->y = 0x148; \
	g_SprWritePtr->size = 0; \
	g_SprWritePtr->attr = attr; \
	g_SprWritePtr->x = (X)

void SentinelsB2_FrameHook(void)
{
	register char shown;
	register short rest;
	register short attr;
	register short i;
	register s16 *p;
	register s16 *q;

	g_FrameCounter++;
	if (g_TwoPlayerStep)
		g_FrameCounter++;
	while (g_FrameCounter >= 6) {
		g_FrameCounter -= 6;
		--g_EscapeTime;
		if (g_EscapeTime < 0)
			g_EscapeTime = 0;
		if (!g_EscapeMode)
			g_EscapeMode = 1;
	}
	if (g_PalFadeActive)
		return;
	SentinelsD1_AnimLight();
	SentinelsB2_AlarmPalette();
	if (g_TwoPlayerStep) {
		SentinelsD1_AnimLight();
		SentinelsB2_AlarmPalette();
	}
	if (g_EscapeMode == 1) {
		shown = 0;
		rest = g_EscapeTime;
		attr = rest / 1000;
		rest = rest - attr * 1000;
		if (attr || shown) {
			EMIT_DIGIT(attr, 0x116);
			shown = 1;
		}
		attr = rest / 100;
		rest = rest - attr * 100;
		if (attr || shown) {
			EMIT_DIGIT(attr, 0x11E);
		}
		attr = rest / 10;
		rest = rest - attr * 10;
		EMIT_DIGIT(attr, 0x126);
		EMIT_DIGIT(10, 0x12D);
		attr = rest;
		EMIT_DIGIT(attr, 0x134);
	}
	if (!g_EscapeTime) {
		switch (g_EscapeMode) {
		case 1:
			g_EscapeMode = 2;
			Pal_StartFade(7, -1, CODEPTR(Tbl_FadeInvertWhite));
			break;
		case 2:
			g_EscapeMode = 3;
			for (i = 0, p = g_PalBase; i < 0x40; i++)
				*p++ = 0xEEE;
			Pal_StartFade(7, -1, CODEPTR(Tbl_FadeBlack));
			g_PalBaseDirty = 0;
			g_PalFadeTimer = 0x3C;
			g_PalFadeDelay = 8;
			g_SceneExitTimer = 0x78;
			g_Player1.health = 0;
			g_Player2.health = 0;
#if defined(XMEN2_NATIVE)
			for (int k = 3; k <= g_opt.players; k++)
				PLAYER34(k)->health = 0;	/* phase 7: players 3-4 in play too (R11) */
#endif
			break;
		case 3:
			for (i = 0, q = g_PalBase; i < 0x40; i++, q++)
				*q = 0;
			g_PalBaseDirty = 1;
			g_SceneExitTimer = 1;
			break;
		}
	}
}
