/* EvilBeast_StateThrow - EVILBEAST state after EvilBeast_ActThrow's impact animation: release both
 * stomp shockwaves (EvilBeast_UpdateThrows moves them), play the follow-through animation
 * (Tbl_EvilBeastAnim2) and go on with the AI script at its end.
 *
 * self: the clone. */
#include "xmen2.h"

void EvilBeast_StateThrow(EvilBeast *self)
{
	self->waves |= 1;
	self->waves |= 2;
	Obj_SetAnim(self, CODEPTR(Tbl_EvilBeastAnim2));
	AI_NEXT_STEP(self, self->ai, 0xFF);
}
