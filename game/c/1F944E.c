/* Scene_EndAfter - end the current scene in n frames (g_SceneExitTimer; the scene loop counts it
 * down, by 2 with g_TwoPlayerStep).
 *
 * n: frames. */
#include "xmen2.h"

void Scene_EndAfter(short n)
{
	g_SceneExitTimer = n;
}
