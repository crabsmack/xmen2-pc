/* Rocket_SetAngleB - aim a rocket that was just made (TANK): Rocket_SetAngleOwner with
 * deactivation margins, which it doesn't use.
 *
 * rocket: a BRAINROCKET.  owner: whoever fired it.  angle: the heading, degrees.
 * marginX, marginY: passed on. */
#include "xmen2.h"

void Rocket_SetAngleB(void *rocket, void *owner, short angle, short marginX, short marginY)
{
	Rocket_SetAngleOwner((BrainRocket *)rocket, (Actor *)owner, angle, marginX, marginY);
}
