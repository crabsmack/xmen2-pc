/* ThronePhone_SetMode - THRONEPHONE's own class slot +$6E: the beam's shape for mode 0 (also any
 * other value) or 1: 10 rows, their height, spacing, and the x sine / y wobble phase steps
 * (per frame, per row) and shifts.
 *
 * self: the beam.  mode: 0 or 1 (ThronePhone_StateCatch toggles it at each catch). */
#include "xmen2.h"

void ThronePhone_SetMode(register ThronePhone *self, short mode)
{
	switch (mode) {
	default:
	case 0:
		self->rows = 10;
		self->height = 0x94;
		self->rowStep = 14;
		self->angleXStep = 8;
		self->shiftX = 11;
		self->angleXRowStep = 0x94;
		self->angleYStep = 8;
		self->shiftY = 13;
		self->angleYRowStep = 8;
		break;
	case 1:
		self->rows = 10;
		self->height = 0x6E;
		self->rowStep = 7;
		self->angleXStep = 12;
		self->shiftX = 11;
		self->angleXRowStep = 0x94;
		self->angleYStep = 5;
		self->shiftY = 11;
		self->angleYRowStep = 8;
		break;
	}
}
