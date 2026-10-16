/* LaserCannon_StateRetract - LASERCANNON state after the last shot of a burst: lower the barrel
 * (Tbl_LaserCannonAnimRetract), then LaserCannon_StateReload when the animation ends (delay $FF).
 *
 * self: the cannon. Returns flags & 8. */
#include "xmen2.h"

short LaserCannon_StateRetract(LaserCannon *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_LaserCannonAnimRetract));
	SET_STATE_WAIT(self, (StateFn)LaserCannon_StateReload, 0xFF);
	return self->flags & 8;
}
