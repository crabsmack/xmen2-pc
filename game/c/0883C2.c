/* CloneA5_InitShockSprites - the boss's floor-shock sprites (g_SceneSprites 0 and 1): Art_CloneA5Spr3,
 * frame 0, palette 2, each with its tiles uploaded at g_BgTilesEnd (moved past them). */
#include "xmen2.h"

void CloneA5_InitShockSprites(void)
{
	g_BgTilesEnd = Spr_Init(&g_SceneSprites[0], 1, CODEPTR(Art_CloneA5Spr3), 0, 2, g_BgTilesEnd, 1);
	g_BgTilesEnd = Spr_Init(&g_SceneSprites[1], 1, CODEPTR(Art_CloneA5Spr3), 0, 2, g_BgTilesEnd, 1);
}
