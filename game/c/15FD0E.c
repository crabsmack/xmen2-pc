/* Scroll_ComputeParallax - fill the scroll buffer from the parallax descriptor and the camera.
 *
 * Nothing to do without a descriptor (g_ParallaxDesc 0). A line descriptor fills g_HScrollBuf
 * with one H-scroll long per screen line {plane A, plane B}: plane A follows g_FgScrollX, plane
 * B is set band by band, from the band holding plane B's top line (g_BgScrollY) on until 224
 * lines are written. A column descriptor (type '2') writes one V-scroll long per 2-column strip
 * {plane A g_FgScrollY + g_FgScrollAdjY, plane B per band} for each of its `count` bands.
 * Plane B per band mode, with the band's g_ParallaxBands state:
 *   0  `speed` (8.8) times the foreground scroll (columns: plus g_BgScrollBaseY);
 *   1  a gradient: the speed changes each line by the state's step (Scroll_SetParallax);
 *   2  drift: moves by `speed` each frame (state drift, in 1/64 pixel), each line by the state's
 *      line step, which changes each frame by the band's first parameter byte, plus the
 *      camera times the second byte (8.8); columns: the drift, moved by `param` each frame;
 *   3  a wave: per-line offsets from a ParallaxWave, its index stepped every `period` frames;
 *   4  drift without the camera.
 * Any other mode ends the walk. Kept from the 68000:
 *   - a line descriptor's count only bounds the search for the first band: past it the walk
 *     goes on band by band (the dbra's flags never stop it) until 224 lines are written;
 *   - a mode 3 band runs mode 4's code after its own (the 68000 has no branch between them):
 *     its lines are written twice, the second time as a drift band;
 *   - on a frame where the wave steps, its lines get plane A scroll 0 (the 68000 reloads the
 *     register that held plane A's value);
 *   - for a gradient band starting above the screen, the step's two words are multiplied by the
 *     lines above apart, its low word taken as signed;
 *   - column descriptors don't search, so their modes 1 and 3 would read a line offset the
 *     68000 never set (the C uses 0; no descriptor has them).
 * Hand-written 68000. */
#include "xmen2.h"

void Scroll_ComputeParallax(void)
{
	ParallaxDesc *desc = g_ParallaxDesc;
	ParallaxBand *band;
	ParallaxState *st;
	ParallaxWave *wave;
	u32 *out;
	unsigned long fg;		/* plane A's H-scroll (line descriptors) */
	unsigned long a, v, acc, w;
	long step;
	unsigned short count, k;
	short y;			/* line descriptors: the band's lines above the screen */
	short start, end, lines, skipped, base, rate, idx, t;
	char seek;

	if (!desc)
		return;
	band = desc->band;
	st = g_ParallaxBands;
	out = g_HScrollBuf;
	count = desc->count;
	fg = (unsigned short)-g_FgScrollX & 0x1FF;
	y = 0;
	seek = desc->type != '2';
	if (seek)
		y = g_BgScrollY;
	for (;;) {
		if (seek) {
			/* the band holding plane B's top line */
			for (;;) {
				y = (short)(y - band->height);
				if (y < 0)
					break;
				band++;
				st++;
				if (count-- == 0 && y == 0)
					return;
			}
			y = (short)(y + band->height);
		}
		/* the band's lines on the screen (line descriptors) */
		start = (short)-y;
		end = (short)(start + band->height);
		skipped = 0;
		if (start < 0) {
			skipped = start;
			start = 0;
		}
		if (end > 224)
			end = 224;
		lines = (short)(end - start);
		switch (band->mode) {
		case 0:
			if (desc->type == '2') {
				v = ((unsigned long)(unsigned short)g_FgScrollY * (unsigned short)band->speed >> 8)
				    + g_BgScrollBaseY;
				v = (unsigned long)((g_FgScrollY + g_FgScrollAdjY) & 0xFF) << 16 | (unsigned short)v;
				k = band->height;
				do
					*out++ = v;
				while (--k);
			} else {
				v = fg << 16 | ((unsigned short)-(short)((unsigned long)(unsigned short)g_FgScrollX
				                                         * (unsigned short)band->speed >> 8) & 0x1FF);
				/* (8 at a time in the 68000: a negative count would run 65536 times 8) */
				for (w = (unsigned short)(lines >> 3) * 8L + (lines & 7); w != 0; w--)
					*out++ = v;
			}
			break;
		case 1:
			acc = (unsigned long)(unsigned short)((unsigned long)(unsigned short)g_FgScrollX
			                                      * (unsigned short)band->speed >> 8) << 12;
			step = (long)st->rate * g_FgScrollX;
			if (y > 0) {
				acc += (unsigned long)((long)(short)((unsigned long)step >> 16) * y) << 16;
				acc += (long)(short)step * y;
			}
			k = lines;
			do {
				*out++ = fg << 16 | ((unsigned short)-(short)(acc >> 12) & 0x1FF);
				acc += step;
			} while (--k);
			break;
		case 3:
			wave = (ParallaxWave *)((u8 *)desc + band->param);
			base = (short)((unsigned long)(unsigned short)g_FgScrollX * (unsigned short)band->speed >> 8);
			a = fg;
			idx = st->rate;
			t = (short)(st->drift - 1);
			if (t <= 0) {
				t = wave->period;
				a = 0;
				idx++;
				if ((signed char)idx >= (signed char)wave->len)
					idx = 0;
			}
			st->drift = t;
			st->rate = idx;
			if (skipped != 0) {
				/* start the wave at the band's first line on the screen */
				idx = (short)(idx - skipped);
				while (idx >= wave->len)
					idx = (short)(idx - wave->len);
			}
			k = lines;
			do {
				*out++ = a << 16 | ((unsigned short)-(short)(wave->offset[idx] + base) & 0x1FF);
				idx++;
				if ((signed char)idx >= (signed char)wave->len)
					idx = 0;
			} while (--k);
			/* fall through: the 68000 goes on into mode 4's code */
		case 2:
		case 4:
			if (desc->type == '2') {
				st->drift += band->param;
				v = (unsigned long)((g_FgScrollY + g_FgScrollAdjY) & 0xFF) << 16
				    | (unsigned short)(st->drift >> 6);
				k = band->height;
				do
					*out++ = v;
				while (--k);
			} else {
				base = (short)((unsigned short)((unsigned long)(unsigned short)(band->mode == 2 ? g_FgScrollX : 0)
				                                * (unsigned short)(signed char)band->param >> 8) << 6);
				st->drift -= band->speed;
				base = (short)(base + st->drift);
				st->rate -= (signed char)(band->param >> 8);
				rate = st->rate;
				k = lines;
				do {
					*out++ = fg << 16 | ((unsigned short)-(base >> 6) & 0x1FF);
					base = (short)(base + rate);
				} while (--k);
			}
			break;
		default:
			return;
		}
		/* the next band */
		if (desc->type == '2') {
			band++;
			st++;
			if (--count == 0)
				return;
		} else {
			y = (short)(y - band->height);
			if ((unsigned short)y <= 0xFF20)	/* 224 lines written (or a band ended above the screen) */
				return;
			band++;
			st++;
			seek = count-- != 0;	/* run out: straight to the next band */
		}
	}
}
