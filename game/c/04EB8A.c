/* BeamScooter_StatePickUp - BEAMSCOOTER: wait until every living player (present, alive, health
 * above 0) rides an object (spr.parent, the object it is attached to, not $FF: standing on the
 * scooter), then freeze each of them standing (XPLAYER slot +$7A FreezeStanding(1)), fly off
 * up-left (acceleration -$10, -$20) and end the scene in 180 frames.
 *
 * self: the scooter. While waiting it returns self->flags & 8; once it leaves it returns no value
 * and waits $FF frames (Actor_Update), the state unchanged. */
#include "xmen2.h"

short BeamScooter_StatePickUp(register BeamScooter *self)
{
	register char p1, p2;

	p1 = 0;
	p2 = 0;
	if (IS_ALIVE(&g_Player1) && g_Player1.health > 0)
		p1 = 1;
	if (IS_ALIVE(&g_Player2) && g_Player2.health > 0)
		p2 = 1;
	if ((!p1 || g_Player1.spr.parent != 0xFF) && (!p2 || g_Player2.spr.parent != 0xFF)) {
		if (p1)
			(*g_Player1.cls->method[29])(&g_Player1, 1);	/* +$7A FreezeStanding */
		if (p2)
			(*g_Player2.cls->method[29])(&g_Player2, 1);
		self->ay = -0x20;
		self->ax = -0x10;
		Scene_EndAfter(0xB4);
		self->timer = 0xFF;
	} else {
		SET_STATE(self, BeamScooter_StatePickUp);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
