/* Tusk_UnlockArena - install Tusk_HookReturn0 as the hook of floor-segment type 20 and put
 * both of TUSK's floor segments (its ledge and the arena floor) on layer 0.
 *
 * self: TUSK. */
#include "xmen2.h"

void Tusk_UnlockArena(Tusk *self)
{
	g_SegTypeHooks[20] = Tusk_HookReturn0;
	self->ledge->layer = 0;
	self->arenaFloor->layer = 0;
}
