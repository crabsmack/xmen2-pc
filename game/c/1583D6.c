/* FluxBomb_StepBurst - one step of a flux bomb's burst (phase 3): its time (countdown) over, the
 * burst ends (phase 4); else more particles are out (burstCount += 100: all 16 at once) and each
 * shown one moves out along its direction (Tbl_FluxBombBurstDirs, the same table for a strong
 * bomb).
 *
 * self: the bomb (FluxBomb_Update). */
#include "xmen2.h"

void FluxBomb_StepBurst(FluxBomb *self)
{
	register FluxParticle *p = self->part;
	register FluxDir *d;
	register short n;
	register short i;

	if (self->countdown <= 0) {
		self->phase = 4;
		return;
	}
	if (self->strong)	/* jt entry 2290; weak: 2289; both are Tbl_FluxBombBurstDirs */
		d = (FluxDir *)CODEPTR(Tbl_FluxBombBurstDirs);
	else
		d = (FluxDir *)CODEPTR(Tbl_FluxBombBurstDirs);
	self->burstCount += 100;
	n = MIN(self->burstCount, 16);
	for (i = 0; i < n; i++) {
		if (p->flags & 1) {
			p->x += d->dx;
			p->y += d->dy;
		}
		p++;
		d++;
	}
}
