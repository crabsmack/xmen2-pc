/* XClone_EndWave - end the clone wave: no more spawning, the collapse effects as the scene hook
 * (XClone_FrameHookCollapse), and the scene ends in 600 frames.
 *
 * Called by XClone_Kill at the 6th kill (never in the corpus). */
#include "xmen2.h"

void XClone_EndWave(void)
{
	g_SpawnEnabled = 0;
	g_SceneFrameHook = XClone_FrameHookCollapse;
	Scene_EndAfter(600);
}
