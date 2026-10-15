/* XPlayer_OutsideActiveWindow - XPLAYER class slot +$42: is it outside the active window? Never:
 * Actor_Update never deactivates a player.
 *
 * self: the player (unused). Returns 0. */
#include "xmen2.h"

char XPlayer_OutsideActiveWindow(void)
{
	return 0;
}
