/* BanimazaDoor_StateShattered - BANIMAZADOOR state once shattered: with a respawn delay, wait
 * that long in BanimazaDoor_StateWaitRespawn; with none, kill the door.
 *
 * self: the door. Returns 0 when it waits, no value when killed. */
#include "xmen2.h"

short BanimazaDoor_StateShattered(register BanimazaDoor *self)
{
	if (self->respawnDelay) {
		self->state = BanimazaDoor_StateWaitRespawn;
		self->timer = self->respawnDelay;
		return 0;
	}
	(*self->cls->method[19])(self);		/* +$52 Kill */
	NO_RETURN_VALUE
}
