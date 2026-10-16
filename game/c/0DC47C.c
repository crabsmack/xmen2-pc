/* SentinelsD1_InstallHooks - set up the Sentinels base exterior after loading: the frame hook
 * SentinelsD1_FrameHook, its sprite sets, the warning light, the flag bits, the players at their
 * start (Players_PlaceAtStart), and the gauss bullet pool. */
#include "xmen2.h"

void SentinelsD1_InstallHooks(void)
{
	g_SceneFrameHook = SentinelsD1_FrameHook;
	SentinelsD1_LoadResources();
	SentinelsD1_InitLight();
	SentinelsD1_InitFlagBits();
	Players_PlaceAtStart(0, 0, 0, 0);
	GaussBullet_InitPool();
}
