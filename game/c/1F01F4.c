/* XPlayer_CanDoubleJumpNo - the base "can it double jump now?" test (class slot +$92): never.
 *
 * self: the player (unused).
 * Returns 0. Nightcrawler, Psylocke and Wolverine use XPlayer_CanDoubleJump instead. */
#include "xmen2.h"

char XPlayer_CanDoubleJumpNo(void)
{
	return 0;
}
