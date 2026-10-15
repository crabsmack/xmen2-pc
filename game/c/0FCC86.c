/* SiberiaB1_PostLoad - the Blizzard's post-load hook (g_ScenePostLoadHook, run last by
 * Level_Load): the wind sprite, the stream row 256 px further, normal-width streaming, one run
 * of the map row hook with interrupts off, then the standard Scene_PostLoadVram. */
#include "xmen2.h"

void SiberiaB1_PostLoad(void)
{
	SiberiaB1_InitWindSprite();
	g_FgStreamY += 0x100;
	g_MapStreamNarrow = 0;
	Irq_Disable();
	(*g_MapRowHook)();
	Irq_Enable();
	Scene_PostLoadVram();
}
