/* Irq_Enable - unmask all interrupts (SR interrupt mask = 0).
 *
 * Hand-written 68000 (andi #$F8FF,sr); paired with Irq_Disable around VDP and Z80 work. */
#include "xmen2.h"

void Irq_Enable(void)
{
	CPU_SetSR(CPU_GetSR() & 0xF8FF);
}
