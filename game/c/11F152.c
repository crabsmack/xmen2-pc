/* BioBalls_StateAnimB - BIOBALLS state: play animation Tbl_BioBallsAnim; its end (or 255 frames)
 * goes on with BioBalls_ActClearAnim.
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short BioBalls_StateAnimB(BioBalls *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_BioBallsAnim));
	SET_STATE_WAIT(self, (StateFn)BioBalls_ActClearAnim, 0xFF);
	return self->flags & 8;
}
