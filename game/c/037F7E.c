/* ThronePhone_StatePullIn - THRONEPHONE state: pull the boss in, each frame by 1/128 of the x gap
 * and 1/64 of the y gap; once his position is within x - 10 .. x + 10, y - 1 .. y + 10 (pixels)
 * he is put exactly on the beam, starts his outro (MagnetoBoss_EnterOutroState), the scene ends
 * in 240 frames (Scene_EndAfter), and the beam rests (ThronePhone_StateDone).
 *
 * self: the beam. Returns self->flags & 8 when done, no value while pulling (or with no boss). */
#include "xmen2.h"

short ThronePhone_StatePullIn(register ThronePhone *self)
{
	register MagnetoBoss *boss = self->boss;

	if (boss) {
		register long dx = self->x - boss->x;
		register long dy = self->y - boss->y;

		dx >>= 7;
		dy >>= 6;
		boss->x += dx;
		boss->y += dy;
		{
			register short bx = boss->x >> 8;
			register short by = boss->y >> 8;
			short left = (self->x >> 8) - 10;
			short top = (self->y >> 8) - 1;
			register short right = (self->x >> 8) + 10;
			register short bottom = (self->y >> 8) + 10;

			if (left <= bx && bx <= right && top <= by && by <= bottom) {
				boss->x = self->x;
				boss->y = self->y;
				MagnetoBoss_EnterOutroState(boss);
				Scene_EndAfter(240);
				SET_STATE(self, (StateFn)ThronePhone_StateDone);
				return self->flags & 8;
			}
		}
	}
	NO_RETURN_VALUE
}
