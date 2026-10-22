/* Player2_StartLife - give player 2 a new life: its character kept, or one picked at random (not
 * player 1's) when it has none; health $700; one life spent. The mirror of Player1_StartLife.
 *
 * Only while g_SiberiaIntro is set (else nothing, 0). The character index is
 * Player_CharIndexOf's (g_P2CharIndex keeps it); an index past 5 (Magneto) makes Wolverine.
 * Returns 1. */
#include "xmen2.h"

char Player2_StartLife(void)
{
	short me, other;

	other = Player_CharIndexOf(g_Player1.cls);
	me = Player_CharIndexOf(g_Player2.cls);
	if (!g_SiberiaIntro)
		return 0;
	if (me == -1) {
		do
			me = (unsigned short)Rand_Next() % 6;
		while (other == me);
	}
	g_P2Health = 0x700;
	switch (me) {
	case 0:				/* Player2_StartLife_Beast (cases 0-3) */
		Player2_SetBeast();
		break;
	case 1:
		Player2_SetCyclops();
		break;
	case 2:
		Player2_SetGambit();
		break;
	case 3:
		Player2_SetNightcrawler();
		break;
	case 4:				/* Player2_StartLife_Psylocke */
		Player2_SetPsylocke();
		break;
	case 5:				/* Player2_StartLife_Wolverine */
	default:
		Player2_SetWolverine();
		break;
	}
	g_P2CharIndex = me;
	LOSE_LIFE(g_P2Lives);
	return 1;
}
