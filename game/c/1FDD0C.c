/* Irq_HBlank - the H-int handler (the hook at $214 that Irq_HBlankVector, the level-4 vector,
 * calls): count it (g_HBlankCount) and switch H-ints off again (VDP register 0 = $04). */
#include "xmen2.h"

void Irq_HBlank(void)
{
	if (g_HBlankCount++) {
	}
	VDP_CTRL = (short)0x8004;
}
