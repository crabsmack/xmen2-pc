/* SentinelsB1_InstallSegHooks - the girder stage's floor-segment type hooks: type 27
 * SentinelsB_SegIsMode3, type 26 SentinelsB_ExitSeg (the stage exit). */
#include "xmen2.h"

void SentinelsB1_InstallSegHooks(void)
{
	g_SegTypeHooks[27] = SentinelsB_SegIsMode3;
	g_SegTypeHooks[26] = SentinelsB_ExitSeg;
}
