/* Intro_SegaLogoFlash - the SEGA logo's palette animation on one palette line: a sweep, a flash, a
 * fade and a second sweep; a button (A, B, C or Start on either pad) skips the last two.
 *
 * cb: run every frame while it waits (Intro_WaitFrame), or 0.  line: the palette line (0-3).
 * Pal_Update is the V-int hook meanwhile (the old hook comes back at the end). The line is
 * cleared; then colours 2-10 of it are written from the tables, the palette uploaded at the
 * next V-blank (g_PalBaseDirty):
 *   Tbl_SegaLogoFlash1: a step per word, up to 9 colours from it (to a negative word), 4 frames
 *     each; then 11 frames;
 *   Tbl_SegaLogoFlash2: each word fills all 9 colours (one before a $EEE also sets colour 1),
 *     2 frames each; then 7 frames;
 *   Tbl_SegaLogoFlash3, Tbl_SegaLogoFlash4: as the first, 5 frames each, a button ends it.
 * The line is cleared again for one frame.
 * Hand-written 68000 behind a C frame (dbra loops, bsr to Intro_WaitFrame, which reads cb from
 * this frame): it can't byte-match. */
#include "xmen2.h"

/* up to 9 colours from the step to colours 2-10, stopping at a negative word */
#define COPY_STEP(src, pal) do { \
	s16 *s_ = (src), *d_ = (pal) + 2; \
	short n_; \
	for (n_ = 0; n_ < 9 && *s_ >= 0; n_++) \
		*d_++ = *s_++; \
} while (0)

/* the line's 16 colours to 0, a long at a time */
#define CLEAR_LINE(pal) do { \
	short n_; \
	for (n_ = 0; n_ < 8; n_++) \
		((s32 *)(pal))[n_] = 0; \
} while (0)

void Intro_SegaLogoFlash(void *cb, short line)
{
	FrameHookFn saved;
	s16 *pal;
	s16 *step;
	short f;

	saved = g_VBlankHook;
	g_VBlankHook = Pal_Update;
	g_PalBaseDirty = 0;
	pal = g_PalBase + line * 16;
	CLEAR_LINE(pal);

	step = (s16 *)CODEPTR(Tbl_SegaLogoFlash1);
	do {
		COPY_STEP(step, pal);
		g_PalBaseDirty = 0xFF;
		for (f = 0; f < 4; f++)
			Intro_WaitFrame(FN_AT(IntroFrameFn, cb));
		step++;
	} while (*step >= 0);
	for (f = 0; f < 11; f++)
		Intro_WaitFrame(FN_AT(IntroFrameFn, cb));

	step = (s16 *)CODEPTR(Tbl_SegaLogoFlash2);
	do {
		short c = *step++;
		short n;

		for (n = 0; n < 9; n++)
			pal[2 + n] = c;
		if (*step == 0xEEE)
			pal[1] = 0xEEE;
		g_PalBaseDirty = 0xFF;
		for (f = 0; f < 2; f++)
			Intro_WaitFrame(FN_AT(IntroFrameFn, cb));
	} while (*step >= 0);
	for (f = 0; f < 7; f++)
		Intro_WaitFrame(FN_AT(IntroFrameFn, cb));

	step = (s16 *)CODEPTR(Tbl_SegaLogoFlash3);
	do {
		COPY_STEP(step, pal);
		g_PalBaseDirty = 0xFF;
		for (f = 0; f < 5; f++) {
			Intro_WaitFrame(FN_AT(IntroFrameFn, cb));
			if ((g_Pad1Raw | g_Pad2Raw) & 0xF0)
				goto done;
		}
		step++;
	} while (*step >= 0);
	step = (s16 *)CODEPTR(Tbl_SegaLogoFlash4);
	do {
		COPY_STEP(step, pal);
		g_PalBaseDirty = 0xFF;
		for (f = 0; f < 5; f++) {
			Intro_WaitFrame(FN_AT(IntroFrameFn, cb));
			if ((g_Pad1Raw | g_Pad2Raw) & 0xF0)
				goto done;
		}
		step++;
	} while (*step >= 0);
done:
	CLEAR_LINE(pal);
	g_PalBaseDirty = 0xFF;
	Intro_WaitFrame(FN_AT(IntroFrameFn, cb));
	g_VBlankHook = saved;
}
