/* SavageBioBalls_ActMotion - BIOBALLS script action (the SAVAGEBIGBIO's balls form): all balls
 * where the first is (BioBalls_FillFromFirst), then a ring (BioBalls_MotionRing, radius $600 from
 * angle 0, anchor 32 px up, ballFlags bit $100: easing in) for 150 frames
 * (SavageBioBalls_StateWait from the next update).
 *
 * self: the balls (a step of their AI script). Returns self->flags & 8. */
#include "xmen2.h"

short SavageBioBalls_ActMotion(register BioBalls *self)
{
	BioBalls_FillFromFirst(self);
	self->pathDy = -0x20;
	self->motion = BioBalls_MotionRing;
	self->motionAngle = 0;
	self->motionSize = 0x600;
	self->ballFlags |= 0x100;
	self->countdown = 0x96;
	SET_STATE(self, SavageBioBalls_StateWait);
	return self->flags & 8;
}
