/* SavageA4_InstallHooks - the Chamber's per-frame hook (SavageA4_FrameHook), its resources, the
 * effect sprite, the palette cycles reset, and the players placed at ($140, $180)
 * (Players_PlaceAtStart(..., 100, $500)). */
#include "xmen2.h"

void SavageA4_InstallHooks(void)
{
	g_SceneFrameHook = SavageA4_FrameHook;
	SavageA4_LoadResources();
	SavageA4_InitEffectSprite();
	SavageA4_ResetPalCycles();
	Players_PlaceAtStart(0x140, 0x180, 100, 0x500);
}
