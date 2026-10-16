/* Game_TickFrameCounter - count a frame (g_FrameCounter); reached through its jump-table entry,
 * the corpus never runs it.
 *
 * Returns the count before (MPW leaves it in d0: no return statement). */
#include "xmen2.h"

long Game_TickFrameCounter(void)
{
	g_FrameCounter++;
#ifndef XMEN2_MPW
	return g_FrameCounter - 1;	/* d0 = the old value, loaded by the post-increment */
#endif
}
