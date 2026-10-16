/* MaintRobot_StateFire - MAINTROBOT state: fire one shot of a volley, a MAINTROBOTFIRE 19 px
 * ahead (towards the nearest player) and 47 px up, with sound $D2; the next in shotDelay frames.
 * With no shot left, MaintRobot_StateResume from the next update.
 *
 * self: the object.
 * The shot gets the robot's sprite priority and faces left (h-flip) when the player is left of
 * the robot. No shot object (New failed): no sound, but the shot is counted all the same.
 * Returns flags bit 3 when the volley is over; no value after a shot. */
#include "xmen2.h"

short MaintRobot_StateFire(register MaintRobot *self)
{
	register XPlayer *p;
	register MaintRobotFire *fire;
	register char left;		/* the player is left of the robot */
	register long fx;

	p = Player_Nearest((Actor *)self);
	left = p->x < self->x;
	fx = left ? self->x - 0x1300 : self->x + 0x1300;
	if (!self->shotsLeft) {
		SET_STATE(self, MaintRobot_StateResume);
		return self->flags & 8;
	}
	if (fire = (MaintRobotFire *)(*CLASS(Tbl_MaintRobotFire_vtable)->method[2])(0,
			CLASS(Tbl_MaintRobotFire_vtable), fx, self->y - 0x2F00, 0)) {	/* New */
		Snd_Stop(0xD2);
		Snd_Play(0xD2);
		if (self->spr.flags & 4 ? 1 : 0)
			fire->spr.flags |= 4;
		if (left)
			fire->spr.flags |= 0x10;
	}
	self->timer = self->shotDelay;
	self->shotsLeft--;
	NO_RETURN_VALUE
}
