/* XClone_StartMakerMode - switch the CloneA scene into clone-maker mode (EvilMaker_StateWaitKills,
 * after two kills): reset the clone counters and flags, allocate the clone-maker buffer
 * (XClone_MakerBufSize bytes, on the stack heap), install the segment hooks (type $19 arena walls:
 * XClone_SegHookArenaLocked; type $1A: CloneA5_SegHook16, always solid) and the scene hook
 * XClone_FrameHook, start the palette flicker, and load the clone resources
 * (XClone_LoadResources through Spr_ReloadDefs, re-packing VRAM). */
#include "xmen2.h"

#define g_CloneArenaFocus	g_SceneVar3B30	/* scene variable: object the arena camera follows */
#define g_CloneFlags		(*(s16 *)g_SceneVar3B08)	/* scene variable: 1 arena locked, 2 reward given, $100 << type killed by its own character */
#define g_CloneRewardTimer	(*(s16 *)g_SceneVar3B06)	/* scene variable: frames before the next reward */
#define g_CloneBufSize		(*(s16 *)g_SceneVar3B04)	/* scene variable: size of the clone-maker buffer */
#define g_CloneBuf		(*(PTR(void) *)&g_SceneVar3B20)	/* scene variable: the clone-maker buffer */

void XClone_StartMakerMode(void)
{
	g_CloneArenaFocus = 0;
	g_CloneWaveKills = 0;
	g_CloneRewardTimer = 0;
	g_CloneFlags = 0;
	g_CloneBufSize = XClone_MakerBufSize();
	g_CloneBuf = Mem_StackAlloc(g_CloneBufSize, STR(Str_Empty_099078, ""));
	g_SegTypeHooks[0x19] = XClone_SegHookArenaLocked;
	g_SegTypeHooks[0x1A] = (SegHookFn)CloneA5_SegHook16;
	unk_CloneA5_085932();
	g_SceneFrameHook = XClone_FrameHook;
	CloneA2_FlickerInit();
	Spr_ReloadDefs(XClone_LoadResources, 1);
}
