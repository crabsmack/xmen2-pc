/* BroodBoss_StateDropBomb - BROODBOSS state: frame 5, drop a DROPBOMB near the top left of the
 * view (camera + $2D, $B), then BroodBoss_StateAfterBomb.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_StateDropBomb(BroodBoss *self)
{
	(*self->cls->method[20])(self, 5);		/* +$56 SetFrame */
	DropBomb_Spawn((g_CameraX + 0x2D) << 8, (g_CameraY + 0xB) << 8, 0);
	SET_STATE(self, (StateFn)BroodBoss_StateAfterBomb);
	return self->flags & 8;
}
