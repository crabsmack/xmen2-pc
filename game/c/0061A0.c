/* Player2_SetGambit - make player 2 Gambit (Player_SetCharacter with Tbl_Gambit_vtable). */
#include "xmen2.h"

void Player2_SetGambit(void)
{
	Player_SetCharacter(CLASS(Tbl_Gambit_vtable), &g_Player2);
}
