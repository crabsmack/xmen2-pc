/* Tusk_LockArena - install Tusk_HookIsEvent3 as the hook of floor-segment type 20.
 *
 * self: TUSK (passed by Tusk_StateIntro, unused). */
#include "xmen2.h"

void Tusk_LockArena(Tusk *self)
{
	g_SegTypeHooks[20] = Tusk_HookIsEvent3;
}
