/* SavageBio_StateDeathWait - SAVAGEBIGBIO death: wait until the BIOBALLS companion's death
 * script is done, then blow it up ball by ball for $F0 frames (SavageBio_StateDeathExplode).
 *
 * self: the boss (after SavageBio_ActDie).
 * Returns self->flags & 8. */
#include "xmen2.h"

short SavageBio_StateDeathWait(SavageBigBio *self)
{
	BioBalls *balls = self->balls;

	if (BioBalls_IsDone(balls)) {
		self->countdown = 0xF0;
		SET_STATE(self, SavageBio_StateDeathExplode);
		return self->flags & 8;
	}
	SET_STATE(self, SavageBio_StateDeathWait);
	return self->flags & 8;
}
