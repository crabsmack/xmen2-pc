/* Player2_SetBeast - make player 2 Beast (Player_SetCharacter with Tbl_Beast_vtable). */
#include "xmen2.h"

void Player2_SetBeast(void)
{
	Player_SetCharacter(CLASS(Tbl_Beast_vtable), &g_Player2);
}
