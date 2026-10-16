/* Tank_StateWreck - TANK state after its death (Tank_OnDeath): the wreck animation
 * Tbl_TankWreckAnim, run again whenever the animation ends (delay $FF), for good.
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short Tank_StateWreck(register Tank *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_TankWreckAnim));
	SET_STATE_WAIT(self, Tank_StateWreck, 0xFF);
	return self->flags & 8;
}
