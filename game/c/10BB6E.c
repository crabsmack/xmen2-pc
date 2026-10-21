/* SpaceA1_RunScene - the scene "SpaceA1 Space_Elevator" (Scene_RegisterAll): the lift (an ELEVATOR,
 * g_Elevator) and DEATHBIRD's burst record (g_SpaceA1Buf) live in this function's frame, cleared;
 * then the empty stub, load the level, set it up (SpaceA1_InstallHooks), shadow/highlight on, the
 * gauss bullet pool, the level loop (Level_Run) until the scene ends, and shadow/highlight off.
 * PC: the lift and the record must be game memory (generated code reaches them through the
 * globals): they are placed on the 68000 stack where the original's frame had them (-$BC(a6) and
 * -$A8(a6), a6 = A7 at entry - 4; no registers saved). */
#include "xmen2.h"

void SpaceA1_RunScene(void)
{
#ifdef XMEN2_MPW
	Elevator elevator;
	DeathbirdBurst burst;
#define ELEV	(&elevator)
#define BURST	(&burst)
#else
	unsigned long a7 = xm::frame_mark();
	DeathbirdBurst *burst = AT68(DeathbirdBurst, xm::frame_alloc(4 + sizeof(DeathbirdBurst)));
	Elevator *elevator = AT68(Elevator, xm::frame_alloc(sizeof(Elevator)));
#define ELEV	elevator
#define BURST	burst
#endif

	g_Elevator = ELEV;
	Mem_Set(ELEV, 0, sizeof(Elevator));
	g_SpaceA1Buf = BURST;
	Mem_Set(BURST, 0, sizeof(DeathbirdBurst));
	g_Elevator->cls = CLASS(Tbl_Elevator_vtable);
	SpaceA1_CallEmptyStub();
	SpaceA1_LoadLevel();
	SpaceA1_InstallHooks();
	VDP_SetShadowHighlight(1);
	GaussBullet_InitPool();
	Level_Run();
	VDP_SetShadowHighlight(0);
#ifndef XMEN2_MPW
	xm::frame_release(a7);
#endif
}
