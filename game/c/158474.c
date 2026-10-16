/* FluxBomb_StepImplode - one step of a flux bomb's implosion (phase 1): each shown particle moves
 * a step toward the centre (Tbl_FluxBombImplodeParticles, the same table for a strong bomb) and
 * is hidden once within 8 px of it in both x and y; when none moved, the implosion is over
 * (phase 2).
 *
 * self: the bomb (FluxBomb_Update). */
#include "xmen2.h"

void FluxBomb_StepImplode(FluxBomb *self)
{
	register FluxParticle *p = self->part;
	register char done = 1;
	register FluxImplode *s = (FluxImplode *)CODEPTR(Tbl_FluxBombImplodeParticles);
	register short i;
	register short x;
	register short y;

	/* s's first load above is dead (the original's, kept). Strong: jt entry 2288, weak: 2287;
	   both are Tbl_FluxBombImplodeParticles. */
	s = self->strong ? (FluxImplode *)CODEPTR(Tbl_FluxBombImplodeParticles)
		: (FluxImplode *)CODEPTR(Tbl_FluxBombImplodeParticles);
	for (i = 0; i < 16; i++) {
		if (p->flags & 1) {
			x = p->x + s->dx;
			y = p->y + s->dy;
			if (ABS(x) < 8 && ABS(y) < 8) {
				p->flags &= ~1;
			} else {
				p->x = x;
				p->y = y;
				done = 0;
			}
		}
		p++;
		s++;
	}
	if (done)
		self->phase = 2;
}
