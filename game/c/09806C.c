/* EvilBeam_Update - EVILBEAM Update (class slot +$0E, run by its Timer every frame): step the
 * animation; once it has none (never set, or a one-shot that ended) destroy the beam (class slot
 * +$16), else hurt each live player whose second box holds the beam's point (TakeDamage: 12,
 * hit type 1).
 *
 * self: the beam.
 * An animation step lasts the script's delay in frames (counted 1 a frame, 2 in two-player
 * stepping); as LittleLeg_Update. A bug: the point's y is a local never set; the 68000 has 0 in
 * its register there (it shares the "done" flag's), so the test is against y = 0. A step count
 * is also computed and not used. */
#include "xmen2.h"

#define SCRIPT(o)	((AnimScript *)(u8 *)(o)->anim)	/* the animation header */

void EvilBeam_Update(EvilBeam *self)
{
	char done = 1;
	short steps = g_TwoPlayerStep ? 2 : 1;
	short y;

	if (self->anim) {
		self->animTime += g_TwoPlayerStep ? 2 : 1;
		if (self->animTime >= SCRIPT(self)->delay) {
			self->animStep++;
			if (SCRIPT(self)->count <= self->animStep) {
				self->animStep = 0;
				if (SCRIPT(self)->flags & 2)
					self->anim = 0;
			}
			if (self->anim) {
				self->spr.frame = *(self->anim + (self->animStep << 1) + 7);
				if (self->spr.frame == 0xFF || self->spr.frame != self->spr.uploaded)
					self->spr.flags &= ~0x20;
				self->animTime = 0;
			}
		}
		if (self->anim)
			done = 0;
	}
	if (done) {
		(*self->cls->method[4])(self);		/* +$16 Destroy */
	} else {
#ifndef XMEN2_MPW
		y = 0;		/* what the 68000 has in the register y shares with done */
#endif
		if (IS_ALIVE(&g_Player1) && self->x >= g_Player1.box2Left && self->x <= g_Player1.box2Right
		    && y >= g_Player1.box2Top && y <= g_Player1.box2Bottom)
			(*g_Player1.cls->method[8])(&g_Player1, 0, 0xC, 1, self->x, y);	/* +$26 TakeDamage */
		if (IS_ALIVE(&g_Player2) && self->x >= g_Player2.box2Left && self->x <= g_Player2.box2Right
		    && y >= g_Player2.box2Top && y <= g_Player2.box2Bottom)
			(*g_Player2.cls->method[8])(&g_Player2, 0, 0xC, 1, self->x, y);	/* +$26 TakeDamage */
#if defined(XMEN2_NATIVE)
		P34_Each([&](XPlayer *q) {	/* players 3-4 in play too (R11) */
			if (IS_ALIVE(q) && self->x >= q->box2Left && self->x <= q->box2Right
			    && y >= q->box2Top && y <= q->box2Bottom)
				(*q->cls->method[8])(q, 0, 0xC, 1, self->x, y);	/* +$26 TakeDamage */
		});
#endif
	}
}
