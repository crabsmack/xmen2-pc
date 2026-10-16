/* ControlBox_StateFire - CONTROLBOX state (firing): step its firing pattern each time the step's
 * time runs out (restarting it after the last step), and fire a blast from its position every
 * fireInterval frames (the first fireTimer frames after ControlBox_StateArm).
 *
 * self: the box. */
#include "xmen2.h"

void ControlBox_StateFire(ControlBox *self)
{
	register PanelStep *s;
	register long i;

	if (!self->countdown) {
		i = ++self->step;
		if (i == self->stepCount) {
			(*self->cls->method[26])(self);	/* +$6E ControlBox_StartPattern */
			i = self->step;
		}
		s = &self->pattern->step[i];
		self->countdown = s->time;
	}
	if (!self->fireTimer) {
		self->fireTimer = self->fireInterval;
		(*self->cls->method[27])(self, self->x, self->y);	/* +$72 ControlBox_FireBlast */
	}
	self->fireTimer--;
}
