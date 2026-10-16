/* Level_Load - load a level for Level_Run: the display off and VRAM/VSRAM cleared, the heap
 * emptied, the scene's pre-load hook, the music, the palettes, the planes (Level_LoadMaps), the
 * floor segments, the spawn records, and every per-level system reset; then the scene's
 * post-load hook.
 *
 * fgMap, fgTiles, bgMap, bgTiles: the planes' map and tileset blobs (Level_LoadMaps; no BG: 0).
 * fgSegs, bgSegs: their floor-segment lists (Seg_LoadList; 0 none).  flag0-flag3: Level_LoadMaps'
 * flags; flag2 also takes the BG map's palette (line 2).  classArg: Obj_AllocPool's.  music: the
 * track (-1 none: the music volume slot gets $7F).  gridShiftX/Y: log2 of the floor-segment
 * grid's cell size (the grid covers the FG map, or the BG map without FG segments).
 * spawnFromBg: the spawn list is the BG map's.  classList: the level's classes (g_ClassList).
 * The FG map's palette goes to line 0. The segment type hooks, sprite definitions (def 1),
 * tile hooks, hooks of the frame loop (scroll Scroll_UpdateParallax, palette
 * Pal_RunCycleScript, frame Game_TickFrameCounter, draw DrawList_DrawAll, pause Pause_Check,
 * segment push-out Seg_ApplyPushOut, blocked Sys_Nop1F949E), the camera (none), the draw list,
 * the spawn/despawn/activate/deactivate windows, the timers and the hit lists are reset. */
#include "xmen2.h"

