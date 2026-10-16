/* SavageA4_RunScene - the scene 'SavageA4 Chamber': the stats record the Chamber's enemies share
 * (8 bytes in this frame, SavageA4_InitBossStages), load the level, the hooks and resources, the
 * parallax (Tbl_SavageA4Parallax), no camera focus yet, the camera (CAMERA's New, Cam_New, with
 * SavageA4_CamSetHooks as its post-update hook) in g_Camera, the gauss bullet pool, a BIOBALLS
 * list of 16, sounds stopped, then the level loop (Level_Run) until the scene ends, and all
 * sounds stopped.
 * PC: the stats record must be game memory (the enemies reach it through the scene variable): it
 * is placed on the 68000 stack where the original's frame had it (-8(a6), a6 = A7 at entry - 4;
 * no registers saved below it). */
#include "xmen2.h"

#define g_A4Stats	(*(PTR(NpcStats) *)g_SceneVar3B14)	/* scene variable: the stats record of the Chamber's enemies */
#define g_A4CamFocus	(*(PTR(Actor) *)g_SceneVar3B20)	/* scene variable: the enemy the camera also keeps in view (0: none) */

void SavageA4_RunScene(void)
{
#ifdef XMEN2_MPW
	NpcStats stats;

	g_A4Stats = &stats;
#else
	unsigned long a7 = CPU_GetSP();

	g_A4Stats = AT68(NpcStats, xm::sp_reserve(4 + 8));
#endif
	SavageA4_InitBossStages();
	SavageA4_CallEmptyStub();
	SavageA4_LoadLevel();
	SavageA4_InstallHooks();
	SavageA4_CallEmptyStub();
	Scroll_SetParallax(CODEPTR(Tbl_SavageA4Parallax));
	g_A4CamFocus = 0;
	g_Camera = (Camera *)(*CLASS(Tbl_Camera_vtable)->method[1])(CLASS(Tbl_Camera_vtable),
		SavageA4_CamSetHooks);	/* Cam_New */
	GaussBullet_InitPool();
	BioBalls_AllocBallList(0x10);
	Snd_StopAll();
	Level_Run();
	Snd_StopAll();
#ifndef XMEN2_MPW
	xm::sp_restore(a7);
#endif
}
