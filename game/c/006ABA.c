/* Game_InitButtonMasks - the pad button masks (the 6-button pad's bits: Up 1, Down 2, Left 4,
 * Right 8, B $10, C $20, A $40, Z $100, Y $200, X $400, Mode $800, and the Left|Right / Up|Down
 * pairs), the VRAM tiles a player's sprite needs (Player_CalcMaxTiles) and the random seed $1A3. */
#include "xmen2.h"

void Game_InitButtonMasks(void)
{
	g_PadMaskLeft = 4;
	g_PadMaskRight = 8;
	g_PadMaskLeftRight = g_PadMaskLeft | g_PadMaskRight;
	g_PadMaskUp = 1;
	g_PadMaskDown = 2;
	g_PadMaskUpDown = g_PadMaskUp | g_PadMaskDown;
	g_PadMaskC = 0x20;
	g_PadMaskB = 0x10;
	g_PadMaskA = 0x40;
	g_PadMaskX = 0x400;
	g_PadMaskY = 0x200;
	g_PadMaskZ = 0x100;
	g_PadMaskMode = 0x800;
	g_PlayerTiles = Player_CalcMaxTiles();
	Rand_Seed(0x1A3);
}
