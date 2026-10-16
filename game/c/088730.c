/* CloneA5_RunScene - the scene 'CloneA5 BroodBoss': the boss arena. Level_Load (FG map Lvl_CloneA5FgMap,
 * FG tiles Lvl_CloneA2Fg, BG map and tiles Lvl_CloneA5Bg, FG floor segments Lvl_CloneA5FgSegs, BG
 * segments Lvl_CloneA5BgSegs, flags 0 1 1 0, class-init argument $F, no music, grid shifts 8 and 8,
 * class list Tbl_CloneA5ClassList); every sound stops, sound parameter $13 = 0; an ICANDY pool of 10
 * objects of $48 bytes in this function's frame (the boss's wall); the HUD on; plane A scrolls in
 * y only, plane B fixed; the boss offsets at (0, $33); the palette cycle (a RAM copy of
 * Tbl_CloneA5PalCycle in this frame, health bar 5); floor-segment types $16 and $15 hooks; the
 * players at their start, each live one drawn with priority; the two debris pools ($12 pieces);
 * no spawn pass; the shock sprites, a ball list of 16, palette line 0 (Pal_CloneA2Line0, colour 0
 * = $200); the boss flags cleared; the tile hooks, the camera and the frame hook
 * (CloneA5_FrameHook, which spawns the boss); one-player stepping; then the level (Level_Run).
 * PC: the pool and the table are game memory (generated code uses them for the whole level):
 * placed on the 68000 stack where the original's frame had them (-$2D0(a6) and -$304(a6), a6 =
 * A7 at entry - 4), the saved registers (16 bytes) reserved below. */
#include "xmen2.h"

#define g_BroodBossFlags	(*(s16 *)g_SceneVar3B0C)	/* scene variable: 1 fight on, 2 floor shock, $10 lower the body, $20 resources loaded, $40 wall smashed */
#define g_BroodUnused3B0E	(*(s16 *)g_SceneVar3B0E)	/* scene variable: cleared here, read by nothing in this scene */
#define g_BroodBossDebris	(*(PTR(DebrisPool) *)g_SceneVar3B14)	/* scene variable: the boss's explosion debris pool */
#define g_BroodPalTable		(*(PTR(u8) *)g_SceneVar3B18)	/* scene variable (CloneA5): the arena's palette cycle table (a byte cursor while it is copied) */
#define g_CloneDebris		(*(PTR(DebrisPool) *)g_SceneVar3B1C)	/* scene variable: the clone-maker debris pool */

void CloneA5_RunScene(void)
{
#ifdef XMEN2_MPW
	u8 table[0x34];		/* the palette cycle table's copy */
	u8 pool[0x2D0];		/* the ICANDY pool: 10 x $48 bytes */
#else
	unsigned long a7 = CPU_GetSP();
	u8 *pool = AT68(u8, xm::sp_reserve(4 + 0x2D0));
	u8 *table = AT68(u8, xm::sp_reserve(0x34));
#endif

#ifndef XMEN2_MPW
	xm::sp_reserve(16);
#endif
	Level_Load(CODEPTR(Lvl_CloneA5FgMap), CODEPTR(Lvl_CloneA2Fg), CODEPTR(Lvl_CloneA5Bg),
		CODEPTR(Lvl_CloneA5Bg), CODEPTR(Lvl_CloneA5FgSegs), CODEPTR(Lvl_CloneA5BgSegs), 0, 1, 1, 0,
		0xF, -1, 8, 8, 0, CODEPTR(Tbl_CloneA5ClassList));
	Snd_StopAll();
	Snd_SetParam(0x13, 0);
	ICandy_PoolInit(pool, 0x48, 10);
	g_HudOn = 1;
	g_FgScrollFactorX = 0;
	g_FgScrollFactorY = 0x100;
	g_BgScrollFactorX = 0;
	g_BgScrollFactorY = 0;
	g_BroodBossOffsetX = 0;
	g_BroodBossOffsetY = 0x3300;
	g_BgScrollBaseY = 0;
	CloneA5_ResetPalCycle();
	g_BroodPalTable = table;
	g_BroodBossHealthBar = 5;
	{
		register short i = 0x33;
		register u8 *src = (u8 *)CODEPTR(Tbl_CloneA5PalCycle);

		while (i-- >= 0)
			*g_BroodPalTable++ = *src++;
	}
	g_BroodPalTable = table;
	g_SegTypeHooks[0x16] = CloneA5_SegHook16;
	g_SegTypeHooks[0x15] = CloneA5_SegHook15;
	Players_PlaceAtStart(0, 0, 0, 0);
	g_CloneDebris = Debris_CreatePool(0x12, 0, 0);
	g_BroodBossDebris = Debris_CreatePool(0x12, 0, 0);
	g_SpawnEnabled = 0;
	if (IS_ALIVE(&g_Player1)) {
		g_Player1.spr.flags |= 4;
		DrawList_SetPriority(g_Player1.drawNode, 1, g_Player1.layer, g_Player1.flags6 & 8);
	}
	if (IS_ALIVE(&g_Player2)) {
		g_Player2.spr.flags |= 4;
		DrawList_SetPriority(g_Player2.drawNode, 1, g_Player2.layer, g_Player2.flags6 & 8);
	}
	CloneA5_InitShockSprites();
	BioBalls_AllocBallList(0x10);
	g_PalBase[0] = 0x200;
	{
		register s16 *dst;
		register s16 *src = (s16 *)CODEPTR(Pal_CloneA2Line0);
		register long i;

		dst = g_PalBase;

		for (i = 0; i < 16; i++)
			*dst++ = *src++;
	}
	g_BroodBossFlags = 0;
	g_BroodUnused3B0E = 0;
	g_TileHooksOn = CloneA5_InitTileHooks();
	CloneA5_InitCamera();
	g_SceneFrameHook = CloneA5_FrameHook;
	g_TwoPlayerStep = 0;
	Level_Run();
#ifndef XMEN2_MPW
	xm::sp_restore(a7);
#endif
}
