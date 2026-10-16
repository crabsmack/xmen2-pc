/* SentinelsB2_InstallSegHooks - the Sentinels escape's floor-segment type hooks
 * (g_SegTypeHooks): type $15 SentinelsB_HazardSeg, $1A SentinelsB_SegIsMode3, $1B
 * SentinelsB_ExitSeg. */
#include "xmen2.h"

void SentinelsB2_InstallSegHooks(void)
{
	g_SegTypeHooks[0x15] = SentinelsB_HazardSeg;
	g_SegTypeHooks[0x1A] = SentinelsB_SegIsMode3;
	g_SegTypeHooks[0x1B] = SentinelsB_ExitSeg;
}
