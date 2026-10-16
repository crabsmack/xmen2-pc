/* PlayerSelect_RevertCursor - a slot takes the character it asked for / goes back to (savedChar),
 * the change is done, and its cursor moves there (PlayerSelect_PlaceCursor).
 *
 * slot: the player's select slot. */
#include "xmen2.h"

void PlayerSelect_RevertCursor(register BigSprite *slot)
{
	slot->charIndex = slot->savedChar;
	slot->changing = 0;
	PlayerSelect_PlaceCursor(slot);
}
