/* Mine_StateArm2 - mine handler: second arming step: the blink Tbl_MineAnimArm2 (its first step's
 * frame shown), then Mine_StateArm3 in 20 frames.
 *
 * m: the mine. */
#include "xmen2.h"

void Mine_StateArm2(register MineRec *m)
{
	m->anim = (u8 *)CODEPTR(Tbl_MineAnimArm2);
	m->frame = *(m->anim + 7);	/* step 0's frame byte (step[0].value) */
	m->animStep = 0;
	m->animTime = 0;
	m->handler = Mine_StateArm3;
	m->delay = 0x14;
}
