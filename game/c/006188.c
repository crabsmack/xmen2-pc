/* Player2_SetNightcrawler - make player 2 Nightcrawler (Player_SetCharacter with Tbl_Nightcrawler_vtable). */
#include "xmen2.h"

void Player2_SetNightcrawler(void)
{
	Player_SetCharacter(CLASS(Tbl_Nightcrawler_vtable), &g_Player2);
}
