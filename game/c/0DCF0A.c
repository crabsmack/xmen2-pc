/* Blast_StateFly - BLAST state: fly on until it reaches a wall or the end of the floor
 * (Seg_AtOpenEndAhead(self, 10)); then stick in it (Blast_StateStick) when it sticks, else
 * vanish (Blast_StateKill), next frame.
 *
 * self: the blast. Returns self->flags & 8 when it stopped; while flying, no value (MPW's d0). */
#include "xmen2.h"

short Blast_StateFly(register Blast *self)
{
	if (Seg_AtOpenEndAhead(self, 10)) {
		if (self->sticks) {
			SET_STATE(self, Blast_StateStick);
			return self->flags & 8;
		}
		SET_STATE(self, (StateFn)Blast_StateKill);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
