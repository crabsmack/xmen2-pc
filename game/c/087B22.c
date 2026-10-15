/* BroodBoss_StateHurtEnd - BROODBOSS state after the hurt flash: palette line 2 back from
 * g_BroodBossPalSave (if it is still the flash: colour 2 = $424); at 0 health
 * BroodBoss_StateDeathStart when the animation ends, else the AI-script step it was on, again,
 * in 10 frames.
 *
 * self: the boss. Returns flags & 8 when it dies, nothing otherwise.
 * (The health test has an empty then-branch, as the ROM shows: MPW C reloads self from the stack
 * at the start of the else.) */
#include "xmen2.h"

short BroodBoss_StateHurtEnd(BroodBoss *self)
{
	register s16 *save;
	register s16 *pal;
	register char i;

	if (g_PalBase[34] == 0x424) {
		save = g_BroodBossPalSave;
		pal = &g_PalBase[32];
		for (i = 0; i < 16; i++)
			*pal++ = *save++;
		g_PalBaseDirty = 1;
	}
	if (self->health > 0)
		;
	else {
		SET_STATE_WAIT(self, BroodBoss_StateDeathStart, 0xFF);
		return self->flags & 8;
	}
	self->state = FN_AT(StateFn, AI_SCRIPT(self->ai)->step + self->ai.index);
	self->timer = 10;
	NO_RETURN_VALUE
}
