/* MaintRobot_TargetInRange - whether a player is in a MAINTROBOT's firing range.
 *
 * self: the robot.  p: the player.
 * Only while the robot is 64-224 px below the camera's top (g_CameraY). The player's body box
 * must reach into x - 192 .. x + 192 px of the robot and, on floor segments of type $21/$22,
 * its bottom must be at most 32 px above the robot's line y - 48 and its top at most 32 px below
 * y; on other types the line y - 48 must lie inside the player's box shrunk by 16 px at top and
 * bottom.
 * Returns 1 in range, else 0 (a char). */
#include "xmen2.h"

char MaintRobot_TargetInRange(register MaintRobot *self, register Actor *p)
{
	short x = self->x >> 8;
	register short y = self->y >> 8;
	register short left = x - 0xC0;
	register short top = y + -0x30;
	register short right = x + 0xC0;

	if (y - g_CameraY < 0x40)
		return 0;
	if (y - g_CameraY > 0xE0)
		return 0;
	switch (self->seg->type) {
	case 0x21:
	case 0x22:
		return left <= p->boxRight && right >= p->boxLeft
		    && top - 0x20 <= p->boxBottom && y + 0x20 >= p->boxTop;
	default:
		return left <= p->boxRight && right >= p->boxLeft
		    && top <= p->boxBottom - 0x10 && top >= p->boxTop + 0x10;
	}
}
