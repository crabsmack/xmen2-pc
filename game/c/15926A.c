/* Magneto_ShotEndAnim - MAGNETO: the follow-through animation after a shot on the ground in aim
 * direction dir.
 *
 * dir: aim direction 1-8 (1/8 down-diagonal, 2/7 level, 3/6 up-diagonal, 4/5 up).
 * Returns the animation script (Tbl_MagnetoShotDownDiagEndAnim, Tbl_MagnetoAnim6 level,
 * Tbl_MagnetoAnim4 up-diagonal, Tbl_MagnetoShotUpEndAnim); 0 for any other dir. Cases:
 * Magneto_ShotEndAnim_Case1/2/4/7/8 (the others are unnamed blocks), _Default, _Result, _Ret.
 *
 * Inline here, separate blocks in the disassembly: Magneto_ShotEndAnim_Ret.
 */
#include "xmen2.h"

void *Magneto_ShotEndAnim(short dir)
{
	register void *anim;

	switch (dir) {
	case 1:			/* Magneto_ShotEndAnim_Case1 */
		anim = CODEPTR(Tbl_MagnetoShotDownDiagEndAnim);
		break;
	case 2:			/* Magneto_ShotEndAnim_Case2 */
		anim = CODEPTR(Tbl_MagnetoAnim6);
		break;
	case 3:
		anim = CODEPTR(Tbl_MagnetoAnim4);
		break;
	case 4:			/* Magneto_ShotEndAnim_Case4 */
		anim = CODEPTR(Tbl_MagnetoShotUpEndAnim);
		break;
	case 5:
		anim = CODEPTR(Tbl_MagnetoShotUpEndAnim);
		break;
	case 6:
		anim = CODEPTR(Tbl_MagnetoAnim4);
		break;
	case 7:			/* Magneto_ShotEndAnim_Case7 */
		anim = CODEPTR(Tbl_MagnetoAnim6);
		break;
	case 8:			/* Magneto_ShotEndAnim_Case8 */
		anim = CODEPTR(Tbl_MagnetoShotDownDiagEndAnim);
		break;
	default:		/* Magneto_ShotEndAnim_Default */
		return 0;
	}
	return anim;		/* Magneto_ShotEndAnim_Result, _Ret */
}
