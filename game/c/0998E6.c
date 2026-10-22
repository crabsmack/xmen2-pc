/* XClone_CamArenaUpdate - the camera's tracking function while a clone arena is locked
 * (XClone_CamSetArenaMode): keep the live players and the arena focus object (the clone or its
 * balls form) in view, inside the arena camera limits.
 *
 * cam: the camera (g_Camera).
 * The players' extent (both, the live one, or with none alive the focus object; nothing at all
 * without one) is widened in x by the focus. x: unless locked (camFlags 1), the centre of that
 * range is kept inside the dead zone; when the range is wider than $E0 px the camera centres
 * $70 px inside the side away from the focus instead. y: unless locked (camFlags 2), the
 * extent's bottom and top are kept inside the dead zone. Then x and y are clamped to the arena
 * camera limits, 6 px inside.
 * With no focus and the range wider than $E0 the side is chosen by the larger player y instead
 * of the focus x: the original kept both in one register. The stores to focusX/focusY are never
 * read (the original's).
 * MPW: the `+ 0` on the focus position (a zero offset written out, as a macro with an offset
 * argument leaves it) computes it in d0 before the chain of assignments, as the ROM does;
 * without it MPW computes straight into x1's register. */
#include "xmen2.h"

#define g_CloneArenaFocus	g_SceneVar3B30	/* scene variable: object the arena camera follows */
#define g_CloneCamMinX		(*(s16 *)g_SceneVar3B0A)	/* scene variables: arena camera limits */
#define g_CloneCamMinY		(*(s16 *)g_SceneVar3B0C)
#define g_CloneCamMaxX		(*(s16 *)g_SceneVar3B0E)
#define g_CloneCamMaxY		(*(s16 *)g_SceneVar3B10)

void XClone_CamArenaUpdate(Camera *cam)
{
	short focusX, focusY;
	short deadLeft, deadRight, deadTop, deadBottom;
	short lo, hi, top, bottom;
	short x1, y1, x2, y2;
	short fx, fy, cx;
	short minY, minX, maxX, maxY;

	deadLeft = cam->deadLeft;
	deadRight = cam->deadRight;
	deadTop = cam->deadTop;
	deadBottom = cam->deadBottom;
#if defined(XMEN2_NATIVE)
	if (g_opt.players > 2 && P34_Extents(&x1, &x2, &y1, &y2))	/* phase 7: the group in play */
		;
	else
#endif
	if (IS_ALIVE(&g_Player1) && IS_ALIVE(&g_Player2)) {
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
		if (!g_CloneArenaFocus)
			return;
		focusX = x2 = x1 = (g_CloneArenaFocus->x >> 8) + 0;
		focusY = y2 = y1 = (g_CloneArenaFocus->y >> 8) + 0;
	}
	top = y1;
	bottom = y2;
	if (g_CloneArenaFocus) {
		fx = g_CloneArenaFocus->x >> 8;
		fy = g_CloneArenaFocus->y >> 8;
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
	minX = g_CloneCamMinX + 6;
	minY = g_CloneCamMinY + 6;
	maxX = g_CloneCamMaxX - 6;
	maxY = g_CloneCamMaxY - 6;
	if (cam->x > maxX)
		cam->x = maxX;
	if (cam->x < minX)
		cam->x = minX;
	if (cam->y > maxY)
		cam->y = maxY;
	if (cam->y < minY)
		cam->y = minY;
}
