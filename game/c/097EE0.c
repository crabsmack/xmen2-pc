/* EvilBeast_ActThrow - EVILBEAST AI-script action: Beast's stomp, the weak kind. Start the two
 * shockwaves at its feet (Beast_ShockwaveInit), stop, play the impact animation
 * (Tbl_EvilBeastAnim) and release them in EvilBeast_StateThrow at its end.
 *
 * self: the clone.
 * Returns self->flags & 8, as every state. The clone's y in pixels is computed and not used. */
#include "xmen2.h"

short EvilBeast_ActThrow(EvilBeast *self)
{
	register short x = self->x >> 8;
	register short y = self->y >> 8;

	Beast_ShockwaveInit(x, &self->waveLeftX, &self->waveRightX,
		&g_SpriteDefs[g_EvilBeastThrowSprDef], 0);
	if (1) {	/* the ROM pops the call's arguments here */
		STOP_MOVING(self);
	}
	Obj_SetAnim(self, CODEPTR(Tbl_EvilBeastAnim));
	SET_STATE_WAIT(self, (StateFn)EvilBeast_StateThrow, 0xFF);
	return self->flags & 8;
}
