/* MagnetoBoss_StateOutroLoop - MAGNETOBOSS outro, last state: Tbl_MagnetoBossAnim17 (again each
 * time the state runs).
 *
 * self: the boss. */
#include "xmen2.h"

void MagnetoBoss_StateOutroLoop(MagnetoBoss *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_MagnetoBossAnim17));
}
