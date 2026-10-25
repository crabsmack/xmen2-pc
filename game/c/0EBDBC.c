/* CoreZap_Update - COREZAP Update (class slot +$4A; the parent's isn't called): killed (class slot
 * +$52) once its screen position is outside the deactivation window (g_DeactWin*); otherwise it
 * moves by its x speed once per step (2 in two-player step mode, else 1), takes the hit box 16
 * px either side of it from 5 px above to 10 px below, and hits each player it touches
 * (CoreZap_HitActor).
 *
 * self: the zap. */
#include "xmen2.h"

void CoreZap_Update(register CoreZap *self)
{
	register short steps = g_TwoPlayerStep ? 2 : 1;
	register short sx = (self->x >> 8) - g_CameraX;
	register short sy = (self->y >> 8) - g_CameraY;

	if (sx > g_DeactWinRight || sx < g_DeactWinLeft || sy > g_DeactWinBottom || sy < g_DeactWinTop) {
		(*self->cls->method[19])(self);		/* +$52 Kill */
		return;
	}
	do
		self->x += self->vx;
	while (--steps);
	sx = self->x >> 8;
	sy = self->y >> 8;
	self->box2Left = sx - 16;
	self->box2Right = sx + 16;
	self->box2Top = sy - 5;
	self->box2Bottom = sy + 10;
	CoreZap_HitActor(self, (Actor *)&g_Player1);
	CoreZap_HitActor(self, (Actor *)&g_Player2);
#if defined(XMEN2_NATIVE)
	P34_Each([&](XPlayer *q) {	/* players 3-4 in play too (R11) */
		CoreZap_HitActor(self, (Actor *)q);
	});
#endif
}
