/* Exodus_AtkSwoopA - EXODUS attack: swoop down to x $80 over floor segment A and fire a kind-5
 * blast that breaks it (Exodus_StateSwoopStart).
 *
 * self: the boss. Returns self->flags & 8.
 * Marks swoop A done, so Exodus_TakeDamage doesn't force it again. */
#include "xmen2.h"

short Exodus_AtkSwoopA(register Exodus *self)
{
	self->targetX = 0x80;
	self->swoopADone = 1;
	self->breakSeg = self->segA;
	self->swoopBlastKind = 5;
	SET_STATE(self, Exodus_StateSwoopStart);
	return self->flags & 8;
}