void Level_Load(void *fgMap, void *fgTiles, void *bgMap, void *bgTiles, void *fgSegs, void *bgSegs,
	unsigned char flag0, unsigned char flag1, register unsigned char flag2, unsigned char flag3,
	short classArg, short music, short gridShiftX, short gridShiftY, char spawnFromBg,
	void *classList)
{
	register long i;
	register s16 *pal;
	long m;
	register SprInst *spr;

	g_VdpReg1 &= ~0x40;
	VDP_CTRL = g_VdpReg1;
	VDP_ClearVram();
	VDP_ClearVsram();
	g_HeapTop = AT68(void, WORK_RAM_START);
	g_LevelFgMap = fgMap;
	g_LevelBgMap = bgMap;
	g_MapStreamNarrow = 1;
	g_BgNarrow = 0;
	g_SolidMapIsBg = 0;
	g_SiberiaIntro = 0;
	(*(FrameHookFn *)&g_ScenePreLoadHook)();
	g_MusicTrack = music;
	Snd_SetParam(0, g_MusicTrack == -1 ? 0x7F : g_MusicTrack);
	Snd_SetParam(1, 0);
	if (g_MusicTrack != -1)
		Music_Play(g_MusicTrack);
	g_ClassList = classList;
#ifdef XMEN2_MPW
	i = (long)fgMap;	/* the blob's address as a number, as the original */
	pal = (s16 *)((char *)i + ((MapHeader *)i)->palette);
#else
	pal = (s16 *)((char *)fgMap + ((MapHeader *)fgMap)->palette);
#endif
	for (i = 0; i < 16; i++)
		g_PalBase[i] = *(pal + i);
	if (bgMap && flag2) {
#ifdef XMEN2_MPW
		m = (long)bgMap;
		pal = (s16 *)((char *)m + ((MapHeader *)m)->palette);
#else
		pal = (s16 *)((char *)bgMap + ((MapHeader *)bgMap)->palette);
#endif
		for (i = 0; i < 16; i++)
			g_PalBase[i + 32] = *(pal + i);
	}
	Level_LoadMaps(fgMap, fgTiles, bgMap, bgTiles, 0, 0, flag0, flag1, flag2, flag3);
	g_CamShake = 0;
	g_VramTileCursor = g_MapTilesEnd;
	g_LevelLoadWord388A = 2;
	for (i = 0; i < 37; i++)
		g_SegTypeHooks[i] = 0;
	g_SegBlockedHook = FN_DATA(Sys_Nop1F949E);
	g_SpriteDefCount = 0;
	spr = g_SpriteDefs;
	for (i = 0; i < 35; spr++, i++) {
		spr->def = AT68(void, 1);
		spr->tileBase = 0;
	}
	g_SegGridShiftX = gridShiftX;
	g_SegGridShiftY = gridShiftY;
#ifdef XMEN2_MPW
	if (fgSegs) {
		i = (long)fgMap;
		g_SegGridW = ((short)((MapHeader *)i)->width * 8 - 1 >> g_SegGridShiftX) + 1;
		g_SegGridH = ((short)((MapHeader *)i)->height * 8 - 1 >> g_SegGridShiftY) + 1;
	} else {
		i = (long)bgMap;
		g_SegGridW = ((short)((MapHeader *)i)->width * 8 - 1 >> g_SegGridShiftX) + 1;
		g_SegGridH = ((short)((MapHeader *)i)->height * 8 - 1 >> g_SegGridShiftY) + 1;
	}
#else
	{
		MapHeader *gm = (MapHeader *)(fgSegs ? fgMap : bgMap);

		g_SegGridW = (((short)gm->width * 8 - 1) >> g_SegGridShiftX) + 1;
		g_SegGridH = (((short)gm->height * 8 - 1) >> g_SegGridShiftY) + 1;
	}
#endif
	Seg_ClearAll();
	if (fgSegs)
		Seg_LoadList((SegListEntry *)fgSegs, 0);
	if (bgSegs)
		Seg_LoadList((SegListEntry *)bgSegs, 1);
	Obj_AllocPool(classArg);
	{
		register s16 *list;
		register char *map;

		if (spawnFromBg)
			map = (char *)bgMap;
		else
			map = (char *)fgMap;
		list = (s16 *)map;
		map += ((MapHeader *)list)->spawns;
		list = (s16 *)map;
		g_SpawnRecCount = *list++;
		g_PlacementList = (Placement *)list;
	}
	Spawn_InitRecords();
	Scene_ClearActivateHook();
	ClassList_CallSlot5A();
	g_HudOn = 1;
	g_BgLockToObj = 0;
	g_CamActive = 1;
	g_SpawnEnabled = 1;
	g_LevelLoadWord3896 = 0;
	g_LevelLoadLong38A0 = 0;
	g_TileHooksOn = 0;
	{
		register short j;

		for (j = 0; j < 8; j++)
			g_TileHooks[j] = 0;
	}
	g_StartCamOffsetX = 0xA0;
	g_Camera = 0;
	g_CamModeSwitched = 0;
	g_ParallaxDesc = 0;
	g_LevelScrollHook = FN_DATA(Scroll_UpdateParallax);
	g_LevelPalHook = FN_DATA(Pal_RunCycleScript);
	g_PalCycleScript = 0;
	g_PalCycleCursor = 0;
	g_PalCycleDelay = -1;
	g_FrameCounter = 0;
	g_TwoPlayerStep = 1;
	g_LevelLoadByte38B8 = 0;
	g_SegResolveHook = FN_DATA(Seg_ApplyPushOut);
	g_SceneFrameHook = (FrameHookFn)Game_TickFrameCounter;
	g_SceneDrawHook = FN_DATA(DrawList_DrawAll);
	{
		register char ok = (char)DrawList_Init();

		g_SpawnWinLeft = -0x80;
		g_SpawnWinRight = 0x1C0;
		g_SpawnWinTop = -0x80;
		g_SpawnWinBottom = 0x160;
		g_DespawnWinLeft = -0x100;
		g_DespawnWinRight = 0x240;
		g_DespawnWinTop = -0x100;
		g_DespawnWinBottom = 0x1E0;
		g_DeactWinLeft = -0x64;
		g_DeactWinRight = 0x1A4;
		g_DeactWinTop = -0x32;
		g_DeactWinBottom = 0x13A;
		g_ActWinLeft = -0x5A;
		g_ActWinRight = 0x19A;
		g_ActWinTop = -0x28;
		g_ActWinBottom = 0x130;
		g_HalfSpeed = 0;
		g_ScalerWork = 0;
		g_PauseHandler = FN_DATA(Pause_Check);
		g_PauseExit = 0;
		g_LevelLoadByte1660 = 1;
		Timer_InitAll();
		HitList_Init();
		(*(FrameHookFn *)&g_ScenePostLoadHook)();
	}
}
