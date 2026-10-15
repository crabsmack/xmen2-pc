/* Deathbird_StateChooseAttack - DEATHBIRD waiting off screen: when countdown has run out, go for
 * the lift switch (SetMoveMode 9, Deathbird_StateGoToSwitch) if the lift is going up, in the 32
 * frames of every 64 with g_FrameCounter bit 5; else take the next attack of the attack list
 * (Tbl_DeathbirdAttackOrder, from the start after its -1 end): 1 swoop (SetMoveMode 5,
 * Deathbird_StateSwoopDown), 2 low dive (7, Deathbird_StateDiveLow), anything else fly by (4,
 * Deathbird_StateFlyOff).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

#define g_DeathbirdAttack	(*(PTR(s8) *)g_SceneVar3B14)	/* scene variable: next entry of the attack list */

short Deathbird_StateChooseAttack(Deathbird *self)
{
	char attack;

	if (self->countdown <= 0) {
		if (g_Elevator->up && g_FrameCounter & 0x20) {
			(*self->cls->method[27])(self, 9);	/* SetMoveMode */
			SET_STATE(self, (StateFn)Deathbird_StateGoToSwitch);
			return self->flags & 8;
		}
		if (*g_DeathbirdAttack == -1)
			g_DeathbirdAttack = (s8 *)CODEPTR(Tbl_DeathbirdAttackOrder);
		attack = *g_DeathbirdAttack;
		g_DeathbirdAttack++;
		switch (attack) {
		case 1:
			(*self->cls->method[27])(self, 5);	/* SetMoveMode */
			SET_STATE(self, (StateFn)Deathbird_StateSwoopDown);
			return self->flags & 8;
		case 2:
			(*self->cls->method[27])(self, 7);	/* SetMoveMode */
			SET_STATE(self, (StateFn)Deathbird_StateDiveLow);
			return self->flags & 8;
		default:
			(*self->cls->method[27])(self, 4);	/* SetMoveMode */
			SET_STATE(self, (StateFn)Deathbird_StateFlyOff);
			return self->flags & 8;
		}
	}
	SET_STATE(self, (StateFn)Deathbird_StateChooseAttack);
	return self->flags & 8;
}
