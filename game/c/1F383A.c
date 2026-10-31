/* XPlayer_StateStand - standing (XPlayer_Stand): an action pressed (XPlayer_ActionPressed) or
 * leaving the ground goes back to the control state (slot +$A6) now, latching the direction
 * still held (Right first); Up held a while starts looking up (XPlayer_StartLookUp); at the
 * animation's end it picks the idle animation (a random draw above the character's
 * idleChance) or the standing one again.
 *
 * self: the player.
 * Looking up: Up held with stateTimer below 3 and above 0, the camera not already looking up
 * and no playerFlags $400000 (Up latched in an attack); otherwise stateTimer restarts at 50.
 * Native, the select_idles option: Gambit's record draws against GAMBIT_IDLE_CHANCE (his own
 * idleChance is above anything Rand_Next returns, so his idle never plays standing in the
 * original; it plays as his look-up, the same script).
 * Returns self->flags & 8, except when it goes back to the control state. */
#include "xmen2.h"

short XPlayer_StateStand(XPlayer *self)
{
	if (XPlayer_ActionPressed(self) || self->flags & 0x1000) {
		self->playerFlags &= ~0x400000;
		self->playerFlags &= ~0x200000;
		if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight) {
			if (!self->pad)
				g_Pad1Latch |= g_PadMaskRight;
			else
				g_Pad2Latch |= g_PadMaskRight;
		} else if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft) {
			if (!self->pad)
				g_Pad1Latch |= g_PadMaskLeft;
			else
				g_Pad2Latch |= g_PadMaskLeft;
		}
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		RETURN_NO_VALUE;
	}
	if ((!self->pad ? g_Pad1Held & g_PadMaskUp : g_Pad2Held & g_PadMaskUp) && self->stateTimer < 3) {
		if (self->stateTimer > 0 && !(g_Camera->camFlags & 4) && !(self->playerFlags & 0x400000)) {
			SET_STATE(self, (StateFn)XPlayer_StartLookUp);
			return self->flags & 8;
		}
		self->stateTimer = 50;
	}
	if (self->anim) {
		SET_STATE(self, (StateFn)XPlayer_StateStand);
		return self->flags & 8;
	}
#if defined(XMEN2_NATIVE)
	/* the select_idles option: Gambit idles too, whoever stands with his record, as the others do
	   (GAMBIT_IDLE_CHANCE; the draw is the same) */
	if (g_opt.select_idles && self->charRec == (CharRecord *)CODEPTR(Tbl_GambitCharRecord)) {
		if ((unsigned short)Rand_Next() > GAMBIT_IDLE_CHANCE)
			Obj_SetAnim(self, CHAR_PART(self->charRec, idleAnim));
		else
			Obj_SetAnim(self, CHAR_PART(self->charRec, walkAnim));
	} else
#endif
	if ((unsigned short)Rand_Next() > self->charRec->idleChance)
		Obj_SetAnim(self, CHAR_PART(self->charRec, idleAnim));
	else
		Obj_SetAnim(self, CHAR_PART(self->charRec, walkAnim));
	SET_STATE(self, (StateFn)XPlayer_StateStand);
	return self->flags & 8;
}
