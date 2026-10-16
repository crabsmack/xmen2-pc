/* Water_DrawHook - the scene draw hook of stages with hazard water (g_SceneDrawHook): the players'
 * splashes and the water's damage, then the draw list (DrawList_DrawAll).
 *
 * A live player below the surface (g_WaterSurfaceY) moving up or down, with no splash running,
 * starts a $24-frame splash at its x and g_WaterSplashY; a running splash counts down and is
 * drawn (Water_DrawSplash). While g_WaterHurts, a live player with health, below the surface and
 * not flashing from a hit takes a hit (TakeDamage, type 7, damage 4, from itself) and flashes 5
 * frames. */
#include "xmen2.h"

#define UNDER_WATER(p)	((long)g_WaterSurfaceY << 8 < (p).y)

void Water_DrawHook(void)
{
	if (!g_P1SplashTimer && IS_ALIVE(&g_Player1)
	    && (UNDER_WATER(g_Player1) && g_Player1.vy > 0 || UNDER_WATER(g_Player1) && g_Player1.vy < 0)) {
		g_P1SplashX = g_Player1.x >> 8;
		g_P1SplashY = g_WaterSplashY;
		g_P1SplashTimer = 0x24;
	}
	if (!g_P2SplashTimer && IS_ALIVE(&g_Player2)
	    && (UNDER_WATER(g_Player2) && g_Player2.vy > 0 || UNDER_WATER(g_Player2) && g_Player2.vy < 0)) {
		g_P2SplashX = g_Player2.x >> 8;
		g_P2SplashY = g_WaterSplashY;
		g_P2SplashTimer = 0x24;
	}
	if (g_P1SplashTimer) {
		--g_P1SplashTimer;
		Water_DrawSplash(&g_Player1, g_P1SplashTimer, g_P1SplashX, g_P1SplashY);
	}
	if (g_P2SplashTimer) {
		--g_P2SplashTimer;
		Water_DrawSplash(&g_Player2, g_P2SplashTimer, g_P2SplashX, g_P2SplashY);
	}
	if (IS_ALIVE(&g_Player1) && g_Player1.health > 0 && g_Player1.hitCooldown <= 0
	    && UNDER_WATER(g_Player1) && g_WaterHurts) {
		(*g_Player1.cls->method[8])(&g_Player1, &g_Player1, 7, 4, -1, -1);	/* +$26 TakeDamage */
		g_Player1.hitCooldown = 5;
	}
	if (IS_ALIVE(&g_Player2) && g_Player2.health > 0 && g_Player2.hitCooldown <= 0
	    && UNDER_WATER(g_Player2) && g_WaterHurts) {
		(*g_Player2.cls->method[8])(&g_Player2, &g_Player2, 7, 4, -1, -1);	/* +$26 TakeDamage */
		g_Player2.hitCooldown = 5;
	}
	DrawList_DrawAll();
}
