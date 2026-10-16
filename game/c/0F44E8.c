/* SentinelsB_ExitSeg - a Sentinels floor-segment type hook (the stage exit: type 26 in SentinelsB1,
 * $1B in SentinelsB2): for message 3, unless the scene is already ending, end it in 30 frames
 * (g_SceneExitTimer); in SentinelsB2 only while the escape countdown runs, and in 5 frames.
 *
 * msg: the hook's message (the other arguments are unused).
 * Returns 1 for message 3, else 0. */
#include "xmen2.h"

#define g_EscapeTime	(*(s16 *)g_SceneVar3B02)	/* scene variable: escape countdown, tenths of a second */

short SentinelsB_ExitSeg(short msg)
{
	if (msg == 3) {
		if (!g_SceneExitTimer) {
			if (g_ScnSentinelsB2 == g_SceneIndex) {
				if (g_EscapeTime)
					g_SceneExitTimer = 5;
			} else
				g_SceneExitTimer = 30;
		}
		return 1;
	}
	return 0;
}
