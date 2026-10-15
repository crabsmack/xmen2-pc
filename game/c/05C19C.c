/* Tusk_HookReturn0 - floor-segment type 20 hook once TUSK's fight starts (Tusk_UnlockArena):
 * always 0 (message 3 has a branch of its own that also returns 0).
 *
 * msg: the hook's message. */
#include "xmen2.h"

short Tusk_HookReturn0(short msg)
{
	if (msg == 3)
		return 0;
	else
		return 0;
}
