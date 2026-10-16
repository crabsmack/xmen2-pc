/* Mine_StateArm3 - mine handler: third arming step: the blink Tbl_MineAnimArm3 (its first step's
 * frame shown), then Mine_StateArm4 in 13 frames.
 *
 * m: the mine. */
#include "xmen2.h"

void Mine_StateArm3(register MineRec *m)
{
	m->anim = (u8 *)CODEPTR(Tbl_MineAnimArm3);
	m->frame = *(m->anim + 7);
	m->animStep = 0;
	m->animTime = 0;
	m->handler = Mine_StateArm4;
	m->delay = 0xD;
}
