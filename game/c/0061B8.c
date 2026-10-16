/* Player2_SetCyclops - make player 2 Cyclops (Player_SetCharacter with Tbl_Cyclops_vtable). */
#include "xmen2.h"

void Player2_SetCyclops(void)
{
	Player_SetCharacter(CLASS(Tbl_Cyclops_vtable), &g_Player2);
}
