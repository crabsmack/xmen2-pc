/* Beast_StateStomp - the stomp hits the floor (after Beast_GroundSpecial's wind-up): start the two
 * shockwaves at his feet (Beast_ShockwaveInit: strong above 7 health units), 1000 frames of
 * invulnerability (hitCooldown) and the impact animation, strong (Tbl_Anim_1829DA) or weak
 * (Tbl_EvilBeastAnim); Beast_StateStompEnd at its end.
 *
 * self: the player.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Beast_StateStomp(register Beast *self)
{
	self->waveY = self->y >> 8;
	Beast_ShockwaveInit(self->x >> 8, &self->waveLeftX, &self->waveRightX, &self->waveSpr,
		self->health >= 0x701);
	self->hitCooldown = 1000;
	if (self->health >= 0x701)
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1829DA));
	else
		Obj_SetAnim(self, CODEPTR(Tbl_EvilBeastAnim));
	SET_STATE_WAIT(self, (StateFn)Beast_StateStompEnd, 0xFF);
	return self->flags & 8;
}
