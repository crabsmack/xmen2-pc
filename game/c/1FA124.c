/* Level_FrameLoop - the level's frame loop (Level_Run): one game frame per pass until the scene
 * ends (g_SceneExitTimer runs out, or g_SceneIndexSet).
 *
 * Each frame: count the players alive (both: g_TwoPlayerStep, objects step 2 frames at once);
 * the HUD's sprites (with g_HudOn: each live player's health bar, its second meter for players
 * with one, a "press start" pair blinking for a missing player 2 in a demo or the SiberiaB1
 * intro with pad 2 plugged in, health below $201 redrawn every 16th frame; a null sprite when
 * the table is otherwise empty or without HUD); the scene's frame hook; the level frame counted;
 * the demo's input and the pads read. Unless frozen (g_LevelFreeze): the spawn records scanned,
 * g_SpawnScanPerFrame a frame round-robin (a record waiting (flags 1) spawns when in view, class
 * slot +$2A, or at once with flags 8: slot +$2E; a live record's object despawns when out of
 * view: object slots +$32 / +$36), or the level's spawn pass hook instead; every active object
 * ($C000) but the players deactivates when out of the window (slots +$3A / +$3E), or the
 * activate hook instead; the timers, the hits (Collide_RunAll), the camera (slot +$12). Start on
 * pad 1 ends a demo (g_StartEndsScene: any scene) in a frame. Unless frozen: the scroll and
 * palette hooks; the draw hook, the sprite table closed, the DMA flush requested; Start pressed
 * pauses (g_PauseHandler) unless fading or ending. Then the frame waits for its V-blank (in two
 * player frames, first for the frame tick), and the exit timer counts down 1 (2). */
#include "xmen2.h"

/* the level's hooks as the functions they are */
#define g_ActHook	(*(FrameHookFn *)&g_ActivateHook)
#define g_ScrollHook	(*(FrameHookFn *)&g_LevelScrollHook)
#define g_PalHook	(*(FrameHookFn *)&g_LevelPalHook)
#define g_DrawHook	(*(FrameHookFn *)&g_SceneDrawHook)
#define g_Pause		(*(FrameHookFn *)&g_PauseHandler)

/* the next slot of the sprite table along the link bytes, filled */
#define HUD_SPRITE(Y, S, A, X) do { \
	g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link); \
	g_SprWritePtr->y = (Y); \
	g_SprWritePtr->size = (S); \
	g_SprWritePtr->attr = (short)(A); \
	g_SprWritePtr->x = (X); \
} while (0)

