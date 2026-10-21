/* Irq_Disable - mask all interrupts (SR interrupt mask = 7).
 *
 * Hand-written 68000 (ori #$700,sr). */
#include "xmen2.h"

void Irq_Disable(void)
{
	plat_set_sr(plat_sr() | 0x0700);
}
