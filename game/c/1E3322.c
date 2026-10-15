/* Cyclops_AirBeamAim - CYCLOPS fire the beam in the air in the direction held, then
 * Cyclops_StateAirBeam, now.
 *
 * self: the player.
 * Up: with Left/Right up-diagonal (Tbl_CyclopsBeamAirUpDiag, pose Tbl_CyclopsAirBeamUpDiagAnim),
 * else straight up (Tbl_CyclopsBeamAirUp, Tbl_Anim_1E1CDC). Down: down-diagonal
 * (Tbl_CyclopsBeamAirDownDiag, Tbl_Anim_1E1CF0) or straight down (Tbl_CyclopsBeamAirDown,
 * Tbl_Anim_1E1CE6). Neither: level (Tbl_CyclopsBeamAirLevel, Tbl_Anim_1E1CD2). Not v-flipped. */
#include "xmen2.h"

void Cyclops_AirBeamAim(register Cyclops *self)
{
	if (!self->pad ? g_Pad1Held & g_PadMaskUp : g_Pad2Held & g_PadMaskUp) {
		if (!self->pad ? g_Pad1Held & g_PadMaskLeftRight : g_Pad2Held & g_PadMaskLeftRight) {
			Cyclops_StartBeam(self, CODEPTR(Tbl_CyclopsBeamAirUpDiag), 0);
			Obj_SetAnim(self, CODEPTR(Tbl_CyclopsAirBeamUpDiagAnim));
			self->state = Cyclops_StateAirBeam;
			Cyclops_StateAirBeam(self);
		} else {
			Cyclops_StartBeam(self, CODEPTR(Tbl_CyclopsBeamAirUp), 0);
			Obj_SetAnim(self, CODEPTR(Tbl_Anim_1E1CDC));
			self->state = Cyclops_StateAirBeam;
			Cyclops_StateAirBeam(self);
		}
	} else if (!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown) {
		if (!self->pad ? g_Pad1Held & g_PadMaskLeftRight : g_Pad2Held & g_PadMaskLeftRight) {
			Cyclops_StartBeam(self, CODEPTR(Tbl_CyclopsBeamAirDownDiag), 0);
			Obj_SetAnim(self, CODEPTR(Tbl_Anim_1E1CF0));
			self->state = Cyclops_StateAirBeam;
			Cyclops_StateAirBeam(self);
		} else {
			Cyclops_StartBeam(self, CODEPTR(Tbl_CyclopsBeamAirDown), 0);
			Obj_SetAnim(self, CODEPTR(Tbl_Anim_1E1CE6));
			self->state = Cyclops_StateAirBeam;
			Cyclops_StateAirBeam(self);
		}
	} else {
		Cyclops_StartBeam(self, CODEPTR(Tbl_CyclopsBeamAirLevel), 0);
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1E1CD2));
		self->state = Cyclops_StateAirBeam;
		Cyclops_StateAirBeam(self);
	}
}
