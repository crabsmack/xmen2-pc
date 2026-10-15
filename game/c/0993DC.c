/* XClone_UnlockArena - the clone that locked its arena unlocks it (XClone_Kill): the players'
 * flag $200000 cleared, spawning on again, the camera back to normal (no post-update hook,
 * XClone_CamSetNormal), the arena walls open (clone flags bit 0), no arena focus, and the
 * clone-maker buffer size recomputed.
 *
 * self: the clone; nothing happens unless it locked the arena. */
#include "xmen2.h"

#define g_CloneArenaFocus	(*(PTR(Actor) *)g_SceneVar3B30)	/* scene variable: object the arena camera follows */
#define g_CloneFlags		(*(s16 *)g_SceneVar3B08)	/* scene variable: 1 arena locked, 2 reward given, $100 << type killed by its own character */
#define g_CloneBufSize		(*(s16 *)g_SceneVar3B04)	/* scene variable: size of the clone-maker buffer */

void XClone_UnlockArena(XClone *self)
{
	if (self->lockedArena) {
		self->lockedArena = 0;
		if (IS_ALIVE(&g_Player1))
			g_Player1.playerFlags &= ~0x200000;
		if (IS_ALIVE(&g_Player2))
			g_Player2.playerFlags &= ~0x200000;
		g_SpawnEnabled = 1;
		g_Camera->postHook = 0;
		XClone_CamSetNormal(g_Camera);
		g_CloneFlags &= ~1;
		g_CloneArenaFocus = 0;
		g_CloneBufSize = XClone_MakerBufSize();
	}
}
