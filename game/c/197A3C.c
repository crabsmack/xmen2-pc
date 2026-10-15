/* Wolverine_Update - WOLVERINE Update (class slot +$4A): XCLIMBER's (XPlayer_Update), then the
 * healing factor: every 900 frames, a Wolverine below 3 health units (but alive) is healed to
 * one whole unit above the unit he is in.
 *
 * self: the player.
 * healTimer counts down by 1, or 2 in two-player mode (g_TwoPlayerStep: the game runs at half
 * rate). When it has run out it restarts at 900; a heal plays sound $39, sets health to
 * (((health - 1) >> 8) + 2) << 8 and redraws the HUD (g_HudDirty). */
#include "xmen2.h"

void Wolverine_Update(register Wolverine *self)
{
	register short units;

	(*CLASS(Tbl_Wolverine_vtable)->parent->method[17])(self);	/* XCLIMBER Update */
	if (self->healTimer <= 0) {
		self->healTimer = 900;
		if (self->health < 0x300 && self->health > 0) {
			units = ((self->health - 1) >> 8) + 1;
			Snd_Stop(0x39);
			Snd_Play(0x39);
			units++;
			self->health = units << 8;
			g_HudDirty = 1;
		}
	} else
		self->healTimer -= g_TwoPlayerStep ? 2 : 1;
}
