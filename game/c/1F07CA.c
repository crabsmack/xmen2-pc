/* XPlayer_CanRejoinAt - can this player rejoin at its partner's safe spot, given how the partner
 * held on there?
 *
 * self:    the player rejoining.
 * partner: the other player.
 * The partner's climb state at the safe spot (a climber's climbFlags bits 4-5): none: yes; on a
 * wall (2, 3): if this player is a climber too (Player_IsClimber); on a ceiling (1): if it is
 * Nightcrawler or Wolverine (Player_IsNcOrWolverine2). Returns 1 / 0. */
#include "xmen2.h"

char XPlayer_CanRejoinAt(XPlayer *self, register XPlayer *partner)
{
	unsigned short held;

	held = Player_IsClimber(partner) ? (((XClimber *)partner)->climbFlags & 0x30) >> 4 : 0;
	if (!held)
		return 1;
	if (held == 2 || held == 3)
		return Player_IsClimber(self);
	return Player_IsNcOrWolverine2(self);
}
