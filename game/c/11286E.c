/* Deathbird_StateDeadFlicker - DEATHBIRD's last state (after Deathbird_StateFalling): every 2
 * frames, fill palette line 1 with 32 bytes of ROM from the address g_FrameCounter, a flicker of
 * changing colours until the scene ends.
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Deathbird_StateDeadFlicker(Deathbird *self)
{
	Mem_Copy(&g_PalBase[0x20], AT68(void, g_FrameCounter), 0x20);
	g_PalBaseDirty = 1;
	SET_STATE_WAIT(self, (StateFn)Deathbird_StateDeadFlicker, 2);
	return self->flags & 8;
}
