/* Mine_StateArm1 - mine handler: first arming step: the faster blink Tbl_MineAnimArm1 (its first
 * step's frame shown), then Mine_StateArm2 in 20 frames. (Unlike a spore's, silent.)
 *
 * m: the mine. */
#include "xmen2.h"

void Mine_StateArm1(register MineRec *m)
{
	m->anim = (u8 *)CODEPTR(Tbl_MineAnimArm1);
	m->frame = *(m->anim + 7);	/* step 0's frame byte (step[0].value) */
	m->animStep = 0;
	m->animTime = 0;
	m->handler = Mine_StateArm2;
	m->delay = 0x14;
}
