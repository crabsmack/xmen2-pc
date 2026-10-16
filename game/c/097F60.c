/* EvilBeast_StateStompRelease - EVILBEAST state after EvilBeast_ActStomp's impact animation: release both
 * stomp shockwaves (EvilBeast_UpdateShockwaves moves them), play the follow-through animation
 * (Tbl_EvilBeastAnim2) and go on with the AI script at its end.
 *
 * self: the clone. */
#include "xmen2.h"

void EvilBeast_StateStompRelease(EvilBeast *self)
{
	self->waves |= 1;
	self->waves |= 2;
	Obj_SetAnim(self, CODEPTR(Tbl_EvilBeastAnim2));
	AI_NEXT_STEP(self, self->ai, 0xFF);
}
