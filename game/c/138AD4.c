/* MaintRobotFire_Update - MAINTROBOTFIRE Update (class slot +$4A; the parent's isn't called): only
 * its animation runs (it doesn't move: the distance passed is 0); a new frame sets flags 8. Killed
 * (slot +$52) once the animation has ended (no anim), else its boxes are recomputed.
 *
 * self: the flame. */
#include "xmen2.h"

void MaintRobotFire_Update(MaintRobotFire *self)
{
	register long x0, y0;
	register short step;

	if (g_TwoPlayerStep)
		step = 2;
	else
		step = 1;
	x0 = self->x;
	y0 = self->y;
	if ((char)Anim_AdvanceByDistance(self, step, self->x - x0, self->y - y0))
		self->flags |= 8;
	if (!self->anim)
		(*self->cls->method[19])(self);		/* +$52 Kill */
	else
		Obj_UpdateBoxes(self);
}
