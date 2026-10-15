/* Magneto_AirShotEndAnim - MAGNETO: the follow-through animation after an air shot in aim
 * direction dir.
 *
 * dir: aim direction 0-9 (0/9 down, 1/8 down-diagonal, 2/7 level, 3/6 up-diagonal, 4/5 up).
 * Returns the animation script (Tbl_MagnetoAnim14 down, Tbl_MagnetoAnim12 down-diagonal,
 * Tbl_MagnetoAnim10 level, Tbl_MagnetoAirShotUpDiagEndAnim, Tbl_MagnetoAirShotUpEndAnim); 0 for
 * any other dir. Cases: Magneto_AirShotEndAnim_Case0/2/3/7/9 (the others are unnamed blocks),
 * _Default, _Result, _Ret. MAGNETOBOSS uses it too (MagnetoBoss_StateShootRecover). */
#include "xmen2.h"

void *Magneto_AirShotEndAnim(short dir)
{
	register void *anim;

	switch (dir) {
	case 0:			/* Magneto_AirShotEndAnim_Case0 */
		anim = CODEPTR(Tbl_MagnetoAnim14);
		break;
	case 1:
		anim = CODEPTR(Tbl_MagnetoAnim12);
		break;
	case 2:			/* Magneto_AirShotEndAnim_Case2 */
		anim = CODEPTR(Tbl_MagnetoAnim10);
		break;
	case 3:			/* Magneto_AirShotEndAnim_Case3 */
		anim = CODEPTR(Tbl_MagnetoAirShotUpDiagEndAnim);
		break;
	case 4:
		anim = CODEPTR(Tbl_MagnetoAirShotUpEndAnim);
		break;
	case 5:
		anim = CODEPTR(Tbl_MagnetoAirShotUpEndAnim);
		break;
	case 6:
		anim = CODEPTR(Tbl_MagnetoAirShotUpDiagEndAnim);
		break;
	case 7:			/* Magneto_AirShotEndAnim_Case7 */
		anim = CODEPTR(Tbl_MagnetoAnim10);
		break;
	case 8:
		anim = CODEPTR(Tbl_MagnetoAnim12);
		break;
	case 9:			/* Magneto_AirShotEndAnim_Case9 */
		anim = CODEPTR(Tbl_MagnetoAnim14);
		break;
	default:		/* Magneto_AirShotEndAnim_Default */
		return 0;
	}
	return anim;		/* Magneto_AirShotEndAnim_Result, _Ret */
}