void Level_FrameLoop(void)
{
	Actor *obj;
	PTR(Actor) *slot;
	XPlayer *pl;
	register long n;
	register long count;
	register long flags;
	register Actor *act;
	register SpawnRec *rec;

	for (;;) {
		g_PlayersAlive = 0;
		if (IS_ALIVE(&g_Player1))
			++g_PlayersAlive;
		if (IS_ALIVE(&g_Player2))
			++g_PlayersAlive;
#if defined(XMEN2_NATIVE)
		/* phase 6 R3 (full_rate_2p): two players on the one-player stepping, so no frame-tick
		   wait below and every counter steps by 1 (PlayerSelect_Run inherits the 0) */
		g_TwoPlayerStep = g_PlayersAlive == 2 && !g_opt.full_rate_2p;
#else
		g_TwoPlayerStep = g_PlayersAlive == 2;
#endif
		Spr_BeginFrame();
		if (g_HudOn) {
			if (g_HudDirty)
				HUD_UpdateHealthBars();
			pl = &g_Player1;
			if (IS_ALIVE(pl)) {
				if (pl->health <= 0x200 && !(g_FrameCounter & 0xE))
					HUD_UpdateHealthBars();
				HUD_SPRITE(0x132, 0xF, 0xA7E8, VIEW_L(0xA5));
				if (g_P1HasMeter)
					HUD_SPRITE(0x133, 7, 0xA7F8, VIEW_L(0x8F));
				else
					HUD_SPRITE(0x133, 3, 0xA7FC, VIEW_L(0x97));
			}
			pl = &g_Player2;
			if (IS_ALIVE(pl)) {
				if (pl->health <= 0x200 && !(g_FrameCounter & 0xE))
					HUD_UpdateHealthBars();
				HUD_SPRITE(0x132, 0xF, 0xA7D0, VIEW_R(0x17A));
				if (g_P2HasMeter)
					HUD_SPRITE(0x133, 7, 0xA7E0, VIEW_R(0x1A1));
				else
					HUD_SPRITE(0x133, 3, 0xA7E0, VIEW_R(0x1A1));
			} else if ((g_SiberiaIntro && (char)Pad_IsP2Connected()) || g_DemoActive) {
				if (g_FrameCounter & 0x10) {
					HUD_SPRITE(0x148, 0xC, 0xA7D0, VIEW_R(0x16D));
					HUD_SPRITE(0x148, 0xC, 0xA7D4, VIEW_R(0x191));
				}
			}
			if (g_SprIndex == 0x51)
				HUD_SPRITE(0x1C, 0, 0, 0x1C);
		} else
			HUD_SPRITE(0x1C, 0, 0, 0x1C);
#if defined(XMEN2_NATIVE)
		plat_spr_owner(&g_SceneFrameHook);	/* phase 6 R4: sprites of the scene's frame hook */
#endif
		(*g_SceneFrameHook)();
		g_LevelFrame++;
		g_FrameTick = 0;
		if (g_DemoActive)
			Demo_ReadInput();
		Pad_Update();
		if (!g_LevelFreeze) {
			if (g_SpawnEnabled) {
				if (g_SpawnPassHook)
					(*g_SpawnPassHook)();
				else {
					n = g_SpawnScanPerFrame;
					do {
						if (!g_SpawnScanLeft) {
							g_SpawnScanLeft = g_SpawnRecCount;
							g_SpawnScanPtr = g_SpawnRecEnd;
						}
						--g_SpawnScanLeft;
						--g_SpawnScanPtr;
						rec = g_SpawnScanPtr;
						flags = rec->flags;
						if (flags & 1 && (flags & 8 || (char)(*rec->cls->method[9])(rec, rec)))
							(*rec->cls->method[10])(rec, rec);	/* waiting, in view: spawn */
						else {
							obj = rec->object == 0xFF ? 0 : (Actor *)g_ObjTable[rec->object];
							if (obj && (char)(*obj->cls->method[11])(obj, rec))
								(*obj->cls->method[12])(obj, rec);	/* out of view: despawn */
						}
					} while (--n);
				}
				if (g_ActHook)
					(*g_ActHook)();
				else {
					count = g_ObjCount;
					count -= 2;
					slot = g_ObjTable + g_ObjCount;
					do {
						act = *--slot;
						if ((act->flags & 0xC000) == 0xC000 && (char)(*act->cls->method[13])(act))
							(*act->cls->method[14])(act);	/* out of the window: deactivate */
					} while (--count);
				}
			}
			Timer_RunAll();
#if defined(XMEN2_NATIVE)
			plat_spr_owner(&g_HitListPlayerAtk);	/* phase 6 R4: sprites of the hits */
#endif
			Collide_RunAll();
#if defined(XMEN2_NATIVE)
			plat_spr_owner(g_Camera);	/* phase 6 R4: sprites of the camera's update */
#endif
			if (g_CamActive)
				(*g_Camera->cls->method[3])(g_Camera);		/* Update */
		}
		if ((g_DemoActive || g_StartEndsScene) && (char)Pad_P1StartDown()) {
			Scene_EndAfter(1);
			g_DemoActive = 0;
			g_DemoSkipped = 1;
		}
		if (!g_LevelFreeze) {
#if defined(XMEN2_NATIVE)
			plat_spr_owner(&g_LevelScrollHook);	/* phase 6 R4: sprites of the scroll hook */
#endif
			(*g_ScrollHook)();
#if defined(XMEN2_NATIVE)
			plat_spr_owner(&g_LevelPalHook);	/* and of the palette hook */
#endif
			(*g_PalHook)();
		}
#if defined(XMEN2_NATIVE)
		plat_spr_owner(&g_SceneDrawHook);	/* phase 6 R4: sprites of the draw hook (the list marks its nodes) */
#endif
		(*g_DrawHook)();
		Spr_EndFrame();
		g_DmaFlushReq = 1;
		if ((g_Pad1Pressed & 0x80 || g_Pad2Pressed & 0x80) && !g_PalFadeActive && !g_SceneExitTimer)
			(*g_Pause)(0);
		if (g_SceneIndexSet)
			return;
		if (g_TwoPlayerStep && !g_LevelFreeze)
			while (!g_FrameTick)
				IRQ_WAIT();
		VDP_WaitVBlank();
		if (g_LevelFreeze)
			continue;
		if (g_SceneExitTimer <= 0)
			continue;
		g_SceneExitTimer -= g_TwoPlayerStep ? 2 : 1;
		if (g_SceneExitTimer <= 0)
			return;
	}
}
