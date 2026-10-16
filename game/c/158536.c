/* FluxBomb_StartImplode - start a flux bomb's implosion (phase 1): the particle tiles
 * (Art_FluxBombTiles, Art_FluxBombTiles2) go to the last two VRAM tiles of g_FluxBombSpr, and each
 * particle of Tbl_FluxBombImplodeParticles (the same table for a strong bomb) is shown at its
 * start, -(dx, dy) << shift; the ones with no step are hidden.
 *
 * self: the bomb. */
#include "xmen2.h"

/* A particle entry with no step (its two step bytes as one word) */
#define NO_STEP(s)	(!*(s16 *)(s))

void FluxBomb_StartImplode(FluxBomb *self)
{
	register FluxParticle *p = self->part;
	register FluxImplode *s = (FluxImplode *)CODEPTR(Tbl_FluxBombImplodeParticles);
	register short i;

	/* s's first load above is dead (the original's, kept). Strong: jt entry 2288, weak: 2287;
	   both are Tbl_FluxBombImplodeParticles. */
	s = self->strong ? (FluxImplode *)CODEPTR(Tbl_FluxBombImplodeParticles)
		: (FluxImplode *)CODEPTR(Tbl_FluxBombImplodeParticles);
	self->phase = 1;
	VDP_QueueDma(CODEPTR(Art_FluxBombTiles), (g_FluxBombSpr.tileBase + g_FluxBombSpr.tileCount - 1) << 5, 0x20);
	VDP_QueueDma(CODEPTR(Art_FluxBombTiles2), (g_FluxBombSpr.tileBase + g_FluxBombSpr.tileCount - 2) << 5, 0x20);
	for (i = 0; i < 16; i++) {
		if (NO_STEP(s)) {
			p->flags &= ~1;
		} else {
			p->x = -(s->dx << s->shift);
			p->y = -(s->dy << s->shift);
			p->flags |= 1;
			s++;
		}
		p++;
	}
}
