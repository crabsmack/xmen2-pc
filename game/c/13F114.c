/* Pop_StateWait - POP state: wait (every frame) until its countdown has run out, then become the
 * explosion effect fxType picks, from the next update: 5 Pop_FxChainSpiral (case
 * Pop_StateWait_Case5), 4 Pop_FxChainPath, 3 Pop_FxBurstSmall, 2 Pop_FxTrailArc, 1 Pop_FxBounce,
 * 0 and any other Pop_FxBurstMedium (Pop_StateWait_CaseDefault).
 *
 * self: the object.
 * Returns flags bit 3. Never runs in the corpus. */
#include "xmen2.h"

short Pop_StateWait(register Pop *self)
{
	if (self->countdown <= 0) {
		switch (self->fxType) {
		case 5:		/* Pop_StateWait_Case5 */
			SET_STATE(self, (StateFn)Pop_FxChainSpiral);
			return self->flags & 8;
		case 4:
			SET_STATE(self, (StateFn)Pop_FxChainPath);
			return self->flags & 8;
		case 3:
			SET_STATE(self, (StateFn)Pop_FxBurstSmall);
			return self->flags & 8;
		case 2:
			SET_STATE(self, (StateFn)Pop_FxTrailArc);
			return self->flags & 8;
		case 1:
			SET_STATE(self, (StateFn)Pop_FxBounce);
			return self->flags & 8;
		case 0:
		default:	/* Pop_StateWait_CaseDefault */
			SET_STATE(self, (StateFn)Pop_FxBurstMedium);
			return self->flags & 8;
		}
	} else {
		SET_STATE(self, Pop_StateWait);
		return self->flags & 8;
	}
}
