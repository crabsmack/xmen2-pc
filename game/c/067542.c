/* PlayerSelect_PlayIdleAnim - a standing portrait plays its character's idle pose on the player
 * select (cases PlayerSelect_PlayIdleAnim_part1 .. _part6; _part7 is the exit, also with no
 * character). That is each character's standing animation (CharRecord.walkAnim), never its idle.
 * Native, the select_idles option: first the draw a standing player makes (P34_SelectIdle).
 *
 * self: the portrait.
 *
 * Inline here, separate blocks in the disassembly: PlayerSelect_PlayIdleAnim_part2, PlayerSelect_PlayIdleAnim_part3, PlayerSelect_PlayIdleAnim_part4, PlayerSelect_PlayIdleAnim_part5, PlayerSelect_PlayIdleAnim_part6, PlayerSelect_PlayIdleAnim_part7.
 */
#include "xmen2.h"

void PlayerSelect_PlayIdleAnim(register ScalingActor *self)
{
#if defined(XMEN2_NATIVE)
	if (g_opt.select_idles && P34_SelectIdle(self))	/* the select_idles option: now and then its idle */
		return;
#endif
	switch (self->slot->charIndex) {
	case 0:
		Obj_SetAnim(self, CODEPTR(Tbl_BeastSelectIdleAnim));
		break;
	case 1:
		Obj_SetAnim(self, CODEPTR(Tbl_CyclopsSelectIdleAnim));
		break;
	case 2:
		Obj_SetAnim(self, CODEPTR(Tbl_GambitSelectIdleAnim));
		break;
	case 3:
		Obj_SetAnim(self, CODEPTR(Tbl_NightcrawlerSelectIdleAnim));
		break;
	case 4:
		Obj_SetAnim(self, CODEPTR(Tbl_PsylockeSelectIdleAnim));
		break;
	case 5:
		Obj_SetAnim(self, CODEPTR(Tbl_WolverineSelectIdleAnim));
		break;
	case 6:
		Obj_SetAnim(self, CODEPTR(Tbl_MagnetoSelectIdleAnim));
		break;
	}
}
