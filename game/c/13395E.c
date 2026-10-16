/* Rocket_SetAngleOwner - aim a rocket that was just made (Rocket_SetAngleA / _SetAngleB): its
 * heading, a high-priority sprite unless its owner's is low (none: high), and x nudged 6 (1/256
 * px) toward the heading's side (heading 0: right, any other: left).
 *
 * rocket: a BRAINROCKET or DROPROCKET.  owner: whoever fired it, or 0.  angle: the heading.
 * marginX, marginY: not used (TANK passes $C0, $5A: the deactivation margins the rocket has,
 * which nothing sets). */
#include "xmen2.h"

void Rocket_SetAngleOwner(BrainRocket *rocket, Actor *owner, short angle, short marginX, short marginY)
{
	if (owner) {
		if (owner->spr.flags & 4)
			rocket->spr.flags |= 4;
	} else
		rocket->spr.flags |= 4;
	rocket->angle = angle;
	if (angle == 0)
		rocket->x += 6;
	else
		rocket->x -= 6;
}
