/* Player1_SetPsylocke - make player 1 Psylocke (Player_SetCharacter with Tbl_Psylocke_vtable). */
#include "xmen2.h"

void Player1_SetPsylocke(void)
{
	Player_SetCharacter(CLASS(Tbl_Psylocke_vtable), &g_Player1);
}
