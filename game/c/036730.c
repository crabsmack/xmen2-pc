/* MagnetoBoss_TakeDamage - MAGNETOBOSS TakeDamage (class slot +$26; the parent's isn't called):
 * hits can't defeat it (THRONEPHONE catches it instead).
 *
 * self:     the boss.
 * attacker: what hit it (unused).  type: hit type.  damage: health to lose.
 * A 30-frame hit cooldown, health down by damage but not below 10; in moveMode 4 (the final
 * fight) the point it drifts to moves 16 px per damage point: left for hit type 9, right for
 * type 10. Sound $7D and the hurt animation (Tbl_MagnetoBossAnim). */
#include "xmen2.h"

void MagnetoBoss_TakeDamage(register MagnetoBoss *self, Actor *attacker, char type,
	register char damage)
{
	register short push;

	self->hitCooldown = 30;
	self->health -= damage;
	if (self->health < 10)
		self->health = 10;
	if (self->moveMode == 4) {
		push = damage << 4;
		switch (type) {
		case 9:
			push = -push;
		case 10:
			self->destX += push;
		}
	}
	Snd_Stop(0x7D);
	Snd_Play(0x7D);
	Obj_ChangeAnim(self, CODEPTR(Tbl_MagnetoBossAnim));
}
