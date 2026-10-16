/* Pal_RunCycleScript - the level's palette-cycle script, once per frame (g_LevelPalHook): each
 * time its delay runs out, copy the next run of colour words into g_PalBase[first..last].
 *
 * The script (g_PalCycleScript, PalCycleScript): a header, then the steps' colour words; a word
 * -1 where a step would start loops to the first, -2 ends the script (g_PalCycleScript = 0); a
 * word >= $1000 after a step gives that step its own delay (& $3FF), else the header's. The
 * palette is marked for upload only when no fade is running. */
#include "xmen2.h"

void Pal_RunCycleScript(void)
{
	short i;

	if (g_PalCycleScript) {
		if (g_PalCycleDelay-- < 1) {
			if (!g_PalCycleCursor)
				g_PalCycleCursor = (s16 *)(g_PalCycleScript + 1);
			if (*g_PalCycleCursor == -1)
				g_PalCycleCursor = (s16 *)(g_PalCycleScript + 1);
			if (*g_PalCycleCursor == -2) {
				g_PalCycleScript = 0;
			} else {
				g_PalBaseDirty = !g_PalFadeActive;
				for (i = g_PalCycleScript->first; i <= g_PalCycleScript->last; i++) {
					g_PalBase[i] = *g_PalCycleCursor;
					++g_PalCycleCursor;
				}
				if (*g_PalCycleCursor >= 0x1000) {
					g_PalCycleDelay = *g_PalCycleCursor & 0x3FF;
					++g_PalCycleCursor;
				} else {
					g_PalCycleDelay = g_PalCycleScript->delay;
				}
			}
		}
	}
}
