/* SiberiaB1_PreLoad - the Blizzard's pre-load hook (g_ScenePreLoadHook, run first by
 * Level_Load): normal-width map streaming, a narrow plane B, the opening on (g_SiberiaIntro);
 * then the standard Scene_PreLoadReset. */
#include "xmen2.h"

void SiberiaB1_PreLoad(void)
{
	g_MapStreamNarrow = 0;
	g_BgNarrow = 1;
	g_SiberiaIntro = 1;
	Scene_PreLoadReset();
}
