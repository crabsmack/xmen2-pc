/* PlayerSelect_PlayPickAnim - a portrait that has landed plays its character's pick pose and
 * attack sound (CharRecord.attackSound, unless $FF; Magneto has no record here, so no sound).
 * Cases PlayerSelect_PlayPickAnim_part1, _part2 (Beast, Cyclops) and the rest of _part2; _part3
 * plays the sound.
 *
 * self: the portrait. With no character the 68000 tests whatever its caller left in a4 (never
 * happens: a portrait lands with a character); the PC version plays nothing then. */
#include "xmen2.h"

void PlayerSelect_PlayPickAnim(register ScalingActor *self)
{
	register CharRecord *rec;

#ifndef XMEN2_MPW
	rec = 0;
#endif
	switch (self->slot->charIndex) {
	case 0:
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_182954));
		rec = (CharRecord *)CODEPTR(Tbl_BeastCharRecord);
		break;
	case 1:
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1E1C60));
		rec = (CharRecord *)CODEPTR(Tbl_CyclopsCharRecord);
		break;
	case 2:
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1CECBE));
		rec = (CharRecord *)CODEPTR(Tbl_GambitCharRecord);
		break;
	case 3:
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A601E));
		rec = (CharRecord *)CODEPTR(Tbl_NightcrawlerCharRecord);
		break;
	case 4:
		Obj_SetAnim(self, CODEPTR(Tbl_PlayerSelectAnim2));
		rec = (CharRecord *)CODEPTR(Tbl_PsylockeCharRecord);
		break;
	case 5:
		Obj_SetAnim(self, CODEPTR(Tbl_PlayerSelectAnim6));
		rec = (CharRecord *)CODEPTR(Tbl_WolverineCharRecord);
		break;
	case 6:
		Obj_SetAnim(self, CODEPTR(Tbl_PlayerSelectAnim2_2));
		rec = 0;
		break;
	}
	if (rec && rec->attackSound != 0xFF) {
		Snd_Stop(rec->attackSound);
		Snd_Play(rec->attackSound);
	}
}
