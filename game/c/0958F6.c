/* XClone_OnHurt - XCLONE was hit (class slot +$6E): drop the hurt flags, flash for $1E frames and
 * switch to the clone record's hurt script: stopped, no animation, falling to the floor first
 * (XClone_StateLand in 10 frames).
 *
 * self: the clone.
 * At 120 recent hits (two within a second) the count starts again. When the hurt script is
 * already running nothing else changes. */
#include "xmen2.h"

void XClone_OnHurt(XClone *self)
{
	void *script;

	script = CLONE_PART(self->record, hurtScript);
	self->flags &= ~0x200;
	self->flags &= ~4;
	self->flags6 |= 0x40;
	self->flashTimer = 0x1E;
	if (self->recentHits >= 120)
		self->recentHits = 0;
	if (script != self->ai.table) {
		AiScript_Set(&self->ai, script);
		self->anim = 0;
		self->ax = 0;
		self->vx = 0;
		self->timer = 10;
		self->state = XClone_StateLand;
	}
}
