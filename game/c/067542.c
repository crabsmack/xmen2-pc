/* PlayerSelect_PlayIdleAnim - a standing portrait plays its character's idle pose on the player
 * select (cases PlayerSelect_PlayIdleAnim_part1 .. _part6; _part7 is the exit, also with no
 * character).
 *
 * self: the portrait. */
#include "xmen2.h"

void PlayerSelect_PlayIdleAnim(register ScalingActor *self)
{
	switch (self->slot->charIndex) {
	case 0:
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_182932));
		break;
	case 1:
		Obj_SetAnim(self, CODEPTR(Tbl_CyclopsSelectIdleAnim));
		break;
	case 2:
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1CEC5C));
		break;
	case 3:
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A5F46));
		break;
	case 4:
		Obj_SetAnim(self, CODEPTR(Tbl_PlayerSelectAnim_2));
		break;
	case 5:
		Obj_SetAnim(self, CODEPTR(Tbl_PlayerSelectAnim));
		break;
	case 6:
		Obj_SetAnim(self, CODEPTR(Tbl_PlayerSelectAnim_3));
		break;
	}
}
