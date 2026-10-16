/* Player2_SetWolverine - make player 2 Wolverine (Player_SetCharacter with Tbl_Wolverine_vtable). */
#include "xmen2.h"

void Player2_SetWolverine(void)
{
	Player_SetCharacter(CLASS(Tbl_Wolverine_vtable), &g_Player2);
}
