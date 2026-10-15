/* Exodus_Update - EXODUS Update (class slot +$4A): the parent's (ACTOR) Update, then its own
 * timers, the hot floor and the arena bounds.
 *
 * self: the boss.
 * hittableTimer counts down: at 0 it leaves the enemy body hit list. blinkTimer counts down: at
 * 0 the hit blink (flags6 $40) ends. Both players are hurt on the hot floor tiles
 * (Exodus_HurtOnHotTiles). Outside x 0-$180, y 0-$380 (pixels) it stops, snaps to its pixel
 * position and starts over with Exodus_StateDescend (also its default state from now on). */
#include "xmen2.h"

void Exodus_Update(register Exodus *self)
{
	register short px, py;

	(*CLASS(Tbl_Exodus_vtable)->parent->method[17])(self);	/* ACTOR Update */
	if (self->hittableTimer && !--self->hittableTimer) {
		List_Unlink(self->bodyNode, &g_HitListEnemyBody);
		self->bodyNode = 0;
	}
	if (self->blinkTimer && !--self->blinkTimer)
		self->flags6 &= ~0x40;
	Exodus_HurtOnHotTiles(&g_Player1);
	Exodus_HurtOnHotTiles(&g_Player2);
	px = self->x >> 8;
	py = self->y >> 8;
	if (px < 0 || px > 0x180 || py < 0 || py > 0x380) {
		STOP_MOVING(self);
		self->moveFlags = 0;
		self->x = self->px << 8;
		self->y = self->py << 8;
		self->timer = 0;
		self->state = self->defaultState = Exodus_StateDescend;
	}
}
