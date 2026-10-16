/* SentinelsD1_InitFlagBits - allocate the Sentinels base's 15 words of switch and door flag bits
 * (g_SentinelsD1Flags, Mem_StackAlloc), all clear. BLAST, CONTROLBOX, CONTROLPANEL and the
 * doors and elevators keep pointers into it (Blast_SetFlags). */
#include "xmen2.h"

void SentinelsD1_InitFlagBits(void)
{
	register s16 *p;
	register long n;

	p = g_SentinelsD1Flags = (s16 *)Mem_StackAlloc(0x1E, STR(Str_Empty_0DC2C4, ""));
	n = 15;
	do {
		*p = 0;
		p++;
	} while (--n);
}
