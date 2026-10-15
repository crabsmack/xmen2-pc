/* BroodBoss_StateDropBomb2 - BROODBOSS state: frame 5, drop a DROPBOMB at the top of the view
 * (camera + $7D, $B), then BroodBoss_StateAfterBomb.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_StateDropBomb2(BroodBoss *self)
{
	(*self->cls->method[20])(self, 5);		/* +$56 SetFrame */
	DropBomb_Spawn((g_CameraX + 0x7D) << 8, (g_CameraY + 0xB) << 8, 0);
	SET_STATE(self, (StateFn)BroodBoss_StateAfterBomb);
	return self->flags & 8;
}
