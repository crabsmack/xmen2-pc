/* Exodus_AtkSwoopB - EXODUS attack: swoop down to x $100 over floor segment B and fire a kind-6
 * blast that breaks it (Exodus_StateSwoopStart).
 *
 * self: the boss. Returns self->flags & 8.
 * Marks swoop B done, so Exodus_TakeDamage doesn't force it again. */
#include "xmen2.h"

short Exodus_AtkSwoopB(register Exodus *self)
{
	self->targetX = 0x100;
	self->swoopBDone = 1;
	self->breakSeg = self->segB;
	self->swoopBlastKind = 6;
	SET_STATE(self, Exodus_StateSwoopStart);
	return self->flags & 8;
}
