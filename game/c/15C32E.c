/* Pause_ShowText - put the PAUSE text in the first two hardware sprites (Pause_Check): two pieces
 * at screen (160, 84) and (176, 84), tiles $794 and $798, palette 1, in front. */
#include "xmen2.h"

void Pause_ShowText(void)
{
	register short x = 0xA0;
	register short y = 0x54;
	register short attr = 0x794;

	attr |= 0x2000;
	attr |= 0x8000;
	g_SpriteTable[0].y = y + 0x60;
	g_SpriteTable[0].size = 5;
	g_SpriteTable[0].attr = attr;
	g_SpriteTable[0].x = x + 0x60;
	attr = 0x798;
	attr |= 0x2000;
	attr |= 0x8000;
	g_SpriteTable[1].y = y + 0x60;
	g_SpriteTable[1].size = 0xD;
	g_SpriteTable[1].attr = attr;
	g_SpriteTable[1].x = x + 0x70;
}
