/* Irq_Disable - mask all interrupts (SR interrupt mask = 7).
 *
 * Hand-written 68000 (ori #$700,sr). */
#include "xmen2.h"

void Irq_Disable(void)
{
	CPU_SetSR(CPU_GetSR() | 0x0700);
}
