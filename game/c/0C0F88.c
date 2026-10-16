/* Water_DrawSplash - draw a player's water splash: frame timer / 4 of g_WaterSplashSpr at (x, y).
 *
 * player: the player splashing (unused).  timer: frames left of the splash ($23 down to 0).
 * x, y: the splash position (world pixels). Caller: Water_DrawHook.
 * MPW: the ROM's frame looks like code compiled out (if (0)): a 2-byte stack local, timer kept
 * apart from frame (d5 / d4), the registers ranked x, y, timer, and the last call's arguments
 * popped before the return (a label after it). The unused local t, the dead uses and the label
 * reproduce that. */
#include "xmen2.h"

void Water_DrawSplash(XPlayer *player, register short timer, register short x, register short y)
{
	short t;
	register short frame;

	if (0) t = 0;
	frame = timer >> 2;
	Spr_SetFrame(g_WaterSplashSpr, frame);
	x -= g_CameraX;
	y -= g_CameraY;
	Spr_Draw(g_WaterSplashSpr, x, y);
end:
	if (0) t = timer + x + y + x;
}
