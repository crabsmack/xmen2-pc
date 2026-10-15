/* XClone_LockArena - a clone locks its arena: it is the arena camera's focus unless one is set,
 * the live players are flagged (playerFlags $200000), the camera is held inside the arena walls
 * (limits no wider than the current ones; XClone_CamSetArenaMode as its post-update hook), all
 * sounds stop and the clone's theme music parameter is set (Snd_SetParam $12; 1-7 by character),
 * sound $74; the arena walls become solid (clone flags bit 0) and spawning stops.
 *
 * self: the clone (XClone_StateWaitPlayers, when the players are inside).
 * Themes: Beast 1, Nightcrawler 2, Magneto 3, Gambit 4, Wolverine 5, Cyclops 6, Psylocke 7. For
 * another type the theme is whatever the register held (the camera's maximum y). */
#include "xmen2.h"

#define g_CloneArenaFocus	(*(PTR(Actor) *)g_SceneVar3B30)	/* scene variable: object the arena camera follows */
#define g_CloneWallLeft		(*(s16 *)g_SceneVar3B2C)	/* scene variable: x of the arena's left wall */
#define g_CloneWallRight	(*(s16 *)g_SceneVar3B2A)	/* scene variable: x of the arena's right wall */
#define g_CloneCamMinX		(*(s16 *)g_SceneVar3B0A)	/* scene variables: arena camera limits */
#define g_CloneCamMinY		(*(s16 *)g_SceneVar3B0C)
#define g_CloneCamMaxX		(*(s16 *)g_SceneVar3B0E)
#define g_CloneCamMaxY		(*(s16 *)g_SceneVar3B10)
#define g_CloneFlags		(*(s16 *)g_SceneVar3B08)	/* scene variable: 1 arena locked, 2 reward given, $100 << type killed by its own character */

void XClone_LockArena(XClone *self)
{
	short minX, minY, maxX, maxY;
	short theme;

	self->flags6 |= 0x1000;
	if (IS_ALIVE(&g_Player1))
		g_Player1.playerFlags |= 0x200000;
	if (IS_ALIVE(&g_Player2))
		g_Player2.playerFlags |= 0x200000;
	if (!g_CloneArenaFocus)
		g_CloneArenaFocus = (Actor *)self;
	minX = MAX(g_CamMinX, g_CloneWallLeft);
	minY = MAX(g_CamMinY, self->arenaTop - 0x20);
	maxX = MIN(g_CamMaxX, g_CloneWallRight - 0x140);
	maxY = MIN(g_CamMaxY, self->arenaFloor + -0xC0);
	g_CloneCamMinX = minX;
	g_CloneCamMinY = minY;
	g_CloneCamMaxX = maxX;
	g_CloneCamMaxY = maxY;
	g_Camera->postHook = XClone_CamSetArenaMode;
	g_CamModeSwitched = 1;
	Snd_StopAll();
#ifndef XMEN2_MPW
	theme = maxY;		/* MPW C keeps theme in maxY's register: another type passes that on */
#endif
	switch (Clone_CharTypeOf(self)) {
	case 1:			/* XClone_LockArena_Case1 */
		theme = 1;
		break;
	case 5:
		theme = 2;
		break;
	case 4:			/* XClone_LockArena_Case4 */
		theme = 3;
		break;
	case 3:			/* XClone_LockArena_Case3 */
		theme = 4;
		break;
	case 7:
		theme = 5;
		break;
	case 2:
		theme = 6;
		break;
	case 6:
		theme = 7;
		break;
	}
	/* XClone_LockArena_SetTheme */
	Snd_SetParam(0x12, theme);
	Snd_Stop(0x74);
	Snd_Play(0x74);
	XClone_CamSetArenaMode(g_Camera);
	g_CloneFlags |= 1;
	g_SpawnEnabled = 0;
	self->lockedArena = 1;
}
