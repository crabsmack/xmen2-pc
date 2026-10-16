/* Water_Init - set up a stage's hazard water: the splash sprite set Art_Spr_0D0DCA (in palette
 * `palette`, g_WaterSplashSpr), the surface and splash heights, no splashes running, and water
 * that hurts (g_WaterHurts).
 *
 * palette: the splash sprite's palette line.  surfaceY: the water surface (world pixels;
 * players below it splash and take damage, Water_DrawHook).  splashY: where splashes are drawn.
 * Callers: SavageC1_RunScene, SavageB1_RunScene, SavageA2_InitFlood. */
#include "xmen2.h"

void Water_Init(short palette, short surfaceY, short splashY)
{
	register long n;

	g_WaterSplashSpr = &g_SpriteDefs[n = SprDef_Add(CODEPTR(Art_Spr_0D0DCA), palette)];
	g_WaterSurfaceY = surfaceY;
	g_WaterSplashY = splashY;
	g_P2SplashTimer = 0;
	g_P1SplashTimer = 0;
	g_WaterHurts = 1;
}
