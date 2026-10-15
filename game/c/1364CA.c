/* Hornet_MoveSwing - HORNET motion (+$94) of a level-placed hornet: swing back and forth on the
 * lower half of a circle around a point beside the spawn position.
 *
 * self: the hornet.
 * The angle moves by swingStep each step between -180 and 0 degrees, turning round at either
 * end; x = spawn x + swingCentre + radius cos(angle), y = spawn y + radius sin(angle) / 2 (an
 * ellipse half as tall as it is wide). The spawn position is px/py, which only Init sets for a
 * hornet (Hornet_Move doesn't call the parent's Move). */
#include "xmen2.h"

void Hornet_MoveSwing(Hornet *self)
{
	short step = self->swingStep;
	short angle = self->swingAngle + step;
	short radius = self->swingRadius;

	if (step >= 0) {
		if (angle > 0) {
			self->swingStep = -self->swingStep;
			angle = 0;
		}
	} else if (angle < -180) {
		self->swingStep = -self->swingStep;
		angle = -180;
	}
	self->swingAngle = angle;
	self->x = ((self->px + self->swingCentre) << 8) + (radius * Math_CosDeg(angle) >> 8);
	self->y = (self->py << 8) + (radius * Math_SinDeg(angle) >> 9);
}
