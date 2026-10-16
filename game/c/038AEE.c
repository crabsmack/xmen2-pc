/* BaniMazaA1_InstallFrameHook - the Moving Blocks' per-frame hook: BaniMazaA1_FrameHook. */
#include "xmen2.h"

void BaniMazaA1_InstallFrameHook(void)
{
	g_SceneFrameHook = BaniMazaA1_FrameHook;
}
