/* Players_PlaceAtStart - put the players still in play at the stage's start (the stage loaders):
 * player 1 at (x, y), player 2 30 px right of it, each constructed there (Init, slot +$12) with
 * the health it carried (at least $700) and activated (slot +$3E); a player with no health carried
 * is left out (flags and health 0). The HUD is redrawn.
 *
 * x, y:   start position in pixels; a start record set by the level (g_P1StartRec) replaces it
 *         (and is used up).
 * x2, y2: a second position, set from g_P2StartRec's turn (but from g_P1StartRec: a bug, and
 *         with both records set that one is gone by then and 0 is read through) and never used.
 * Whether each player had a character, and a "players hit each other" flag that calls
 * Player2_MakeHostile, are kept but never set or read: code that was compiled out. */
#include "xmen2.h"

void Players_PlaceAtStart(register long x, register long y, long x2, long y2)
{
	char p1In;
	register char p2In;
	register char hostile;

	p1In = 0;
	p2In = 0;
	hostile = 0;
	if (g_P1CharIndex != -1)
		p1In = 1;
	if (g_P2CharIndex != -1)
		p2In = 1;
	if (g_P1StartRec) {
		x = g_P1StartRec->x;
		y = g_P1StartRec->y;
		g_P1StartRec = 0;
	}
	if (g_P2StartRec) {
#ifdef XMEN2_MPW
		x2 = g_P1StartRec->x;
		y2 = g_P1StartRec->y;
#else
		/* g_P1StartRec is 0 here when both were set: the 68000 reads the vector table */
		x2 = (g_P1StartRec ? (Placement *)g_P1StartRec : (Placement *)Vectors)->x;
		y2 = (g_P1StartRec ? (Placement *)g_P1StartRec : (Placement *)Vectors)->y;
#endif
		g_P2StartRec = 0;
	}
	if (g_P1Health) {
		(*g_Player1.cls->method[3])(&g_Player1, x << 8, y << 8, 0);	/* +$12 Init */
		g_Player1.health = MAX(0x700, g_P1Health);
		(*g_Player1.cls->method[14])(&g_Player1);			/* +$3E Activate */
		g_Player1.pad = 0;
	} else {
		g_Player1.flags = 0;
		g_Player1.health = 0;
	}
	if (g_P2Health) {
		(*g_Player2.cls->method[3])(&g_Player2, x + 30 << 8, y << 8, 0);	/* +$12 Init */
		g_Player2.health = MAX(0x700, g_P2Health);
		(*g_Player2.cls->method[14])(&g_Player2);			/* +$3E Activate */
		g_Player2.pad = 1;
	} else {
		g_Player2.flags = 0;
		g_Player2.health = 0;
	}
#if defined(XMEN2_NATIVE)
	if (g_opt.players > 2) {	/* phase 7: players 3 and 4 the same, 60 and 90 px right of player 1 */
		register short k;
		register XPlayer *p;

		for (k = 3; k <= 2 + P34_COUNT; k++) {
			p = PLAYER34(k);
			if (g_PX[k - 3].health) {
				P34_Enter(k);
				(*p->cls->method[3])(p, x + 30 * (k - 1) << 8, y << 8, 0);	/* +$12 Init */
				p->health = MAX(0x700, g_P2Health);	/* player k's, in player 2's place */
				(*p->cls->method[14])(p);					/* +$3E Activate */
				p->pad = 1;
				P34_Leave(k);
			} else {
				p->flags = 0;
				p->health = 0;
			}
		}
	}
#endif
	g_HudDirty = 1;
	if (hostile)
		Player2_MakeHostile();
}
