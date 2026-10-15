/* Nightcrawler_Attack - NIGHTCRAWLER Attack (class slot +$22): the parent's (XCLIMBER, i.e.
 * XPlayer_Attack); a hit with the dive kick bounces him back up.
 *
 * self: the player.  target, type, damage, sx, sy: as XPlayer_Attack (passed on; damage is a
 *       char here).
 * In the air and diving (playerFlags 1): the dive sound $84 stops, $87 plays, vy -$700, the
 * record's gravity and fall speed, no vx, the control state (slot +$A6) in 2 frames, playerFlags
 * 2 and 1 off (the dive can be used again), B unlatched. */
#include "xmen2.h"

void Nightcrawler_Attack(register Nightcrawler *self, Actor *target, char type, char damage,
	short sx, short sy)
{
	(*CLASS(Tbl_Nightcrawler_vtable)->parent->method[7])(self, target, type, damage, sx,
		sy);						/* XCLIMBER Attack */
	if (self->flags & 0x1000 && self->playerFlags & 1) {
		Snd_Stop(0x84);
		Snd_Stop(0x87);
		Snd_Play(0x87);
		self->vy = -0x700;
		self->ay = self->charRec->gravity;
		self->timer = 2;
		self->maxVy = self->charRec->maxFallVy;
		self->state = (StateFn)self->cls->method[40];		/* StateControl */
		self->vx = 0;
		self->playerFlags &= ~2;
		self->playerFlags &= ~1;
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
	}
}
