/* SavageA2_InitFlood - start the Perimeter's flood (SavageA2/A3) just below the lowest camera view
 * (g_CamMaxY + $E0): the water layer (Water_Init type 3, its surface and splash line 1 px below)
 * and its raster hook (g_SceneDrawHook = Water_DrawHook); SavageA2_CamUpdateFlood raises it. */
#include "xmen2.h"

void SavageA2_InitFlood(void)
{
	g_FloodY88 = (g_CamMaxY + 0xE0) << 8;
	g_FloodLineY = g_FloodY88 >> 8;
	Water_Init(3, g_FloodLineY + 1, g_FloodLineY + 1);
	g_SceneDrawHook = FN_DATA(Water_DrawHook);
}
