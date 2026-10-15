/* XClone_FrameHookCollapse - the scene hook after the last clone (XClone_EndWave): the building
 * comes down. Each frame counts the frame and the reward timer (down to 0) and two random timers;
 * when the blast timer runs out (reloaded with $1E + 3 x 0..15 frames) a chain explosion
 * (Tbl_XCloneImpactChain on a plain ACTOR, harmless) goes off near the top of the view, $20-$5F px
 * from its left edge (camera centre x below $61E) or $E1-$120 px from it (above 1), with a
 * screen shake (Tbl_XCloneCollapseQuake); when the debris timer runs out (reloaded with $3C +
 * 0..31) 10 debris pieces fall from the top, 0..127 px from the left edge. The timers run twice
 * as fast in two-player step mode.
 *
 * Never runs in the corpus. With the view centre at x <= 1 the blast's x would be the frame
 * step: the original kept both (and the debris y) in one variable, n. */
#include "xmen2.h"

#define g_CloneRewardTimer	(*(s16 *)g_SceneVar3B06)	/* scene variable: frames before the next reward */
#define g_CollapseDebrisTimer	(*(s16 *)g_SceneVar3B36)	/* scene variable: frames before the next falling debris */
#define g_CollapseBlastTimer	(*(s16 *)g_SceneVar3B38)	/* scene variable: frames before the next blast */
#define g_CloneDebris		(*(PTR(DebrisPool) *)g_SceneVar3B1C)	/* scene variable: the clone-maker debris pool */

void XClone_FrameHookCollapse(void)
{
	short n = g_TwoPlayerStep ? 2 : 1;
	short cx, cy;
	Actor *fx;

	g_FrameCounter += n;
	if (g_CloneRewardTimer > 0)
		g_CloneRewardTimer -= n;
	if (g_CloneRewardTimer < 0)
		g_CloneRewardTimer = 0;
	if (g_CollapseDebrisTimer > 0)
		g_CollapseDebrisTimer -= n;
	if (g_CollapseBlastTimer > 0)
		g_CollapseBlastTimer -= n;
	if (g_CollapseBlastTimer <= 0) {
		g_CollapseBlastTimer = ((unsigned short)Rand_Next() & 0xF) + 0x1E
			+ ((unsigned short)Rand_Next() & 0xF) + ((unsigned short)Rand_Next() & 0xF);
		cx = g_CameraX + 0xA0;
		cy = g_CameraY + 0xC0;
		if (cx < 0x61E)
			n = g_CameraX + 0x20 + ((unsigned short)Rand_Next() & 0x3F);
		else if (cx > 1)
			n = g_CameraX + 0x120 - ((unsigned short)Rand_Next() & 0x3F);
		fx = (Actor *)(*CLASS(Tbl_Actor_vtable)->method[2])(0,		/* New */
			CLASS(Tbl_Actor_vtable), n << 8, cy << 8, 0);
		if (fx)
			ExplodeFx_ChainRepeat(fx, CODEPTR(Tbl_XCloneImpactChain), 0, -0x10, -0x20, 0);
		g_CamShake = CODEPTR(Tbl_XCloneCollapseQuake);
	}
	if (g_CollapseDebrisTimer <= 0) {
		g_CollapseDebrisTimer = ((unsigned short)Rand_Next() & 0x1F) + 0x3C;
		cx = g_CameraX + ((unsigned short)Rand_Next() & 0x7F);
		n = g_CameraY;
		Debris_Spawn(cx, n, g_CloneDebris, 10);
	}
}
