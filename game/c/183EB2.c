/* Beast_StateUpAttack - Beast's upward swipe (Beast_GroundAttackUp): a partner standing on it
 * (his body box holding the point 100 px above Beast) is thrown up: TakeDamage with hit type 12
 * and no damage.
 *
 * self: the player.
 * At the animation's end XPlayer_AttackRecover from the next frame; once countdown runs out,
 * at the end of the animation (delay $FF). The partner must be present and alive; his
 * TakeDamage gets Beast as attacker and the point (x, y - 100) as the hit position. */
#include "xmen2.h"

void Beast_StateUpAttack(register Beast *self)
{
	register XPlayer *partner = (XPlayer *)self == &g_Player1 ? &g_Player2 : &g_Player1;
	register short x = self->x >> 8;
	register short y = (self->y >> 8) - 100;

	if (!self->anim) {
		self->state = (StateFn)XPlayer_AttackRecover;
		self->timer = 0;
		return;
	}
	if (self->countdown <= 0) {
		self->state = (StateFn)XPlayer_AttackRecover;
		self->timer = 0xFF;
		return;
	}
	if (IS_ALIVE(partner) && x >= partner->boxLeft && x <= partner->boxRight
	    && y >= partner->boxTop && y <= partner->boxBottom)
		(*partner->cls->method[8])(partner, self, 12, 0, x, y);	/* TakeDamage */
}
