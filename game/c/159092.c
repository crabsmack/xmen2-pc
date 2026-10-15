/* Magneto_AirShotAnim - MAGNETO: the wind-up animation of an air shot in aim direction dir.
 *
 * dir: aim direction 0-9 (0/9 down, 1/8 down-diagonal, 2/7 level, 3/6 up-diagonal, 4/5 up;
 *      the second of each pair faces the other way).
 * Returns the animation script (Tbl_MagnetoAnim13 down, Tbl_MagnetoAnim11 down-diagonal,
 * Tbl_MagnetoAnim9 level, Tbl_MagnetoAirShotUpDiagAnim, Tbl_MagnetoAirShotUpAnim); 0 for any
 * other dir. Cases: Magneto_AirShotAnim_Case0/2/3/7/9 (the others are unnamed blocks), _Default,
 * _Result, _Ret. MAGNETOBOSS uses it too (MagnetoBoss_StateShoot). */
#include "xmen2.h"

void *Magneto_AirShotAnim(short dir)
{
	register void *anim;

	switch (dir) {
	case 0:			/* Magneto_AirShotAnim_Case0 */
		anim = CODEPTR(Tbl_MagnetoAnim13);
		break;
	case 1:
		anim = CODEPTR(Tbl_MagnetoAnim11);
		break;
	case 2:			/* Magneto_AirShotAnim_Case2 */
		anim = CODEPTR(Tbl_MagnetoAnim9);
		break;
	case 3:			/* Magneto_AirShotAnim_Case3 */
		anim = CODEPTR(Tbl_MagnetoAirShotUpDiagAnim);
		break;
	case 4:
		anim = CODEPTR(Tbl_MagnetoAirShotUpAnim);
		break;
	case 5:
		anim = CODEPTR(Tbl_MagnetoAirShotUpAnim);
		break;
	case 6:
		anim = CODEPTR(Tbl_MagnetoAirShotUpDiagAnim);
		break;
	case 7:			/* Magneto_AirShotAnim_Case7 */
		anim = CODEPTR(Tbl_MagnetoAnim9);
		break;
	case 8:
		anim = CODEPTR(Tbl_MagnetoAnim11);
		break;
	case 9:			/* Magneto_AirShotAnim_Case9 */
		anim = CODEPTR(Tbl_MagnetoAnim13);
		break;
	default:		/* Magneto_AirShotAnim_Default */
		return 0;
	}
	return anim;		/* Magneto_AirShotAnim_Result, _Ret */
}
