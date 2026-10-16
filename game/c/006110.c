/* Player1_SetGambit - make player 1 Gambit (Player_SetCharacter with Tbl_Gambit_vtable). */
#include "xmen2.h"

void Player1_SetGambit(void)
{
	Player_SetCharacter(CLASS(Tbl_Gambit_vtable), &g_Player1);
}
