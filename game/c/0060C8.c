/* Player1_SetWolverine - make player 1 Wolverine (Player_SetCharacter with Tbl_Wolverine_vtable). */
#include "xmen2.h"

void Player1_SetWolverine(void)
{
	Player_SetCharacter(CLASS(Tbl_Wolverine_vtable), &g_Player1);
}
