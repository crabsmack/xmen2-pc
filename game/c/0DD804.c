/* ControlBox_StartPattern - CONTROLBOX class slot +$6E (its own): start its firing pattern
 * (Tbl_ControlBoxPattern, 4-byte steps) at step 0, for that step's time; stage 1 (firing: its Draw
 * shows the pattern's light).
 *
 * self: the box. ControlBox_StateFire restarts it after the last step. */
#include "xmen2.h"

void ControlBox_StartPattern(ControlBox *self)
{
	register PanelStep *s;

	self->pattern = (PanelPattern *)CODEPTR(Tbl_ControlBoxPattern);
	self->stepCount = self->pattern->size / 4;
	self->step = 0;
	s = &self->pattern->step[self->step];
	self->countdown = s->time;
	self->stage = 1;
}
