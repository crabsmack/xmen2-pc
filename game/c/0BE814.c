/* Launcher_AddShot - fire a shot from the launcher: the first free slot gets a shot 10 px to the
 * side it flies to (right for vx > 0, else left) and 16 px above the launcher.
 *
 * self: the launcher.  vx: the shot's speed (Launcher_StateFire: +-shotSpeed).
 * With all four slots in use nothing happens. */
#include "xmen2.h"

void Launcher_AddShot(register Launcher *self, register short vx)
{
	register LauncherShot *shot;

	for (shot = self->shots; shot < &self->shots[4]; shot++) {
		if (!shot->x) {
			shot->x = self->x + (vx > 0 ? (short)0xA00 : (short)-0xA00);
			shot->y = self->y - 0x1000;
			shot->vx = vx;
			break;
		}
	}
}
