/* Player1_SetNightcrawler - make player 1 Nightcrawler (Player_SetCharacter with Tbl_Nightcrawler_vtable). */
#include "xmen2.h"

void Player1_SetNightcrawler(void)
{
	Player_SetCharacter(CLASS(Tbl_Nightcrawler_vtable), &g_Player1);
}
