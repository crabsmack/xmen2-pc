/* XPlayer_FreezeStanding - XPLAYER class slot +$7A: freeze the player in its standing pose, not
 * moving and not hurt (scripted scenes: BeamScooter_StatePickUp, on both players).
 *
 * self: the player.
 * flags6 $4000 stops XPlayer_Update moving it, the standing animation, the frozen state
 * (XPlayer_StateFrozen), and a hit cooldown of 20000 frames. */
#include "xmen2.h"

void XPlayer_FreezeStanding(XPlayer *self)
{
	self->flags6 |= 0x4000;
	Obj_SetAnim(self, CHAR_PART(self->charRec, walkAnim));
	self->state = (StateFn)XPlayer_StateFrozen;
	self->timer = 0;
	self->hitCooldown = 20000;
}
