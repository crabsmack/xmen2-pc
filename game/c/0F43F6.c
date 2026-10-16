/* SentinelsB1_InstallHooks - set up the girder stage after loading: the frame hook
 * SentinelsB1_FrameHook, the empty SentinelsB1_EmptyStub, the Sentinels sprite sets, warning light
 * and flag bits (SentinelsD1's), the players at their start (Players_PlaceAtStart), and the gauss
 * bullet pool. */
#include "xmen2.h"

void SentinelsB1_InstallHooks(void)
{
	g_SceneFrameHook = SentinelsB1_FrameHook;
	SentinelsB1_EmptyStub();
	SentinelsD1_LoadResources();
	SentinelsD1_InitLight();
	SentinelsD1_InitFlagBits();
	Players_PlaceAtStart(0, 0, 0, 0);
	GaussBullet_InitPool();
}
