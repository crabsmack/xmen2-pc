/* RollingDiscs_UpdateScroll - the Moving Blocks' scroll hook (g_LevelScrollHook, every level
 * frame): the H-scroll of the rolling-disc bands, and their hits on the players.
 *
 * Every line of the H-scroll buffer first gets the plain scroll (-g_FgScrollX & $1FF, both
 * planes). Then each band (g_DiscBands): its scroll moves by its speed, the speed by its
 * acceleration (sound 3 when that turns it round), kept within +-maxSpeed. A band less than $80
 * lines above the screen and above its bottom: its 128 lines (clipped to the screen) of plane B
 * get the band's scroll added (for a band that scrolls; g_DiscBandReverse[index] = 1 while it
 * rolls right), and unless the hurt cooldown runs (g_DiscHurtCooldown, counted down here) each
 * player alive and not in hit cooldown whose body box meets the band's box (x from its last line's
 * scroll + x, $80 square less 16 each side) takes 3 damage of type 10 (rolling right) or 9, and
 * the cooldown restarts at $DC. Then the buffer is queued to VRAM $F400 ($380 bytes) and a VSRAM
 * upload requested.
 * MPW: the ROM widens the && of HURTABLE twice (ext.w, ext.l) before testing it, as through a
 * short. Each hit block keeps 8 bytes of stack for two locals only compiled-out code used (the
 * hit point, probably: TakeDamage gets 0, 0). */
#include "xmen2.h"

/* p can be hurt: present and alive, not in hit cooldown */
#define HURTABLE(p)	((long)(short)(IS_ALIVE(p) && !(p)->hitCooldown))

void RollingDiscs_UpdateScroll(void)
{
	short old;
	long count;
	XPlayer *p1;
	XPlayer *p2;
	register u32 *buf;
	register long fill;
	register long off;
	register DiscBand *band;
	register long sy;
	register long speed;
	register s16 *line;
	register long n;

	p1 = HURTABLE(&g_Player1) ? &g_Player1 : 0;
	p2 = HURTABLE(&g_Player2) ? &g_Player2 : 0;
	buf = g_HScrollBuf;
	fill = -g_FgScrollX;
	count = 0xE0;
	fill = (fill & 0x1FF) << 16 | fill & 0x1FF;
	do
		*buf++ = fill;
	while (--count);
	for (count = g_DiscBandCount, band = g_DiscBands; count; count--, band++) {
		sy = band->y - g_CameraY;
		old = speed = band->speed;
		band->scroll += speed;
		speed += band->accel;
		if (speed >= 0 && old < 0 || speed < 0 && old >= 0) {
			Snd_Stop(3);
			Snd_Play(3);
		}
		if (speed > band->maxSpeed)
			speed = band->maxSpeed;
		else if (speed < -band->maxSpeed)
			speed = -band->maxSpeed;
		band->speed = speed;
		if (sy > -0x80 && sy < 0xE0) {
			line = (s16 *)g_HScrollBuf + 1;		/* plane B of line 0 */
			n = 0x80;
			off = (band->scroll >> 8) & 0x1FF;
			if (sy < 0)
				n += sy;
			else if (sy > 0x60) {
				long top = sy;

				n -= sy - 0x60;
				line += top << 1;
			} else
				line += sy << 1;
			if (band->scrolls) {
				do {
					*line += off;
					*line &= 0x1FF;
					line += 2;
				} while (--n);
				g_DiscBandReverse[band->index] = band->speed > 0 ? 1 : 0;
				line -= 2;
			}
			if (g_DiscHurtCooldown) {
				--g_DiscHurtCooldown;
				continue;
			}
			{
				short x1 = g_CameraX + ((band->x + *line) & 0x1FF);
				short x2 = x1 + 0x80;
				short y1 = g_CameraY + sy;
				short y2 = y1 + 0x80;

				x1 += 16;
				x2 -= 16;
				y1 += 16;
				y2 -= 16;
				if (p1 && x1 <= p1->boxRight && x2 >= p1->boxLeft && y1 <= p1->boxBottom
				    && y2 >= p1->boxTop) {
					long hx, hy;
					short dir = band->speed > 0 ? 10 : 9;

					if (0) {
						hx = x1;
						hy = y1;
					}
					(*p1->cls->method[8])(p1, 0, dir, 3, 0, 0);	/* +$26 TakeDamage */
					g_DiscHurtCooldown = 0xDC;
				}
				if (p2 && x1 <= p2->boxRight && x2 >= p2->boxLeft && y1 <= p2->boxBottom
				    && y2 >= p2->boxTop) {
					long hx, hy;
					short dir = band->speed > 0 ? 10 : 9;

					if (0) {
						hx = x1;
						hy = y1;
					}
					(*p2->cls->method[8])(p2, 0, dir, 3, 0, 0);	/* +$26 TakeDamage */
					g_DiscHurtCooldown = 0xDC;
				}
#if defined(XMEN2_NATIVE)
				P34_Each([&](XPlayer *q) {	/* players 3-4 in play too (R11) */
					if (HURTABLE(q) && x1 <= q->boxRight && x2 >= q->boxLeft && y1 <= q->boxBottom
					    && y2 >= q->boxTop) {
						(*q->cls->method[8])(q, 0, band->speed > 0 ? 10 : 9, 3, 0, 0);	/* +$26 TakeDamage */
						g_DiscHurtCooldown = 0xDC;
					}
				});
#endif
			}
		}
	}
	VDP_QueueDma(g_HScrollBuf, 0xF400, 0x380);
	Scroll_RequestVsram();
}
