/* Launcher_ClearShots - free a launcher's four shot slots (x 0).
 *
 * self: the launcher (Launcher_Init). */
#include "xmen2.h"

void Launcher_ClearShots(register Launcher *self)
{
	register LauncherShot *shot;

	for (shot = self->shots; shot < &self->shots[4]; shot++)
		shot->x = 0;
}
