/* Player1_SetCyclops - make player 1 Cyclops (Player_SetCharacter with Tbl_Cyclops_vtable). */
#include "xmen2.h"

void Player1_SetCyclops(void)
{
	Player_SetCharacter(CLASS(Tbl_Cyclops_vtable), &g_Player1);
}
