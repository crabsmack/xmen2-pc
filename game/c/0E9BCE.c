/* Hologram_LockArena - the players are in: pan the camera to the arena (x $140, y $240) and lock
 * it there, and close the gate behind them (its segment's bit $40 off).
 *
 * self: the hologram (Hologram_WaitPlayersInArena). */
#include "xmen2.h"

void Hologram_LockArena(Hologram *self)
{
	(*g_Camera->cls->method[12])(g_Camera, 0x140, 0x240, 2, 2, 1, 1);	/* PanTo */
	self->gateSeg->flags &= ~0x40;
}
