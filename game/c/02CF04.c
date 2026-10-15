/* Exodus_StateNextAttack - EXODUS state: the next step of its attack pattern, each an attack
 * state run from the next update; past the pattern's last step, the next pattern
 * (Exodus_NextPattern).
 *
 * self: the boss.
 * Patterns (self->pattern), one attack per step:
 *   0: blasts left+right (Exodus_AtkBlast17), down-left (2), down-right (3), down (4), swoop to
 *      segment A, swoop to segment B
 *   1: left+right   2: down, down-left   3: down, down-right   4: down-left, down-right
 *   5: down, left+right
 * Returns self->flags & 8 after an attack is chosen, no value otherwise.
 * The disassembly splits it into Exodus_StateNextAttack_part1 (pattern 0), _part2 (its cases),
 * _part3 (its default), _part4 (patterns 1, 2, 3), _part5 (4, 5) and the tail _part6. */
#include "xmen2.h"

short Exodus_StateNextAttack(register Exodus *self)
{
	switch (self->pattern) {
	case 0:		/* Exodus_StateNextAttack_part1 */
		switch (++self->step) {
		case 1:		/* Exodus_StateNextAttack_part2 */
			SET_STATE(self, Exodus_AtkBlast17);
			return self->flags & 8;
		case 2:
			SET_STATE(self, Exodus_AtkBlast2);
			return self->flags & 8;
		case 3:
			SET_STATE(self, Exodus_AtkBlast3);
			return self->flags & 8;
		case 4:
			SET_STATE(self, Exodus_AtkBlast4);
			return self->flags & 8;
		case 5:
			SET_STATE(self, Exodus_AtkSwoopA);
			return self->flags & 8;
		case 6:
			SET_STATE(self, Exodus_AtkSwoopB);
			return self->flags & 8;
		default:	/* Exodus_StateNextAttack_part3 */
			Exodus_NextPattern(self);
			break;
		}
		break;
	case 1:		/* Exodus_StateNextAttack_part4 */
		switch (++self->step) {
		default:
			break;
		case 1:
			SET_STATE(self, Exodus_AtkBlast17);
			return self->flags & 8;
		}
		Exodus_NextPattern(self);
		break;
	case 2:
		switch (++self->step) {
		case 1:
			SET_STATE(self, Exodus_AtkBlast4);
			return self->flags & 8;
		case 2:
			SET_STATE(self, Exodus_AtkBlast2);
			return self->flags & 8;
		default:
			Exodus_NextPattern(self);
			break;
		}
		break;
	case 3:
		switch (++self->step) {
		case 1:
			SET_STATE(self, Exodus_AtkBlast4);
			return self->flags & 8;
		case 2:
			SET_STATE(self, Exodus_AtkBlast3);
			return self->flags & 8;
		default:
			Exodus_NextPattern(self);
			break;
		}
		break;
	case 4:		/* Exodus_StateNextAttack_part5 */
		switch (++self->step) {
		case 1:
			SET_STATE(self, Exodus_AtkBlast2);
			return self->flags & 8;
		case 2:
			SET_STATE(self, Exodus_AtkBlast3);
			return self->flags & 8;
		default:
			Exodus_NextPattern(self);
			break;
		}
		break;
	case 5:
		switch (++self->step) {
		case 1:
			SET_STATE(self, Exodus_AtkBlast4);
			return self->flags & 8;
		case 2:
			SET_STATE(self, Exodus_AtkBlast17);
			return self->flags & 8;
		default:
			Exodus_NextPattern(self);
			break;
		}
		break;
	}
	NO_RETURN_VALUE		/* Exodus_StateNextAttack_part6 */
}
