/* CloneA5_ResetPalCycle - restart the boss arena's palette cycle (CloneA5_FrameHook): its
 * Pal_CycleStep state (g_CloneA5PalTimer timer, [1] colour index, [0] sequence index) to 0.
 *
 * MPW: not byte-identical. The original cleared three separate globals ($FFC5B0, $FFC5AE,
 * $FFC5AC: three clr.w); as one array MPW merges the first two clears into a clr.l. With the
 * three declared as separate words the code is the ROM's exactly. */
#include "xmen2.h"

void CloneA5_ResetPalCycle(void)
{
	g_CloneA5PalTimer = 0;
	g_CloneA5PalColour = 0;
	g_CloneA5PalSeq = 0;
}
