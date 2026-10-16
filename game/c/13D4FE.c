/* Ninja_ActAttack - NINJA script action: the attack animation Tbl_NinjaAnimAttack with sound $AC
 * (restarted), then the next script step when it ends (delay $FF).
 *
 * self: the object. */
#include "xmen2.h"

void Ninja_ActAttack(register Ninja *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_NinjaAnimAttack));
	Snd_Stop(0xAC);
	Snd_Play(0xAC);
	AI_NEXT_STEP(self, self->ai, 0xFF);
}
