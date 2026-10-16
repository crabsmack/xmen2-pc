/* SentinelsB_ExitSeg - a Sentinels floor-segment type hook (type 26: the stage exit): for message 3,
 * unless the scene is already ending, end it in 30 frames (g_SceneExitTimer); in SentinelsB2 only
 * while its alarm countdown runs, and in 5 frames.
 *
 * msg: the hook's message (the other arguments are unused).
 * Returns 1 for message 3, else 0. */
#include "xmen2.h"

#define g_B2AlarmCountdown	(*(s16 *)g_SceneVar3B02)	/* scene variable: SentinelsB2's alarm countdown */

short SentinelsB_ExitSeg(short msg)
{
	if (msg == 3) {
		if (!g_SceneExitTimer) {
			if (g_ScnSentinelsB2 == g_SceneIndex) {
				if (g_B2AlarmCountdown)
					g_SceneExitTimer = 5;
			} else
				g_SceneExitTimer = 30;
		}
		return 1;
	}
	return 0;
}
