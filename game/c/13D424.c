/* Ninja_StateLeapAir - NINJA state: in the air: the airborne animation Tbl_NinjaAnimAirborne and
 * the leap sound $80 (restarted), then wait for the landing (Ninja_StateWaitLanding from the
 * next update).
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short Ninja_StateLeapAir(register Ninja *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_NinjaAnimAirborne));
	Snd_Stop(0x80);
	Snd_Play(0x80);
	SET_STATE(self, Ninja_StateWaitLanding);
	return self->flags & 8;
}
