/* Tusk_Defeated - TUSK's state at 0 health (Tusk_TakeDamage): end the scene in 200 frames and
 * play death effect 3, the bouncing puffs (ExplodeFx_Bounce).
 *
 * self: TUSK.
 * The effect is a switch on the constant 3 (0 or other: remove it (class slot +$52 Kill),
 * 1 ExplodeFx_BurstMedium, 2 ExplodeFx_BurstSmall, 3 ExplodeFx_Bounce, 4 ExplodeFx_TrailArc),
 * which MPW C compiles as a full switch. Its cases are Tusk_Defeated_part1.._part5 in the
 * disassembly. */
#include "xmen2.h"

void Tusk_Defeated(Tusk *self)
{
	g_SceneExitTimer = 200;
	switch (3) {
	case 0:				/* Tusk_Defeated_part1 */
	default:
		(*self->cls->method[19])(self);		/* +$52 Kill */
		break;
	case 1:				/* Tusk_Defeated_part2 */
		ExplodeFx_BurstMedium(self);
		break;
	case 2:				/* (Tusk_Defeated_part2 too) */
		ExplodeFx_BurstSmall(self);
		break;
	case 3:				/* Tusk_Defeated_part3 */
		ExplodeFx_Bounce(self);
		break;
	case 4:				/* Tusk_Defeated_part4 */
		ExplodeFx_TrailArc(self);
		break;
	}
}				/* Tusk_Defeated_part5: the exit */
