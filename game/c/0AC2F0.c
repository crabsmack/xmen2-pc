/* FloatBubbleExplode_ClassInit - per-level class setup of FLOATBUBBLEEXPLODE (class slot +$5E):
 * only its empty hook FloatBubbleExplode_Nop6A.
 *
 * self: 0 (the slot is called on the class, with the class's index in the level's class list
 *       as a second, ignored argument).
 * A data register is saved and restored with nothing in it (an unused register variable). */
#include "xmen2.h"

void FloatBubbleExplode_ClassInit(FloatBubbleExplode *self)
{
	register short unused;

	if (0)
		unused = 0;
	if (1) {	/* the ROM pops the call's argument at the end of this block */
		FloatBubbleExplode_Nop6A(self);
	}
}
