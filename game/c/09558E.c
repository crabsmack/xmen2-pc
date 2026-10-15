/* XClone_Update - XCLONE per-frame update (class slot +$4A): the parent's Update, then (unless it
 * stopped the object: flags bit 4) count down the hit flash, the attack cooldown and the recent
 * hits, by 2 a frame in two-player step mode, and run the per-frame hook (slot +$82).
 *
 * self: the clone.
 * The hit flash (flags6 bit $40) ends when its timer reaches 0. */
#include "xmen2.h"

void XClone_Update(XClone *self)
{
	(*CLASS(Tbl_XClone_vtable)->parent->method[17])(self);	/* BIONPC Update */
	if (!(self->flags & 0x10)) {
		if (self->flashTimer > 0) {
			self->flashTimer -= g_TwoPlayerStep ? 2 : 1;
			if (self->flashTimer <= 0)
				self->flags6 &= ~0x40;
		}
		if (self->attackCooldown > 0)
			self->attackCooldown -= g_TwoPlayerStep ? 2 : 1;
		if (self->recentHits > 0)
			self->recentHits -= g_TwoPlayerStep ? 2 : 1;
		(*self->cls->method[31])(self);		/* per-frame hook (XClone_Nop82) */
	}
}
