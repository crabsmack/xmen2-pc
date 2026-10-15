/* Gambit_AirThrowAim - aim the air throw by the d-pad and throw (Gambit_StateThrow, now): Up 5,
 * Up+Left/Right 6, nothing 7, Down+Left/Right 8, Down 9, each with its animation
 * (Tbl_Anim_1CEDCA .. Tbl_Anim_1CEE0A).
 *
 * self: the player (Gambit_StateAirThrowBrake). */
#include "xmen2.h"

void Gambit_AirThrowAim(register Gambit *self)
{
	if (!self->pad ? g_Pad1Held & g_PadMaskUp : g_Pad2Held & g_PadMaskUp) {
		if (!self->pad ? g_Pad1Held & g_PadMaskLeftRight : g_Pad2Held & g_PadMaskLeftRight) {
			Obj_SetAnim(self, CODEPTR(Tbl_Anim_1CEDDA));
			self->throwKind = 6;
			self->state = Gambit_StateThrow;
			Gambit_StateThrow(self);
		} else {
			Obj_SetAnim(self, CODEPTR(Tbl_Anim_1CEDCA));
			self->throwKind = 5;
			self->state = Gambit_StateThrow;
			Gambit_StateThrow(self);
		}
	} else if (!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown) {
		if (!self->pad ? g_Pad1Held & g_PadMaskLeftRight : g_Pad2Held & g_PadMaskLeftRight) {
			Obj_SetAnim(self, CODEPTR(Tbl_Anim_1CEDFA));
			self->throwKind = 8;
			self->state = Gambit_StateThrow;
			Gambit_StateThrow(self);
		} else {
			Obj_SetAnim(self, CODEPTR(Tbl_Anim_1CEE0A));
			self->throwKind = 9;
			self->state = Gambit_StateThrow;
			Gambit_StateThrow(self);
		}
	} else {
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1CEDEA));
		self->throwKind = 7;
		self->state = Gambit_StateThrow;
		Gambit_StateThrow(self);
	}
}
