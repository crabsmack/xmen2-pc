/* Blast_StateStick - BLAST state: stuck in a wall: stop, the impact animation (Tbl_BlastImpactAnim),
 * out of the enemy attacker hit list (List_Unlink), flag bit 2 of its switch set (class slot
 * +$6E, Blast_SetFlags) for $3C frames, then Blast_StateUnstick.
 *
 * self: the blast. Returns self->flags & 8. */
#include "xmen2.h"

short Blast_StateStick(register Blast *self)
{
	STOP_MOVING(self);
	Obj_ChangeAnim(self, CODEPTR(Tbl_BlastImpactAnim));
	List_Unlink(self->attackNode, &g_HitListEnemyAtk);
	self->attackNode = 0;
	(*self->cls->method[26])(self, 2);		/* +$6E SetFlags */
	SET_STATE_WAIT(self, Blast_StateUnstick, 0x3C);
	return self->flags & 8;
}
