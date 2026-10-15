/* XPlayer_CheckGrabNo - the base "grab a wall or ceiling here?" test (class slot +$96): never.
 *
 * self: the player (unused).
 * Returns 0. XCLIMBER overrides it with XClimber_CheckGrab. */
#include "xmen2.h"

char XPlayer_CheckGrabNo(void)
{
	return 0;
}
