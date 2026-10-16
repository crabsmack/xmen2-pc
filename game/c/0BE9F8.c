/* Launcher_ShotHitsPlayer - whether a launcher shot at (x, y) hits a player, who then takes the
 * hit: TakeDamage (class slot +$26) from no attacker, of the given type, 1 damage, at (x, y).
 *
 * p: the player.  x, y: the shot (pixels).  type: the hit type (Launcher_Draw: 9 for a shot
 * flying right, else 10).
 * A hit: the player is alive, not in hit cooldown, and (x, y) is inside its second box
 * (inclusive). Returns 1 on a hit, else 0. Never runs in the corpus. */
#include "xmen2.h"

char Launcher_ShotHitsPlayer(register XPlayer *p, register short x, register short y, long type)
{
	if (IS_ALIVE(p) && !p->hitCooldown && x >= p->box2Left && x <= p->box2Right
	    && y >= p->box2Top && y <= p->box2Bottom) {
		(*p->cls->method[8])(p, 0, type, 1, x, y);	/* +$26 TakeDamage */
		return 1;
	}
	return 0;
}
