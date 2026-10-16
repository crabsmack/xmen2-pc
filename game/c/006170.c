/* Player2_SetPsylocke - make player 2 Psylocke (Player_SetCharacter with Tbl_Psylocke_vtable). */
#include "xmen2.h"

void Player2_SetPsylocke(void)
{
	Player_SetCharacter(CLASS(Tbl_Psylocke_vtable), &g_Player2);
}
