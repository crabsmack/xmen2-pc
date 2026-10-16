/* ControlPanel_StartPattern - CONTROLPANEL class slot +$72 (its own): start its firing pattern
 * (Art_ControlPanelNem: 32 steps) at step 0, for that step's time; stage 5 (firing: its Draw
 * shows the pattern's light).
 *
 * self: the panel. ControlPanel_StateFire restarts it after the last step. */
#include "xmen2.h"

void ControlPanel_StartPattern(ControlPanel *self)
{
	register PanelStep *s;

	self->pattern = (PanelPattern *)CODEPTR(Art_ControlPanelNem);
	self->stepCount = self->pattern->size / 4;
	self->step = 0;
	s = &self->pattern->step[self->step];
	self->countdown = s->time;
	self->stage = 5;
}
