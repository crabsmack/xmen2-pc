/* Nightcrawler_StartTeleport - NIGHTCRAWLER the special (A; Nightcrawler_GroundSpecial and
 * _AirSpecial): vanish and teleport in the direction held, as far as the meter says; once per
 * teleport.
 *
 * self: the player.
 * Already teleporting (playerFlags $800000): back to the control state (slot +$A6) from the next
 * update. Else: px = 1 (teleporting), playerFlags $800000, 60 frames of hit cooldown, stopped,
 * off the floor (Obj_Detach; playerFlags 8 and $40), no attack, sound $85, the vanishing
 * animation (Tbl_EvilNightcrawlerAnim). On the ground with Down held over a floor of a layer
 * above 0 he goes down through it: that costs 8 of the meter (and bits 2-3 of pad are cleared).
 * teleportTime = the meter, at least $E; teleportDir = the Up/Down/Left/Right bits held. With a
 * direction (playerFlags $200000): Nightcrawler_TeleportDown (down through the floor) or
 * Nightcrawler_TeleportVanish when the animation ends; with none, Nightcrawler_TeleportAppear.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Nightcrawler_StartTeleport(register Nightcrawler *self)
{
	register char down;

#ifdef XMEN2_MPW
	down = !(self->flags & 0x1000)
	    && (!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown)
	    && (s8)self->seg->layer > 0;
#else
	/* with no segment the 68000 reads ROM $000002 ($C5: negative) */
	down = !(self->flags & 0x1000)
	    && (!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown)
	    && (self->seg ? (s8)self->seg->layer : (s8)-59) > 0;
#endif
	if (self->playerFlags & 0x800000) {
		SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		return self->flags & 8;
	}
	self->px = 1;
	self->playerFlags |= 0x800000;
	self->hitCooldown = 60;
	STOP_MOVING(self);
	Obj_Detach(self);
	self->playerFlags |= 8;
	self->playerFlags |= 0x40;
	self->attack = 0;
	Snd_Stop(0x85);
	Snd_Play(0x85);
	Obj_SetAnim(self, CODEPTR(Tbl_EvilNightcrawlerAnim));
	if (down) {
		self->charge -= 8;
		self->pad &= ~0xC;
	}
	self->teleportTime = MAX(0xE, self->charge);
	self->teleportDir = !self->pad ? g_Pad1Held & 0xF : g_Pad2Held & 0xF;
	if (self->teleportDir) {
		self->playerFlags |= 0x200000;
		if (down) {
			SET_STATE_WAIT(self, (StateFn)Nightcrawler_TeleportDown, 0xFF);
			return self->flags & 8;
		}
		SET_STATE_WAIT(self, Nightcrawler_TeleportVanish, 0xFF);
		return self->flags & 8;
	}
	SET_STATE_WAIT(self, Nightcrawler_TeleportAppear, 0xFF);
	return self->flags & 8;
}
