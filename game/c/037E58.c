/* ThronePhone_StateInit - THRONEPHONE's first state: remember the Magneto boss (g_BgLockObj,
 * MagnetoBoss_Init stores itself there), then wait for him (ThronePhone_StateWaitMagneto).
 *
 * self: the beam. Returns self->flags & 8. */
#include "xmen2.h"

short ThronePhone_StateInit(register ThronePhone *self)
{
	self->boss = (MagnetoBoss *)g_BgLockObj;
	SET_STATE(self, ThronePhone_StateWaitMagneto);
	return self->flags & 8;
}
