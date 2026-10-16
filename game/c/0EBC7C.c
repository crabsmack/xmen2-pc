/* CoreShields_FireZap - fire a COREZAP (its New, class slot +$0E) 16 px left of and 26 px above
 * the shields, at x speed vx; with it sound $CD and the zap lights' chain starts (light 0 at
 * frame 0: SentinelsA1_StepZapLights).
 *
 * self: the shields.  vx: the zap's x speed (CoreShields_StateActive: -$200).
 * Returns the zap, 0 when none could be made (then no sound, no lights). */
#include "xmen2.h"

#define g_ZapLight0	(*(s16 *)g_SceneVar3B02)	/* scene variable: frame of zap light 0 (-1 idle) */

CoreZap *CoreShields_FireZap(register CoreShields *self, short vx)
{
	register long x = self->x + -0x1000;
	register long y = self->y + -0x1A00;
	register CoreZap *zap;

	zap = (CoreZap *)(*CLASS(Tbl_CoreZap_vtable)->method[2])(0,	/* New */
		CLASS(Tbl_CoreZap_vtable), x, y, 0);
	if (zap) {
		zap->vx = vx;
		Snd_Stop(0xCD);
		Snd_Play(0xCD);
		g_ZapLight0 = 0;
	}
	return zap;
}
