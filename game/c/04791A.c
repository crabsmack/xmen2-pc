/* BaniMazaA2_RunScene - the scene 'BaniMazaA2 ApocalypseBoss': load the level, place the players,
 * make the camera, restart the palette animation, the scene sprite, the debris (four groups in
 * this frame), a 10-frame timer that then makes BaniMazaA2_AnimSprite the V-blank hook
 * (Timer_SetVBlankHookCb), then run the level loop (Level_Run) until the scene ends.
 * PC: the debris groups must be game memory (other functions reach them through the scene
 * variable): they are placed on the 68000 stack where the original's frame had them (-$218(a6),
 * a6 = A7 at entry - 4), with the frame's 8 more bytes (the saved a3/a4) reserved below them for
 * the calls made from here. */
#include "xmen2.h"

void BaniMazaA2_RunScene(void)
{
#ifdef XMEN2_MPW
	DebrisGroup groups[4];
	register DebrisGroup *debris = groups;
	register Timer *t;
#else
	unsigned long a7 = CPU_GetSP();
	DebrisGroup *debris = AT68(DebrisGroup, xm::sp_reserve(4 + 0x218));
	Timer *t;

	xm::sp_reserve(8);
#endif

	BaniMazaA2_CallEmptyStub();
	BaniMazaA2_LoadLevel();
	BaniMazaA2_PlacePlayers();
	BaniMazaA2_InitCamera();
	BaniMazaA2_ResetCounters();
	BaniMazaA2_InitSprite();
	if (1)	/* (a block: MPW pops the call's argument at its end, as the ROM does) */
		BaniMazaA2_InitDebrisBuf(debris);
	if (t = Timer_Create((TimerFn)Timer_SetVBlankHookCb, 10, FN_DATA(BaniMazaA2_AnimSprite), 1))
		Timer_Start(t);
	Level_Run();
#ifndef XMEN2_MPW
	xm::sp_restore(a7);
#endif
}
