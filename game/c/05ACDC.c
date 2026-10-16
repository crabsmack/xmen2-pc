/* BaniMazaB2_InstallFrameHook - BaniMazaB2_FrameHook becomes the scene's per-frame hook
 * (g_SceneFrameHook). */
#include "xmen2.h"

void BaniMazaB2_InstallFrameHook(void)
{
	g_SceneFrameHook = BaniMazaB2_FrameHook;
}
