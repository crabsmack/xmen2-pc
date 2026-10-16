/* Scroll_SetParallax - start a scene's parallax: set up the scroll mode for the descriptor, reset
 * every band's state and clear the H-scroll buffer.
 *
 * desc: the parallax descriptor (Tbl_<Scene>Parallax; becomes g_ParallaxDesc).
 * Line scroll (any type but '2'): VDP register 11 = g_VdpReg11 + 3 (per-line H-scroll) and the
 * H-scroll table's free tiles ($7BE, $42 of them); column scroll ('2'): + 4 (column V-scroll).
 * Each band's state (g_ParallaxBands, a long each here) is 0, except a gradient band's (mode 1,
 * Scroll_SetParallax_Gradient): its speed's step per line, ((end - start) * $1000 >> 8) /
 * (height - 1) from the band's start (speed) and end (param) speeds. Modes 0, 2, 3
 * (Scroll_SetParallax_Band0, _Band2, _Band3) and others (Scroll_SetParallax_Next) clear it.
 * The H-scroll buffer's $1C0 words are cleared. */
#include "xmen2.h"

/* The original's file defined g_ParallaxBands: MPW C keeps the address of an array defined in
   the compile unit in a register (lea g_ParallaxBands(a5),a3, indexed .w; Pal_SaveBase). Its
   entries are longs here (Scroll_ComputeParallax reads their two words). */
#ifdef XMEN2_MPW
ParallaxState g_ParallaxBands[32];
#endif
#define BAND_STEP(i)	(*(s32 *)&g_ParallaxBands[i])

#define desc	((ParallaxDesc *)descp)

void Scroll_SetParallax(void *descp)
{
	register short i;
	register ParallaxBand *band;
	short count;

	if ((s8)desc->type != '2') {
		g_HScrollFreeTile = 0x7BE;
		g_HScrollFreeCount = 0x42;
		VDP_CTRL = g_VdpReg11 + 3;
	} else
		VDP_CTRL = g_VdpReg11 + 4;
	g_ParallaxDesc = desc;
	count = (s8)desc->count;
	band = (ParallaxBand *)((s16 *)desc + 1);	/* desc->band, past the type and count bytes */
	for (i = 0; i < count; band += 1, i++) {
		switch ((s8)band->mode) {
		case 0:					/* Scroll_SetParallax_Band0 */
			BAND_STEP(i) = 0;
			break;
		case 1: {				/* Scroll_SetParallax_Gradient */
			long end;
			ParallaxBand *b;
			register short lines;
			register short from = band->speed;
			register short to;

			b = band;
			to = b->param;
			lines = b->height - 1;
			end = Math_MulFix8(0x1000, to);
			BAND_STEP(i) = (end - (long)Math_MulFix8(0x1000, from)) / lines;
			break;
		}
		case 2:					/* Scroll_SetParallax_Band2 */
			BAND_STEP(i) = 0;
			break;
		case 3:					/* Scroll_SetParallax_Band3 */
			BAND_STEP(i) = 0;
			break;
		default:				/* Scroll_SetParallax_Next */
			BAND_STEP(i) = 0;
		}
	}
	for (i = 0; i < 0x1C0; i++)
		((s16 *)g_HScrollBuf)[i] = 0;
}
