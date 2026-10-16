/* ThronePhone_StateCatch - THRONEPHONE state: catch the boss: one more catch (mode), the shape of
 * mode & 1 (its SetMode, class slot +$6E), the boss starts spinning (MagnetoBoss_EnterSpinState),
 * and from the next update it is pulled in (ThronePhone_StatePullIn).
 *
 * self: the beam. Returns self->flags & 8; no value without a boss. */
#include "xmen2.h"

short ThronePhone_StateCatch(register ThronePhone *self)
{
	register MagnetoBoss *boss = self->boss;

	if (boss) {
		self->mode++;
		(*self->cls->method[26])(self, (unsigned char)(self->mode & 1));	/* +$6E ThronePhone_SetMode */
		MagnetoBoss_EnterSpinState(boss);
		SET_STATE(self, ThronePhone_StatePullIn);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
