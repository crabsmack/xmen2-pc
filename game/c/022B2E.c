/* AvalonA2_HookEndOnEvent3 - floor-segment type $15 hook of the Falling Room: message 3 (a player
 * reached the room's exit) ends the scene after 1 frame (Scene_EndAfter).
 *
 * msg: the hook's message.
 * Returns 1 for message 3, else 0. */
#include "xmen2.h"

short AvalonA2_HookEndOnEvent3(short msg)
{
	if (msg == 3) {
		Scene_EndAfter(1);
		return 1;
	} else
		return 0;
}
