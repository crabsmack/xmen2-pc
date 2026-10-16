/* Nightcrawler_StateTeleportFlash - NIGHTCRAWLER the strong reappearance
 * (Nightcrawler_TeleportAppear): the palette flash, one step a frame, then
 * Nightcrawler_TeleportDone.
 *
 * self: the player (py counts the flash steps).
 * Unless a palette fade runs (then the flash counts as over): Nightcrawler_FlashPalette(py), py
 * +1 (+2 in two-player mode). Over: the base palette back (g_PalBaseDirty), then
 * Nightcrawler_TeleportDone when the animation ends (on the next update with none).
 * Returns self->flags & 8 then; nothing while it flashes. */
#include "xmen2.h"

short Nightcrawler_StateTeleportFlash(register Nightcrawler *self)
{
	register char done;

	if (g_PalFadeActive)
		done = 1;
	else {
		done = Nightcrawler_FlashPalette(self->py);
		self->py += g_TwoPlayerStep + 1;
	}
	if (done) {
		g_PalBaseDirty = 1;
		if (self->anim) {
			SET_STATE_WAIT(self, (StateFn)Nightcrawler_TeleportDone, 0xFF);
			return self->flags & 8;
		}
		SET_STATE(self, (StateFn)Nightcrawler_TeleportDone);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
