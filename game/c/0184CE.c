/* ShootAcolyte_StateAim - SHOOTACOLYTE state: play the aiming animation Tbl_ShootAcolyteAnim2,
 * then fire (ShootAcolyte_StateFire when it ends: delay $FF).
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short ShootAcolyte_StateAim(register ShootAcolyte *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_ShootAcolyteAnim2));
	SET_STATE_WAIT(self, ShootAcolyte_StateFire, 0xFF);
	return self->flags & 8;
}
