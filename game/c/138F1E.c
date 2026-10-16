/* MaintRobot_StateChase - MAINTROBOT state (chase mode): drive along its floor segment towards
 * the nearest player; stop to fire (MaintRobot_StateStop from the next update) once its countdown
 * has run out and the player is in range (MaintRobot_TargetInRange), or when it is blocked (flags
 * bit $100, cleared) with the countdown run out.
 *
 * self: the object.
 * The velocity along the segment (vx = +/-speed, or 0 level with the player) comes from the
 * segment's type: $21 the player's box right / left of the robot's (case
 * MaintRobot_StateChase_Case0), $22 the same reversed (MaintRobot_StateChase_Case1), $23 the
 * robot's line y - 48 below / above the player's box shrunk by 16 px (in Case0's code), $24 that
 * reversed (MaintRobot_StateChase_Case3); other types: no change. All end at
 * MaintRobot_StateChase_End.
 * Returns flags bit 3 when it stops; no value while it drives. */
#include "xmen2.h"

short MaintRobot_StateChase(register MaintRobot *self)
{
	register XPlayer *p;
	register short speed;
	register long top;		/* its line y - 48, pixels */

	p = Player_Nearest((Actor *)self);
	speed = self->speed;
	if (self->countdown <= 0 && MaintRobot_TargetInRange(self, (Actor *)p)) {
		SET_STATE(self, MaintRobot_StateStop);
		return self->flags & 8;
	}
	if ((self->flags & 0x100) && self->countdown <= 0) {
		self->flags &= ~0x100;
		SET_STATE(self, MaintRobot_StateStop);
		return self->flags & 8;
	}
	switch (self->seg->type) {
	case 0x21:	/* MaintRobot_StateChase_Case0 */
		self->vx = p->boxLeft > self->boxRight ? speed
			: p->boxRight < self->boxLeft ? -speed : 0;
		break;
	case 0x23:
		top = (self->y >> 8) + -0x30;
		self->vx = p->boxBottom - 0x10 < top ? speed
			: p->boxTop + 0x10 > top ? -speed : 0;
		break;
	case 0x22:	/* MaintRobot_StateChase_Case1 */
		self->vx = p->boxLeft > self->boxRight ? -speed
			: p->boxRight < self->boxLeft ? speed : 0;
		break;
	case 0x24:	/* MaintRobot_StateChase_Case3 */
		top = (self->y >> 8) + -0x30;
		self->vx = p->boxBottom - 0x10 < top ? -speed
			: p->boxTop + 0x10 > top ? speed : 0;
		break;
	}
	NO_RETURN_VALUE		/* MaintRobot_StateChase_End */
}
