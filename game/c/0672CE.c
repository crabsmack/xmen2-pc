/* ScalingActor_SetFrame - SCALINGACTOR class slot +$56: show frame f the way its slot's character
 * shows its frames: Beast_SetFrame, ACTOR's for Cyclops, Gambit_SetFrame, Nightcrawler_SetFrame,
 * Psylocke_SetFrame, Wolverine_SetFrame, Magneto_SetFrame (cases ScalingActor_SetFrame_part1 and
 * _part2); no character, nothing (_part3, the exit).
 *
 * self: the portrait.  f: the frame. */
#include "xmen2.h"

void ScalingActor_SetFrame(register ScalingActor *self, register short f)
{
	switch (self->slot->charIndex) {
	case 0:
		Beast_SetFrame((Beast *)self, f);
		break;
	case 1:
		Actor_SetFrame(self, f);
		break;
	case 2:
		Gambit_SetFrame((Gambit *)self, f);
		break;
	case 3:
		Nightcrawler_SetFrame((Nightcrawler *)self, f);
		break;
	case 4:
		Psylocke_SetFrame((Psylocke *)self, f);
		break;
	case 5:
		Wolverine_SetFrame((Wolverine *)self, f);
		break;
	case 6:
		Magneto_SetFrame((Magneto *)self, f);
		break;
	}
}
