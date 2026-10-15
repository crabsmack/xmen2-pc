/* Sys_GetStackPtr - the stack pointer as a 24-bit address.
 *
 * Hand-written 68000 (move.l a7,d0 / andi.l #$FFFFFF). Its caller, Mem_StackAlloc, discards
 * the value: probably a removed heap/stack collision check. */
#include "xmen2.h"

long Sys_GetStackPtr(void)
{
	return CPU_GetSP() & ((1L << 24) - 1);	/* the 68000's 24 address bits */
}
