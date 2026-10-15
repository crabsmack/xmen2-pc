/* Hornet_StatePickPattern - HORNET state of a DEATHBIRD hornet that has dropped in: set up the
 * flight pattern of its kind, then start the attack cycle (Hornet_StateFacePlayer after
 * attackDelay frames, at once when it is -1).
 *
 * self: the hornet. Returns self->flags & 8, or what Hornet_StateFacePlayer returns.
 * Kinds: 0 (and anything else) bounce left and right (vx $200), 1 bounce up and down (vy $180),
 * 2 dive once and leave, 3 dive again and again (vy $300 both). The cases are
 * Hornet_StatePickPattern_CaseDefault, _Case1 (1-3) and _StartAttack (the tail).
 * (xmen2disasm config/compiler/match/136D86.c is the same function against its own struct.) */
#include "xmen2.h"

short Hornet_StatePickPattern(Hornet *self)
{
	switch (self->kind) {
	default:
	case 0:				/* Hornet_StatePickPattern_CaseDefault */
		self->vx = 0x200;
		self->moveFlags |= 1;
		self->move = (HornetMoveFn)Hornet_MoveBounce;
		break;
	case 1:				/* Hornet_StatePickPattern_Case1 */
		self->vy = 0x180;
		self->moveFlags |= 2;
		self->move = (HornetMoveFn)Hornet_MoveBounce;
		break;
	case 2:
		self->vy = 0x300;
		self->moveFlags |= 2;
		self->move = (HornetMoveFn)Hornet_MoveDiveOnce;
		break;
	case 3:
		self->vy = 0x300;
		self->moveFlags |= 2;
		self->move = (HornetMoveFn)Hornet_MoveDiveRepeat;
		break;
	}
	/* Hornet_StatePickPattern_StartAttack */
	if (self->attackDelay == -1) {
		self->state = (StateFn)Hornet_StateFacePlayer;
		return Hornet_StateFacePlayer(self);
	}
	self->timer = self->attackDelay;
	self->state = (StateFn)Hornet_StateFacePlayer;
	return self->flags & 8;
}
