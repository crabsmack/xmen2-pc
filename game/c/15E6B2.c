/* Actor_Kill - ACTOR Kill (class slot +$52): destroy an object.
 *
 * self: the object.
 * It is detached from what it rides and drops what it carries (Obj_Detach on each attached
 * object), is Deactivated when present and alive, becomes unused (flags 0) and its timer is
 * freed; its spawn record (spawnRec, when not $FF) is no longer live and has no object. */
#include "xmen2.h"

void Actor_Kill(Actor *self)
{
	Obj_Detach(self);
	if (self->spr.firstChild != 0xFF)
		Obj_ForEachChildCall(self, (ObjChildFn)Obj_Detach);
	if (IS_ALIVE(self))
		(*self->cls->method[16])(self);		/* +$46 Deactivate */
	self->flags = 0;
	Timer_Free(self->timerObj);
	self->timerObj = 0;
	if (self->spawnRec != 0xFF) {
		g_SpawnRecs[self->spawnRec].flags &= ~2;
		g_SpawnRecs[self->spawnRec].object = 0xFF;
	}
}
