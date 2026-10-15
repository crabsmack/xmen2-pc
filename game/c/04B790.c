/* Apocalypse_StateDropBombs - APOCALYPSE state: drop the bomb pattern. While at most 5 bombs are
 * live, spawn the next bomb of the pattern (ApocBomb_Spawn with the round's speed) and wait its
 * delay; at the pattern's end ($FFFF) select the next pattern (0-5, wrapping) for the next round
 * and wait for the bombs (Apocalypse_StateBombsDone).
 *
 * self: the boss.
 * Returns self->flags & 8. With 6 bombs live it waits the last delay read, 0 (bug kept: the
 * delay is unsigned, so the test for -1, run this state again now, is never true). */
#include "xmen2.h"

#define g_ApocBombCount	(*(s16 *)g_SceneVar3B08)	/* scene variable: live bombs */

short Apocalypse_StateDropBombs(register Apocalypse *self)
{
	register unsigned short param;
	register char done = 0;
	register unsigned short delay = 0;

	if (g_ApocBombCount <= 5) {
		param = self->pattern->param;
		delay = self->pattern->delay;
		ApocBomb_Spawn(self->bombSpeed, param);
		self->pattern++;
		if (self->pattern->param == 0xFFFF) {
			done = 1;
			if (++self->patternIndex > 5)
				self->patternIndex = 0;
			Apocalypse_SelectBombPattern(self);
		}
		if (done) {
			SET_STATE(self, (StateFn)Apocalypse_StateBombsDone);
			return self->flags & 8;
		}
	}
	if (delay == -1) {
		GOTO_STATE(self, Apocalypse_StateDropBombs);
	} else {
		SET_STATE_WAIT(self, Apocalypse_StateDropBombs, delay);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
