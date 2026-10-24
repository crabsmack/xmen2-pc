/* SavageA4_CamUpdate - the Chamber camera's tracking function (SavageA4_CamSetHooks): keep the
 * live players and the camera focus (the enemy in g_A4CamFocus) in view; XClone_CamArenaUpdate
 * without the arena limits.
 *
 * cam: the camera (g_Camera).
 * The players' extent (both when g_PlayersAlive is 2, else the live one, or with none alive the
 * focus; nothing at all without one) is widened in x by the focus. x: unless locked (camFlags 1),
 * the centre of that range is kept inside the dead zone; when the range is wider than $E0 px the
 * camera centres $70 px inside the side away from the focus instead. y: unless locked (camFlags
 * 2), the extent's bottom and top are kept inside the dead zone.
 * With no focus and the range wider than $E0 the side is chosen by the larger player y instead
 * of the focus x: the original kept both in one register. The stores to focusX/focusY are never
 * read (the original's).
 * g_SceneVar3B20 is a long (other scenes keep other types there) read through its address: MPW
 * reloads it into a0 after each store, as the ROM does (declared as raw bytes, the cast pointer
 * stayed in a3). The `+ 0` computes the focus x and y in d0 before the assignment chain, as in
 * XClone_CamArenaUpdate. */
#include "xmen2.h"

#define g_A4CamFocus	(*(PTR(Actor) *)&g_SceneVar3B20)	/* scene variable: the enemy the camera also keeps in view (0: none) */

void SavageA4_CamUpdate(Camera *cam)
{
	short focusX, focusY;
	short deadLeft, deadRight, deadTop, deadBottom;
	short lo, hi, top, bottom;
	short x1, y1, x2, y2;
	short fx, fy, cx;

	deadLeft = cam->deadLeft;
	deadRight = cam->deadRight;
	deadTop = cam->deadTop;
	deadBottom = cam->deadBottom;
#if defined(XMEN2_NATIVE)
	if (g_opt.players > 2 && P34_Extents(&x1, &x2, &y1, &y2))	/* phase 7: the group in play */
		;
	else
#endif
	if (g_PlayersAlive == 2) {
		x1 = MIN(g_Player1.x, g_Player2.x) >> 8;
		y1 = MIN(g_Player1.y, g_Player2.y) >> 8;
		x2 = MAX(g_Player1.x, g_Player2.x) >> 8;
		y2 = MAX(g_Player1.y, g_Player2.y) >> 8;
	} else if (IS_ALIVE(&g_Player1)) {
		x1 = g_Player1.x >> 8;
		x2 = g_Player1.x >> 8;
		y1 = g_Player1.y >> 8;
		y2 = g_Player1.y >> 8;
	} else if (IS_ALIVE(&g_Player2)) {
		x1 = g_Player2.x >> 8;
		x2 = g_Player2.x >> 8;
		y1 = g_Player2.y >> 8;
		y2 = g_Player2.y >> 8;
	} else {
		if (!g_A4CamFocus)
			return;
		focusX = x2 = x1 = (g_A4CamFocus->x >> 8) + 0;
		focusY = y2 = y1 = (g_A4CamFocus->y >> 8) + 0;
	}
	top = y1;
	bottom = y2;
	if (g_A4CamFocus) {
		fx = g_A4CamFocus->x >> 8;
		fy = g_A4CamFocus->y >> 8;
		lo = MIN(x1, fx);
		hi = MAX(x2, fx);
	} else {
		lo = x1;
		hi = x2;
#ifndef XMEN2_MPW
		fx = y2;		/* MPW C: fx shares y2's register */
#endif
	}
	cx = (lo + hi) >> 1;
	if (hi - lo > 0xE0) {
		if ((x1 + x2) >> 1 > fx)
			cx = x2 - 0x70;
		else
			cx = x1 + 0x70;
	}
	if (!(cam->camFlags & 1))
		cam->x = cx < cam->x + deadLeft ? cx - deadLeft
			: cx > cam->x + deadRight ? cx - deadRight : cam->x;
	if (!(cam->camFlags & 2)) {
		if (bottom > cam->y + deadBottom)
			cam->y = bottom - deadBottom;
		if (top < cam->y + deadTop)
			cam->y = top - deadTop;
	}
}
