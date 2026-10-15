/* Nightcrawler_TeleportAppear - NIGHTCRAWLER reappears at the end of a teleport, as an attack:
 * above 7 health units the strong one with a palette flash.
 *
 * self: the player.
 * 60 frames of hit cooldown. Health over $700: attack strength[3], sound $CA, the flash
 * animation (Tbl_EvilNightcrawlerAnim2), py 0 (the flash step), Nightcrawler_StateTeleportFlash
 * in 2 frames. Else strength[1], sound $86, Tbl_Anim_1A6086, Nightcrawler_TeleportDone when the
 * animation ends.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Nightcrawler_TeleportAppear(register Nightcrawler *self)
{
	self->hitCooldown = 60;
	if (self->health >= 0x701) {
		self->attack = self->charRec->strength[3];
		Snd_Stop(0xCA);
		Snd_Play(0xCA);
		Obj_SetAnim(self, CODEPTR(Tbl_EvilNightcrawlerAnim2));
		self->py = 0;
		SET_STATE_WAIT(self, Nightcrawler_StateTeleportFlash, 2);
		return self->flags & 8;
	}
	self->attack = self->charRec->strength[1];
	Snd_Stop(0x86);
	Snd_Play(0x86);
	Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A6086));
	SET_STATE_WAIT(self, (StateFn)Nightcrawler_TeleportDone, 0xFF);
	return self->flags & 8;
}
