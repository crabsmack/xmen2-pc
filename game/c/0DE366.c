/* ControlPanel_StateFire - CONTROLPANEL state (destroyed): step its firing pattern each time the
 * step's time runs out (restarting it after the last step), and fire a blast from its position
 * every fireInterval frames (the first fireTimer frames after ControlPanel_StateArm).
 *
 * self: the panel. */
#include "xmen2.h"

void ControlPanel_StateFire(ControlPanel *self)
{
	register PanelStep *s;
	register long i;

	if (!self->countdown) {
		i = ++self->step;
		if (i == self->stepCount) {
			(*self->cls->method[27])(self);	/* +$72 ControlPanel_StartPattern */
			i = self->step;
		}
		s = &self->pattern->step[i];
		self->countdown = s->time;
	}
	if (!self->fireTimer) {
		self->fireTimer = self->fireInterval;
		(*self->cls->method[28])(self, self->x, self->y);	/* +$76 ControlPanel_FireBlast */
	}
	self->fireTimer--;
}
