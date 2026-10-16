/* BioBalls_StateIdle - BIOBALLS state: do nothing, every frame (its AI script's one step,
 * Tbl_BioBalls_ScriptIdle).
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short BioBalls_StateIdle(BioBalls *self)
{
	SET_STATE(self, BioBalls_StateIdle);
	return self->flags & 8;
}
