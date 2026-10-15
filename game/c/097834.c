/* XClone_ActAttack - XCLONE script action: attack with the character's combo: the first combo
 * animation when the attack cooldown is out (which reloads it from the character's combo window),
 * else the second; to its end ($FF). A character without combo animations goes on now.
 *
 * self: the clone (a step of its AI script). */
#include "xmen2.h"

void XClone_ActAttack(XClone *self)
{
	if (self->charRec->comboAnim1) {
		if (self->attackCooldown > 0) {
			Obj_SetAnim(self, CLONE_PART(self->charRec, comboAnim2));
		} else {
			self->attackCooldown = self->charRec->comboWindow;
			Obj_SetAnim(self, CLONE_PART(self->charRec, comboAnim1));
		}
		AI_NEXT_STEP(self, self->ai, 0xFF);
	} else {
		AI_NEXT_STEP(self, self->ai, 0);
	}
}
