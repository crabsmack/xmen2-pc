/* Mine_StateArm4 - mine handler: last arming step: the fastest blink (Tbl_MineArmAnim, its first
 * step's frame shown), then Mine_Explode in 13 frames.
 *
 * m: the mine. */
#include "xmen2.h"

void Mine_StateArm4(register MineRec *m)
{
	m->anim = (u8 *)CODEPTR(Tbl_MineArmAnim);
	m->frame = *(m->anim + 7);
	m->animStep = 0;
	m->animTime = 0;
	m->handler = Mine_Explode;
	m->delay = 0xD;
}
