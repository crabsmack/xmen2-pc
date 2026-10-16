/* Player1_SetBeast - make player 1 Beast (Player_SetCharacter with Tbl_Beast_vtable). */
#include "xmen2.h"

void Player1_SetBeast(void)
{
	Player_SetCharacter(CLASS(Tbl_Beast_vtable), &g_Player1);
}
