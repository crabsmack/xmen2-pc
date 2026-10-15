/* BigAcolyte_ActWaitPlayers - BIGACOLYTE intro script action (step 1) and state: wait until every
 * live player is past x $1A5E (a dead one counts as past), then wall them in (a floor segment
 * of type 1 at x $1A5F, y $3AC-$401), pan the camera to x $1A3C and lock it, and go on with the
 * script in 120 frames.
 *
 * self: the boss.
 * Meanwhile a player beyond x $19C0 is moved in front (sprite priority, draw list front) and
 * one back before x $1900 behind again. Returns self->flags & 8 while waiting, nothing when
 * the script goes on. The "back" flags are set and never read. */
#include "xmen2.h"

short BigAcolyte_ActWaitPlayers(register BigAcolyte *self)
{
	char past1, past2;
	char back1, back2;
	register short px;
	register Seg *seg;

	past1 = 0;
	past2 = 0;
	back1 = 0;
	back2 = 0;
	if (IS_ALIVE(&g_Player1)) {
		px = g_Player1.x >> 8;
		if (px > 0x19C0 && !(g_Player1.spr.flags & 4)) {
			g_Player1.spr.flags |= 4;
			DrawList_SetPriority(g_Player1.drawNode, 1, g_Player1.layer, g_Player1.flags6 & 8);
		}
		if (px < 0x1900) {
			if (g_Player1.spr.flags & 4) {
				g_Player1.spr.flags &= ~4;
				DrawList_SetPriority(g_Player1.drawNode, 0, g_Player1.layer, g_Player1.flags6 & 8);
			}
			back1 = 1;
		}
		if (px > 0x1A5E)
			past1 = 1;
	} else {
		past1 = 1;
		back1 = 1;
	}
	if (IS_ALIVE(&g_Player2)) {
		px = g_Player2.x >> 8;
		if (px > 0x19C0 && !(g_Player2.spr.flags & 4)) {
			g_Player2.spr.flags |= 4;
			DrawList_SetPriority(g_Player2.drawNode, 1, g_Player2.layer, g_Player2.flags6 & 8);
		}
		if (px < 0x1900) {
			if (g_Player2.spr.flags & 4) {
				g_Player2.spr.flags &= ~4;
				DrawList_SetPriority(g_Player2.drawNode, 0, g_Player2.layer, g_Player2.flags6 & 8);
			}
			back2 = 1;
		}
		if (px > 0x1A5E)
			past2 = 1;
	} else {
		past2 = 1;
		back2 = 1;
	}
	if (past1 && past2) {
		seg = &g_Segs[Seg_Alloc()];
		Seg_Setup(seg, 1, 0, 0x1A5F, 0x3AC, 0x1A5F, 0x401);
		(*g_Camera->cls->method[12])(g_Camera, 0x1A3C, g_CameraY, 2, 2, 1, 0);	/* PanTo */
		AI_NEXT_STEP(self, self->ai, 120);
	} else {
		SET_STATE(self, BigAcolyte_ActWaitPlayers);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
