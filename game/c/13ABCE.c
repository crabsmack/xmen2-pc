/* Mine_StateStart - mine handler, its first: play the slow idle blink (Tbl_MineAnimIdle, its first
 * step's frame shown), then look for a player from the next frame (Mine_StatePickTarget).
 *
 * m: the mine. */
#include "xmen2.h"

void Mine_StateStart(register MineRec *m)
{
	m->anim = (u8 *)CODEPTR(Tbl_MineAnimIdle);
	m->frame = *(m->anim + 7);	/* step 0's frame byte (step[0].value) */
	m->animStep = 0;
	m->animTime = 0;
	m->handler = Mine_StatePickTarget;
	m->delay = 0;
}
