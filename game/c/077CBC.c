/* LightBeam_StateIdle - LIGHTBEAM's only state (also its default and hurt state): run again next
 * frame, doing nothing (a beam has no update anyway: LightBeam_Activate).
 *
 * self: the beam. Returns flags & 8. */
#include "xmen2.h"

short LightBeam_StateIdle(LightBeam *self)
{
	SET_STATE(self, LightBeam_StateIdle);
	return self->flags & 8;
}
