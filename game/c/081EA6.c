/* CloneA2_InstallHooks - the CloneA2 hooks: floor-segment type $1A hurts the player
 * (SegHook_HurtPlayer), the scene frame hook (CloneA2_FrameHook); then its sprite definitions,
 * the palette flicker, and the players at the start (Players_PlaceAtStart, all arguments 0). */
#include "xmen2.h"

void CloneA2_InstallHooks(void)
{
	g_SegTypeHooks[0x1A] = (SegHookFn)SegHook_HurtPlayer;
	g_SceneFrameHook = CloneA2_FrameHook;
	CloneA2_LoadResources();
	CloneA2_FlickerInit();
	Players_PlaceAtStart(0, 0, 0, 0);
}
