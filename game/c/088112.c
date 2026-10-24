/* CloneA5_CamApply - the boss arena camera's apply (CAMERA +$0C, CloneA5_CamSetHooks): the camera is
 * pinned to the arena position (g_BroodCamX/Y, BroodBoss_Init) whatever the camera says, then
 * shaken by g_CamShake; plane B follows the boss offsets (g_BroodBossOffsetX/Y as its scroll
 * bases), plane A scrolls on by the claw's step (g_BroodScrollStep, then cleared); the scroll
 * factors give both planes' y and plane B's x; with g_CloneA5CamFlags bit 0, plane A wraps $3C0
 * px forward once it falls behind the arena's left edge (g_BroodArenaLeft). Then the map
 * streaming hooks run with interrupts off and both planes are marked for the next V-blank.
 *
 * cam: the camera.
 * The shake script is pairs of signed bytes (dx, dy) added to the camera; $64 ends it, $65 n goes
 * back n bytes, $66 x calls Sys_Nop1F945C(x, 0). It loads the old camera position into a register
 * it never uses (kept). */
#include "xmen2.h"

#define g_Shake			(*(PTR(s8) *)&g_CamShake)	/* g_CamShake read as its bytes */
#define g_BroodScrollStep	(*(s16 *)g_SceneVar3B02)	/* scene variable (CloneA5): pixels the arena scrolls this frame (BroodClaw_Swing) */
#define g_BroodArenaLeft	(*(s32 *)&g_SceneVar3B20)	/* scene variable: scroll x the claw swings the arena to (CloneA5_CamUpdate, BroodClaw_Swing) */
#define g_BroodCamY		(*(s16 *)g_SceneVar3B22)	/* scene variable: camera y in the arena (BroodBoss_Init) */
#define g_BroodCamX		(*(s16 *)g_SceneVar3B24)	/* scene variable: camera x in the arena (BroodBoss_Init) */

void CloneA5_CamApply(register Camera *cam)
{
	register short old;
	register short x;
	register short y;

	x = cam->x;
	y = cam->y;
	old = g_CameraX;
	old = g_CameraY;
	g_CameraX = x;
	g_CameraY = y;
	if (g_CameraX != g_BroodCamX)
		g_CameraX = g_BroodCamX;
	if (g_CameraY != g_BroodCamY)
		g_CameraY = g_BroodCamY;
	if (g_Shake) {
		if (*g_Shake == 0x64)
			g_Shake = 0;
		else if (*g_Shake == 0x65)
			g_Shake -= g_Shake[1];
		else if (*g_Shake == 0x66) {
			g_Shake++;
			Sys_Nop1F945C(*g_Shake++, 0);
		} else {
			g_CameraX += *g_Shake++;
			g_CameraY += *g_Shake++;
		}
	}
	g_BgScrollBaseX = g_BroodBossOffsetX >> 8;
	g_BgScrollBaseY = g_BroodBossOffsetY >> 8;
	g_FgScrollX += g_BroodScrollStep;
	g_FgScrollY = g_FgScrollBaseY + (g_CameraY * g_FgScrollFactorY >> 8);
	g_BgScrollX = g_BgScrollBaseX + (g_CameraX * g_BgScrollFactorX >> 8);
	g_BgScrollY = g_BgScrollBaseY + (g_CameraY * g_BgScrollFactorY >> 8);
	g_BroodScrollStep = 0;
	if (g_CloneA5CamFlags & 1) {
		if (g_FgScrollX < g_BroodArenaLeft) {
			g_FgScrollX += 0x3C0;
			g_FgStreamX += 0x3C0;
			g_FgScrollAdjX -= 0x3C0;
		}
	}
	Irq_Disable();
	(*g_MapColHook)();
	(*g_MapRowHook)();
	Irq_Enable();
	g_FgScrollDirty = 1;
	g_BgScrollDirty = 1;
}
