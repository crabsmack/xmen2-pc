/* Scene_ClearActivateHook - no override of the level loop's object activation pass any more
 * (g_ActivateHook 0: Level_FrameLoop walks the objects itself). */
#include "xmen2.h"

void Scene_ClearActivateHook(void)
{
	g_ActivateHook = 0;
}
