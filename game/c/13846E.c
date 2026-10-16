/* LaserCannon_StateExtend - LASERCANNON state: raise the barrel (Tbl_LaserCannonAnimExtend), then
 * LaserCannon_StateLoadBurst when the animation ends (delay $FF).
 *
 * self: the cannon. Returns flags & 8. */
#include "xmen2.h"

short LaserCannon_StateExtend(LaserCannon *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_LaserCannonAnimExtend));
	SET_STATE_WAIT(self, (StateFn)LaserCannon_StateLoadBurst, 0xFF);
	return self->flags & 8;
}
