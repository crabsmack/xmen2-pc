/* XPlayer_StartLookUp - start looking up (XPlayer_StateLookUp): stop, mark it (playerFlags
 * $40000) and pan the camera up.
 *
 * self: the player. XPlayer_StateStand starts it after Up is held a while.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short XPlayer_StartLookUp(XPlayer *self)
{
	self->stateTimer = 0;
	STOP_MOVING(self);
	self->playerFlags |= 0x40000;
	(*g_Camera->cls->method[8])(g_Camera);		/* LookUp */
	SET_STATE(self, (StateFn)XPlayer_StateLookUp);
	return self->flags & 8;
}
