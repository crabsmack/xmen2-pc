/* FluxBomb_StartBurst - start a flux bomb's burst (phase 3) for 6 frames (15 when strong): the
 * blast hits (FluxBomb_DamageArea), no particle is out yet, the particle tiles go to VRAM again,
 * and each particle with a direction (Tbl_FluxBombBurstDirs, the same table for a strong bomb)
 * is shown at the centre; the ones without are hidden.
 *
 * self: the bomb (FluxBomb_Explode). */
#include "xmen2.h"

/* A direction entry of (0, 0) (its two bytes as one word) */
#define NO_STEP(d)	(!*(s16 *)(d))

void FluxBomb_StartBurst(FluxBomb *self)
{
	register FluxParticle *p = self->part;
	register FluxDir *d;
	register short i;

	if (self->strong) {
		d = (FluxDir *)CODEPTR(Tbl_FluxBombBurstDirs);
		self->countdown = 15;
	} else {
		d = (FluxDir *)CODEPTR(Tbl_FluxBombBurstDirs);
		self->countdown = 6;
	}
	self->phase = 3;
	FluxBomb_DamageArea(self);
	self->burstCount = 0;
	VDP_QueueDma(CODEPTR(Art_FluxBombTiles), (g_FluxBombSpr.tileBase + g_FluxBombSpr.tileCount - 1) << 5, 0x20);
	VDP_QueueDma(CODEPTR(Art_FluxBombTiles2), (g_FluxBombSpr.tileBase + g_FluxBombSpr.tileCount - 2) << 5, 0x20);
	for (i = 0; i < 16; i++) {
		if (NO_STEP(d)) {
			p->flags &= ~1;
		} else {
			p->x = 0;
			p->y = 0;
			p->flags |= 1;
			d++;
		}
		p++;
	}
}
