/* Rocket_SetAngleA - aim a rocket that was just made (SAVAGEBIO, CHILDSHIP...): Rocket_SetAngleOwner
 * with no margins.
 *
 * rocket: a BRAINROCKET or DROPROCKET.  owner: whoever fired it.  angle: the heading, degrees. */
#include "xmen2.h"

void Rocket_SetAngleA(void *rocket, void *owner, short angle)
{
	Rocket_SetAngleOwner((BrainRocket *)rocket, (Actor *)owner, angle, 0, 0);
}
