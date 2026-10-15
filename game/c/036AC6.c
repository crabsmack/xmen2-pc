/* MagnetoBoss_EnterSpinState - THRONEPHONE has caught MAGNETOBOSS: spin (MagnetoBoss_StateSpin)
 * and stop being hittable.
 *
 * self: the boss.
 * flags6 |= $4002, out of the enemy body hit list, flags6 bit $40 (hit blink) off, the spin
 * animation (Tbl_MagnetoBossAnim2_2). Called by ThronePhone_StateCatch. */
#include "xmen2.h"

void MagnetoBoss_EnterSpinState(register MagnetoBoss *self)
{
	self->state = (StateFn)MagnetoBoss_StateSpin;
	self->timer = 0;
	self->flags6 |= 0x4002;
	List_Unlink(self->bodyNode, &g_HitListEnemyBody);
	self->bodyNode = 0;
	self->flags6 &= ~0x40;
	Obj_ChangeAnim(self, CODEPTR(Tbl_MagnetoBossAnim2_2));
}
