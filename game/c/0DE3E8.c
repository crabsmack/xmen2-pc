/* ControlPanel_StateHit - CONTROLPANEL hurt state: count the hit (stage + 1): the second and
 * third damage it (sound $CB; ControlPanel_StateDamaged1 / _StateDamaged2), any later one
 * destroys it (ControlPanel_StateDestroyed), from the next update.
 *
 * self: the panel.
 * Hit-flash 10. The stage is compared with 5 (firing) first and the result unused. Returns
 * self->flags & 8. */
#include "xmen2.h"

short ControlPanel_StateHit(register ControlPanel *self)
{
	self->hitCooldown = 10;
	if (self->stage == 5) {
	}
	self->stage++;
	switch (self->stage) {
	case 2:
		Snd_Stop(0xCB);
		Snd_Play(0xCB);
		SET_STATE(self, (StateFn)ControlPanel_StateDamaged1);
		return self->flags & 8;
	case 3:
		Snd_Stop(0xCB);
		Snd_Play(0xCB);
		SET_STATE(self, (StateFn)ControlPanel_StateDamaged2);
		return self->flags & 8;
	case 4:
		SET_STATE(self, ControlPanel_StateDestroyed);
		return self->flags & 8;
	default:
		SET_STATE(self, ControlPanel_StateDestroyed);
		return self->flags & 8;
	}
}
