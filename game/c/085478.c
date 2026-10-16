/* Teleporter_Init - TELEPORTER Init (class slot +$12): the parent's (ACTOR) Init, then a pad: no
 * vertical speed or gravity, state and default state Teleporter_StateStart, flags6 |= $4083; its
 * channel from placement param 0 (0 without a placement), and the placement of the first
 * TELEPORTRECEIVER spawn record of that channel (g_SpawnRecs) as its receiver.
 *
 * self: the pad.  x, y: position (8 fraction bits).  pl: its placement entry, or null.
 * With no receiver of its channel, receiver is left as it was. */
#include "xmen2.h"

void Teleporter_Init(Teleporter *self, long x, long y, Placement *pl)
{
	char found;
	long channel;
	long i;
	register SpawnRec *rec;

	(*CLASS(Tbl_Teleporter_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->vy = 0;
	self->ay = 0;
	self->state = (StateFn)Teleporter_StateStart;
	self->defaultState = (StateFn)Teleporter_StateStart;
	self->flags6 |= 0x4083;
	if (pl)
		self->channel = pl->param[0];
	else
		self->channel = 0;
	found = 0;
	channel = self->channel;
	for (i = 0; i < g_SpawnRecCount; i++) {
		rec = &g_SpawnRecs[i];
		if (rec->cls == CLASS(Tbl_TeleportReceiver_vtable)) {
			pl = rec->placement;
			if (pl->param[0] == channel) {
				self->receiver = pl;
				found = 1;
				break;
			}
		}
	}
}
