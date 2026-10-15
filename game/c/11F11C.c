/* BioBalls_StateAnimA - BIOBALLS state: play animation Tbl_XCloneAnim7; its end (or 255 frames)
 * goes on with BioBalls_ActClearAnim.
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short BioBalls_StateAnimA(BioBalls *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_XCloneAnim7));
	SET_STATE_WAIT(self, (StateFn)BioBalls_ActClearAnim, 0xFF);
	return self->flags & 8;
}
