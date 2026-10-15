/* Magneto_ShotAnim - MAGNETO: the wind-up animation of a shot on the ground in aim direction dir.
 *
 * dir: aim direction 1-8 (1/8 down-diagonal, 2/7 level, 3/6 up-diagonal, 4/5 up; on the ground
 *      there is no straight-down shot).
 * Returns the animation script (Tbl_MagnetoShotDownDiagAnim, Tbl_MagnetoAnim5 level,
 * Tbl_MagnetoAnim3 up-diagonal, Tbl_MagnetoShotUpAnim); 0 for any other dir. Cases:
 * Magneto_ShotAnim_Case1/2/4/7/8 (the others are unnamed blocks), _Default, _Result, _Ret. */
#include "xmen2.h"

void *Magneto_ShotAnim(short dir)
{
	register void *anim;

	switch (dir) {
	case 1:			/* Magneto_ShotAnim_Case1 */
		anim = CODEPTR(Tbl_MagnetoShotDownDiagAnim);
		break;
	case 2:			/* Magneto_ShotAnim_Case2 */
		anim = CODEPTR(Tbl_MagnetoAnim5);
		break;
	case 3:
		anim = CODEPTR(Tbl_MagnetoAnim3);
		break;
	case 4:			/* Magneto_ShotAnim_Case4 */
		anim = CODEPTR(Tbl_MagnetoShotUpAnim);
		break;
	case 5:
		anim = CODEPTR(Tbl_MagnetoShotUpAnim);
		break;
	case 6:
		anim = CODEPTR(Tbl_MagnetoAnim3);
		break;
	case 7:			/* Magneto_ShotAnim_Case7 */
		anim = CODEPTR(Tbl_MagnetoAnim5);
		break;
	case 8:			/* Magneto_ShotAnim_Case8 */
		anim = CODEPTR(Tbl_MagnetoShotDownDiagAnim);
		break;
	default:		/* Magneto_ShotAnim_Default */
		return 0;
	}
	return anim;		/* Magneto_ShotAnim_Result, _Ret */
}
