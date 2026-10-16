/* Pad_Get - the buttons of player 1 or 2 now: read from the pad (interrupts off, the Z80 off the
 * bus), or while a demo plays (g_DemoActive) the recorded input for player 1 and nothing for 2.
 *
 * p1: 1 player 1, 0 player 2.
 * Returns the buttons; with both Left and Right (or Up and Down) down, neither counts. */
#include "xmen2.h"

short Pad_Get(register char p1)
{
	register short pad;

	if (g_DemoActive) {
		if (p1)
			return g_DemoPad;
		return 0;
	}
	Irq_Disable();
	Z80_BusRequest();
	if (p1)
		pad = Pad_GetP1Clean();
	else
		pad = Pad_GetP2Clean();
	Z80_BusFree();
	Irq_Enable();
	if ((pad & 0xC) == 0xC)
		pad &= ~0xC;
	if ((pad & 3) == 3)
		pad &= ~3;
	return pad;
}
