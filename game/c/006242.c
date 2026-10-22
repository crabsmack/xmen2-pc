/* Player1_StartLife - give player 1 a new life: its character kept, or one picked at random (not
 * player 2's) when it has none; health $700; one life spent.
 *
 * Only while g_SiberiaIntro is set (else nothing, 0). The character index is
 * Player_CharIndexOf's (g_P1CharIndex keeps it); an index past 5 (Magneto) makes Wolverine.
 * NATIVE: the start_char option picks it instead of the random draw, Magneto (6) included.
 * Returns 1. */
#include "xmen2.h"

char Player1_StartLife(void)
{
	short me, other;

	me = Player_CharIndexOf(g_Player1.cls);
	other = Player_CharIndexOf(g_Player2.cls);
	if (!g_SiberiaIntro)
		return 0;
#if defined(XMEN2_NATIVE)
	if (me == -1 && g_opt.start_char)	/* the start_char option: that one, not at random */
		me = g_opt.start_char - 1;
#endif
	if (me == -1) {
		do
			me = (unsigned short)Rand_Next() % 6;
		while (me == other);
	}
	g_P1Health = 0x700;
	switch (me) {
#if defined(XMEN2_NATIVE)
	case 6:				/* the start_char option's Magneto (the original never picks him here) */
		Player_SetCharacter(CLASS(Tbl_Magneto_vtable), &g_Player1);
		break;
#endif
	case 0:				/* Player1_StartLife_Beast */
		Player1_SetBeast();
		break;
	case 1:
		Player1_SetCyclops();
		break;
	case 2:				/* Player1_StartLife_Gambit */
		Player1_SetGambit();
		break;
	case 3:				/* Player1_StartLife_Nightcrawler */
		Player1_SetNightcrawler();
		break;
	case 4:				/* Player1_StartLife_Psylocke */
		Player1_SetPsylocke();
		break;
	case 5:				/* Player1_StartLife_Wolverine */
	default:
		Player1_SetWolverine();
		break;
	}
	g_P1CharIndex = me;
	LOSE_LIFE(g_P1Lives);
	return 1;
}
