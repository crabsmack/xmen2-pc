/* Gambit_AirKick - GAMBIT state: start the air kick: his record's air attack animation and attack
 * sound, B unlatched, a dash at $800 towards a held Left or Right (no acceleration, maximum
 * speed $800), 12 frames (unk_A8) of Gambit_StateAirKick from the next update. Landed: the
 * control state (slot +$A6), now.
 *
 * self: the player. Returns self->flags & 8 after starting the kick, no value otherwise. */
#include "xmen2.h"

short Gambit_AirKick(register Gambit *self)
{
	if (!(self->flags & 0x1000)) {
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		RETURN_NO_VALUE;
	}
	Obj_SetAnim(self, CHAR_PART(self->charRec, airAttackAnim));
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
	Snd_Stop(self->charRec->attackSound);
	Snd_Play(self->charRec->attackSound);
	if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft) {
		self->spr.flags |= 0x10;
		self->maxVx = 0x800;
		self->vx = -0x800;
		self->ax = 0;
	} else if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight) {
		self->spr.flags &= ~0x10;
		self->maxVx = 0x800;
		self->vx = 0x800;
		self->ax = 0;
	}
	self->unk_A8 = 12;
	SET_STATE(self, (StateFn)Gambit_StateAirKick);
	return self->flags & 8;
}
