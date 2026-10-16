/* xmen2_globals.h - the game's A5-world RAM globals, under the disassembly's names.
 *
 * MPW: plain externs, linked to their A5 offsets (cc.py --match-recomp checks every
 * reference). PC: the same names on the emulated work RAM, as the 68000 addresses the
 * original took them ($FFFFxxxx: A5-relative), so &g_X keeps its original value.
 * Types are the disassembly's (word/byte/long/ptr); arrays and records get their real
 * types as the code using them is written. First draft by tools/gen_headers.py.
 */
#ifndef XMEN2_GLOBALS_H
#define XMEN2_GLOBALS_H

#include "xmen2_types.h"

#ifdef XMEN2_MPW
#define GLOBAL(type, name, addr)              extern type name
#define GLOBAL_ARRAY(type, name, n, addr)     extern type name[n]
#else
#define GLOBAL(type, name, addr)              /* name = *(type *)$addr */
#define GLOBAL_ARRAY(type, name, n, addr)     /* name = (type *)$addr */
#define XM_G(type, addr)                      (*(type *)(g_ram_ffff + ((addr) & 0xFFFF)))
#define XM_GA(type, addr)                     ((type *)(g_ram_ffff + ((addr) & 0xFFFF)))
#endif

/* $FFC51A 6 words: AvalonB1 palette-cycle step counters passed by address (AvalonB1_CyclePalettes; AvalonB1_RunScene clears) */
#ifdef XMEN2_MPW
extern s16 g_AvalonB1PalCycles[6];
#else
#define g_AvalonB1PalCycles XM_GA(s16, 0xFFC51A)
#endif
/* $FFC526 ptr: debris pool from Debris_CreatePool in BigAcolyte_ActAppear (+2 = $400, +4 = sprite definition from BigAcolyte_LoadSprite); used by BigAcolyte_StateBurstWall */
#ifdef XMEN2_MPW
extern PTR(DebrisPool) g_BigAcolyteDebris;
#else
#define g_BigAcolyteDebris XM_G(PTR(DebrisPool), 0xFFC526)
#endif
/* $FFC52A word: sprite-definition slot of Art_FarAcolyteSpr3 (FarAcolyte_ClassInit via SprDef_Add; FarAcolyte_Draw) */
#ifdef XMEN2_MPW
extern s16 g_FarAcolyteSprDef3;
#else
#define g_FarAcolyteSprDef3 XM_G(s16, 0xFFC52A)
#endif
/* $FFC52C word: sprite-definition slot of Art_FarAcolyteSpr2 (FarAcolyte_ClassInit via SprDef_Add; FarAcolyte_Draw) */
#ifdef XMEN2_MPW
extern s16 g_FarAcolyteSprDef2;
#else
#define g_FarAcolyteSprDef2 XM_G(s16, 0xFFC52C)
#endif
/* $FFC52E word: sprite-definition slot of Art_YoyoAcolyteSpr2 (the yoyo, jt[149]) (YoyoAcolyte_ClassInit via SprDef_Add; YoyoAcolyte_Draw) */
#ifdef XMEN2_MPW
extern s16 g_YoyoAcolyteSprDef2;
#else
#define g_YoyoAcolyteSprDef2 XM_G(s16, 0xFFC52E)
#endif
/* $FFC530 word: AvalonA2 retracting floor value: $300 at AvalonA2_PlacePlayers, RetractFloor_StateWait adds $10 up to $500; copied into -$3AFA */
#ifdef XMEN2_MPW
extern s16 g_RetractFloorDepth;
#else
#define g_RetractFloorDepth XM_G(s16, 0xFFC530)
#endif
/* $FFC532 word: sprite-definition slot of Art_RetractFloorSpr (RetractFloor_ClassInit via SprDef_Add; RetractFloor_Init, RetractFloor_Draw) */
#ifdef XMEN2_MPW
extern s16 g_RetractFloorSprDef;
#else
#define g_RetractFloorSprDef XM_G(s16, 0xFFC532)
#endif
/* $FFC534 word: sprite-definition slot of Art_ZapperSpr (Zapper_ClassInit via SprDef_Add; Zapper_Init) */
#ifdef XMEN2_MPW
extern s16 g_ZapperSprDef;
#else
#define g_ZapperSprDef XM_G(s16, 0xFFC534)
#endif
/* $FFC536 4 records {word step 0-2, word delay}: AvalonA3's effect sprites 4..1 (the delays are
   g_AvalonA3SprDelays; AvalonA3_FrameHook steps them) */
#ifdef XMEN2_MPW
extern A3SprAnim g_AvalonA3SprAnim[4];
#else
#define g_AvalonA3SprAnim XM_GA(A3SprAnim, 0xFFC536)
#endif
/* $FFC538 4 words at stride 4: AvalonA3 effect-sprite frame delays (AvalonA3_InitSprites staggers them 3, 2, 1, 0; AvalonA3_FrameHook reloads 5) */
#ifdef XMEN2_MPW
extern s16 g_AvalonA3SprDelays[4];
#else
#define g_AvalonA3SprDelays XM_GA(s16, 0xFFC538)
#endif
/* $FFC546 word: AvalonA3 psi-flash countdown ($E from Exodus_StartPsiFlash; AvalonA3_PsiFlashStep steps the palette, AvalonA3_FrameHook tests; AvalonA3_SavePalettes clears) */
#ifdef XMEN2_MPW
extern s16 g_PsiFlashTimer;
#else
#define g_PsiFlashTimer XM_G(s16, 0xFFC546)
#endif
/* $FFC548 ptr: the EXODUS boss object (Exodus_Init stores itself; Exodus_CheckFloorHit) */
#ifdef XMEN2_MPW
extern PTR(void) g_ExodusObj;
#else
#define g_ExodusObj XM_G(PTR(void), 0xFFC548)
#endif
/* $FFC54C word: sprite-definition slot of Art_ExodusSpr (Exodus_ClassInit via SprDef_Add; Exodus_Init) */
#ifdef XMEN2_MPW
extern s16 g_ExodusSprDef;
#else
#define g_ExodusSprDef XM_G(s16, 0xFFC54C)
#endif
/* $FFC54E word: sprite-definition slot of Art_ExodusBlastNem (ExodusBlast_ClassInit via SprDef_Add; ExodusBlast_Init) */
#ifdef XMEN2_MPW
extern s16 g_ExodusBlastSprDef;
#else
#define g_ExodusBlastSprDef XM_G(s16, 0xFFC54E)
#endif
/* $FFC550 byte: AvalonA4 palette animation step, index into Tbl_AvalonA4PalPulse (AvalonA4_AnimatePalettes, _part4; AvalonA4_ResetEffectCounters clears; signed: ext.w) */
#ifdef XMEN2_MPW
extern s8 g_AvalonA4PalStep;
#else
#define g_AvalonA4PalStep XM_G(s8, 0xFFC550)
#endif
/* $FFC552 word: class index whose g_ClassData entry is the rubble particle pool (MagnetoBoss_ClassInit; MagnetoBoss_SpawnRubble) */
#ifdef XMEN2_MPW
extern s16 g_MagnetoRubbleClass;
#else
#define g_MagnetoRubbleClass XM_G(s16, 0xFFC552)
#endif
/* $FFC554 word: sprite-definition slot of Art_MagnetoBossSpr (MagnetoBoss_ClassInit via SprDef_Add; read by MagnetoBoss_RubbleTimer) */
#ifdef XMEN2_MPW
extern s16 g_MagnetoBossSprDef;
#else
#define g_MagnetoBossSprDef XM_G(s16, 0xFFC554)
#endif
/* $FFC556 word: sprite-definition slot of Art_ThronePhoneSpr (ThronePhone_ClassInit via SprDef_Add; ThronePhone_Init, ThronePhone_Draw) */
#ifdef XMEN2_MPW
extern s16 g_ThronePhoneSprDef;
#else
#define g_ThronePhoneSprDef XM_G(s16, 0xFFC556)
#endif
/* $FFC558 4 ptrs: BaniMazaA1_InitTablePtrs points them at Tbl_BaniMazaA1ColourSeq4, 3, 2, 1; nothing reads them */
#ifdef XMEN2_MPW
extern PTR(void) g_BmA1ColourSeqs[4];
#else
#define g_BmA1ColourSeqs XM_GA(PTR(void), 0xFFC558)
#endif
/* $FFC568 bytes: per-band direction flags of the rolling discs (RollingDiscs_UpdateScroll); BaniMazaA1_AnimateBlocks reverses its block animations with them */
#ifdef XMEN2_MPW
extern u8 g_DiscBandReverse[];
#else
#define g_DiscBandReverse XM_GA(u8, 0xFFC568)
#endif
/* $FFC56C word: BaniMazaA1 switch effect timer ($28 when a switch tile is hit, BaniMazaA1_SwitchTileHook; BaniMazaA1_FrameHook counts down; InstallTileHooks clears) */
#ifdef XMEN2_MPW
extern s16 g_BmA1SwitchTimer;
#else
#define g_BmA1SwitchTimer XM_G(s16, 0xFFC56C)
#endif
/* $FFC56E word: animation frame of BaniMazaA1 block group 2 (BaniMazaA1_AnimateBlocks) */
#ifdef XMEN2_MPW
extern u16 g_BmA1BlockFrame2;
#else
#define g_BmA1BlockFrame2 XM_G(u16, 0xFFC56E)
#endif
/* $FFC570 word: step timer of BaniMazaA1 block group 2 (BaniMazaA1_AnimateBlocks; InitSprites starts it at 4) */
#ifdef XMEN2_MPW
extern s16 g_BmA1BlockTimer2;
#else
#define g_BmA1BlockTimer2 XM_G(s16, 0xFFC570)
#endif
/* $FFC572 word: animation frame of BaniMazaA1 block group 1 (BaniMazaA1_AnimateBlocks) */
#ifdef XMEN2_MPW
extern u16 g_BmA1BlockFrame1;
#else
#define g_BmA1BlockFrame1 XM_G(u16, 0xFFC572)
#endif
/* $FFC574 word: step timer of BaniMazaA1 block group 1 (BaniMazaA1_AnimateBlocks; InitSprites starts it at 2) */
#ifdef XMEN2_MPW
extern s16 g_BmA1BlockTimer1;
#else
#define g_BmA1BlockTimer1 XM_G(s16, 0xFFC574)
#endif
/* $FFC576 word: animation frame of BaniMazaA1 block group 0 (8 frames; BaniMazaA1_AnimateBlocks) */
#ifdef XMEN2_MPW
extern u16 g_BmA1BlockFrame0;
#else
#define g_BmA1BlockFrame0 XM_G(u16, 0xFFC576)
#endif
/* $FFC578 word: frames until BaniMazaA1 animated block group 0 steps (6; BaniMazaA1_AnimateBlocks, BaniMazaA1_InitSprites) */
#ifdef XMEN2_MPW
extern s16 g_BmA1BlockTimer0;
#else
#define g_BmA1BlockTimer0 XM_G(s16, 0xFFC578)
#endif
/* $FFC57A word: sprite-definition slot of Art_BanimazaDoorSpr (BanimazaDoor_ClassInit via SprDef_Add; BanimazaDoor_Init) */
#ifdef XMEN2_MPW
extern s16 g_BanimazaDoorSprDef;
#else
#define g_BanimazaDoorSprDef XM_G(s16, 0xFFC57A)
#endif
/* $FFC57C ptr: $120-byte buffer allocated by BanimazaDoor_ClassInit; each BANIMAZADOOR keeps it in +$8C */
#ifdef XMEN2_MPW
extern PTR(void) g_BanimazaDoorBuf;
#else
#define g_BanimazaDoorBuf XM_G(PTR(void), 0xFFC57C)
#endif
/* $FFC580 word: frames before rolling discs can hurt a player again ($DC after a hit; RollingDiscs_UpdateScroll, ClassInit clears) */
#ifdef XMEN2_MPW
extern s16 g_DiscHurtCooldown;
#else
#define g_DiscHurtCooldown XM_G(s16, 0xFFC580)
#endif
/* $FFC582 ptr: rolling-disc band records, $14 bytes each (RollingDiscs_ClassInit; UpdateScroll, ReverseAtY; RollingDiscs_Init keeps it in +$8C) */
#ifdef XMEN2_MPW
extern PTR(DiscBand) g_DiscBands;
#else
#define g_DiscBands XM_G(PTR(DiscBand), 0xFFC582)
#endif
/* $FFC586 word: number of rolling-disc bands (RollingDiscs_ClassInit from the level list; RollingDiscs_UpdateScroll, RollingDiscs_ReverseAtY) */
#ifdef XMEN2_MPW
extern s16 g_DiscBandCount;
#else
#define g_DiscBandCount XM_G(s16, 0xFFC586)
#endif
/* $FFC588 ptr: the ROLLINGDISCS object (RollingDiscs_Init stores self) */
#ifdef XMEN2_MPW
extern PTR(RollingDiscs) g_RollingDiscs;
#else
#define g_RollingDiscs XM_G(PTR(RollingDiscs), 0xFFC588)
#endif
/* $FFC58C word: sprite-definition slot of Art_ApocalypseSpr (Apocalypse_ClassInit via SprDef_Add; Apocalypse_Init) */
#ifdef XMEN2_MPW
extern s16 g_ApocalypseSprDef;
#else
#define g_ApocalypseSprDef XM_G(s16, 0xFFC58C)
#endif
/* $FFC58E word: increment ApocBomb_AccelTimer adds to g_ApocBombSpeed (ApocBomb_SetSpeed; ApocBomb_StartAccelTimer clears) */
#ifdef XMEN2_MPW
extern s16 g_ApocBombAccel;
#else
#define g_ApocBombAccel XM_G(s16, 0xFFC58E)
#endif
/* $FFC590 word: speed given to new and moving APOCBOMBs (ApocBomb_SetSpeed, raised by ApocBomb_AccelTimer up to $280; ApocBomb_Init/Move copy it to +$90) */
#ifdef XMEN2_MPW
extern s16 g_ApocBombSpeed;
#else
#define g_ApocBombSpeed XM_G(s16, 0xFFC590)
#endif
/* $FFC592 word: sprite-definition slot of Art_ApocBombSpr (ApocBomb_ClassInit via SprDef_Add; ApocBomb_Init, ApocBomb_DrawShadow) */
#ifdef XMEN2_MPW
extern s16 g_ApocBombSprDef;
#else
#define g_ApocBombSprDef XM_G(s16, 0xFFC592)
#endif
/* $FFC594 word: sprite-definition slot of Art_BoulderSpr2 (Boulder_ClassInit via SprDef_Add; Boulder_Init) */
#ifdef XMEN2_MPW
extern s16 g_BoulderSprDef2;
#else
#define g_BoulderSprDef2 XM_G(s16, 0xFFC594)
#endif
/* $FFC596 word: sprite-definition slot of Art_BoulderSpr (Boulder_ClassInit via SprDef_Add; Boulder_Init) */
#ifdef XMEN2_MPW
extern s16 g_BoulderSprDef;
#else
#define g_BoulderSprDef XM_G(s16, 0xFFC596)
#endif
/* $FFC598 ptr: the 30 boulder debris records, live ones first (Boulder_InitDebrisPool; Alloc/Free/Update) */
#ifdef XMEN2_MPW
extern PTR(BoulderDebris) g_BoulderDebrisPool;
#else
#define g_BoulderDebrisPool XM_G(PTR(BoulderDebris), 0xFFC598)
#endif
/* $FFC59C word: boulder debris in use (Boulder_InitDebrisPool clears; Boulder_AllocDebris/FreeDebris/UpdateDebris) */
#ifdef XMEN2_MPW
extern s16 g_BoulderDebrisCount;
#else
#define g_BoulderDebrisCount XM_G(s16, 0xFFC59C)
#endif
/* $FFC59E word: sprite-definition slot of Art_BoulderSpr3 (Boulder_ClassInit via SprDef_Add; Boulder_UpdateDebris) */
#ifdef XMEN2_MPW
extern s16 g_BoulderDebrisSprDef;
#else
#define g_BoulderDebrisSprDef XM_G(s16, 0xFFC59E)
#endif
/* $FFC5A0 word: sprite-definition slot of Art_BaniMazaA1Nem3 (Tuskette_ClassInit via SprDef_Add; Tuskette_Init) */
#ifdef XMEN2_MPW
extern s16 g_TusketteSprDef;
#else
#define g_TusketteSprDef XM_G(s16, 0xFFC5A0)
#endif
/* $FFC5A2 word: game progress flags: bit 1 = logo START pressed / demos seen (Cerebro_PlayIntro, Game_PlayAttractDemo, SiberiaB1_FrameHook), $4000 = game finished (FinalText_RunScene), $8000 = SentinelsB2 reached, $10/$20/$200/$400/$2000 set by pause cheats (Pause_CheatUnlock); Credits/Failure clear it */
#ifdef XMEN2_MPW
extern s16 g_ProgressFlags;
#else
#define g_ProgressFlags XM_G(s16, 0xFFC5A2)
#endif
/* $FFC5A4 word: Cerebro text window bottom row (Cerebro_Run; Cerebro_PrintText, ClearTextWindow); in levels LightBeam_ClassInit reuses it for its sprite-definition slot (LightBeam_Init) */
#ifdef XMEN2_MPW
extern s16 g_TextWinBottom;
#else
#define g_TextWinBottom XM_G(s16, 0xFFC5A4)
#endif
/* $FFC5A6 word: Cerebro text window right column (Cerebro_Run, CursorInput_Confirm; Cerebro_PrintText, Cerebro_ClearTextWindow); Pause_LinkSprites also writes it */
#ifdef XMEN2_MPW
extern s16 g_TextWinRight;
#else
#define g_TextWinRight XM_G(s16, 0xFFC5A6)
#endif
/* $FFC5A8 word: Cerebro text window top row (Cerebro_Run, CursorInput_Confirm); the pause code reuses it as its button-code state (Pause_HandleInput, Pause_HandleInput_EndCode) */
#ifdef XMEN2_MPW
extern s16 g_TextWinTop;
#else
#define g_TextWinTop XM_G(s16, 0xFFC5A8)
#endif
/* $FFC5AA word: Cerebro text window left column (Cerebro_Run, CursorInput_Confirm, Failure_RunScene; Cerebro_PrintText/ClearTextWindow) */
#ifdef XMEN2_MPW
extern s16 g_TextWinLeft;
#else
#define g_TextWinLeft XM_G(s16, 0xFFC5AA)
#endif
/* $FFC5AC 3 words: CloneA5 palette-cycle step counters passed by address (CloneA5_FrameHook; CloneA5_ResetPalCycle clears) */
#ifdef XMEN2_MPW
extern s16 g_CloneA5PalCycles[3];
#else
#define g_CloneA5PalCycles XM_GA(s16, 0xFFC5AC)
#endif
/* $FFC5B2 word: CloneA5 camera flags; bit 0 set by CloneA5_InitCamera, tested by CloneA5_CamApply */
#ifdef XMEN2_MPW
extern s16 g_CloneA5CamFlags;
#else
#define g_CloneA5CamFlags XM_G(s16, 0xFFC5B2)
#endif
/* $FFC5B4 word: sprite-definition slot of Art_BroodClawSpr (CloneA5_LoadResources via SprDef_Add; BroodClaw_Init) */
#ifdef XMEN2_MPW
extern s16 g_BroodClawSprDef;
#else
#define g_BroodClawSprDef XM_G(s16, 0xFFC5B4)
#endif
/* $FFC5B6 byte: BROODBOSS health * 2 (BroodBoss_Update; 5 at CloneA5_RunScene); CloneA5_FrameHook writes it into the object at -$3B18 (+4) */
#ifdef XMEN2_MPW
extern u8 g_BroodBossHealthBar;
#else
#define g_BroodBossHealthBar XM_G(u8, 0xFFC5B6)
#endif
/* $FFC5B8 long: BROODBOSS vertical offset ($3300 at CloneA5_RunScene; BroodBoss_Update moves it by $200 between $2B00 and $3300); parts and CloneA5_CamApply read it */
#ifdef XMEN2_MPW
extern s32 g_BroodBossOffsetY;
#else
#define g_BroodBossOffsetY XM_G(s32, 0xFFC5B8)
#endif
/* $FFC5BC long: BROODBOSS x offset (BroodBoss_StateRoar resets it; Recoil/Advance/Death states move it); parts, LittleLeg and CloneA5_CamApply (boss plane scroll) read it */
#ifdef XMEN2_MPW
extern s32 g_BroodBossOffsetX;
#else
#define g_BroodBossOffsetX XM_G(s32, 0xFFC5BC)
#endif
/* $FFC5C0 word: clones killed in the current wave; 6 end it (XClone_Kill; XClone_ClassInit/StartMakerMode clear; BroodClaw_StateDropClone counts) */
#ifdef XMEN2_MPW
extern s16 g_CloneWaveKills;
#else
#define g_CloneWaveKills XM_G(s16, 0xFFC5C0)
#endif
/* $FFC5C2 word: sprite-definition slot of Art_PlayerSpr used for the objects EVILBEAST throws (XClone_LoadResources via SprDef_Add; EvilBeast_ActThrow, EvilBeast_UpdateThrows) */
#ifdef XMEN2_MPW
extern s16 g_EvilBeastThrowSprDef;
#else
#define g_EvilBeastThrowSprDef XM_G(s16, 0xFFC5C2)
#endif
/* $FFC5C4 word: clones killed while the clone maker runs (XClone_KillAndCount; EvilMaker_StateWaitKills compares; XClone_ClassInit clears; BroodClaw_StateDropClone tests) */
#ifdef XMEN2_MPW
extern s16 g_CloneMakerKills;
#else
#define g_CloneMakerKills XM_G(s16, 0xFFC5C4)
#endif
/* $FFC5C6 word: sprite-definition slot of Art_FloatBubbleSpr (FloatBubble_ClassInit via SprDef_Add; FloatBubble_Init) */
#ifdef XMEN2_MPW
extern s16 g_FloatBubbleSprDef;
#else
#define g_FloatBubbleSprDef XM_G(s16, 0xFFC5C6)
#endif
/* $FFC5C8 ptr: the PERIMETERDOOR object (PerimeterDoor_Init stores itself; PerimeterDoor_TileHook) */
#ifdef XMEN2_MPW
extern PTR(void) g_PerimeterDoorObj;
#else
#define g_PerimeterDoorObj XM_G(PTR(void), 0xFFC5C8)
#endif
/* $FFC5CC word: hits taken by the perimeter door (PerimeterDoor_ClassInit clears, PerimeterDoor_TileHit counts; PerimeterDoor_StateWaitBroken compares) */
#ifdef XMEN2_MPW
extern s16 g_PerimeterDoorHits;
#else
#define g_PerimeterDoorHits XM_G(s16, 0xFFC5CC)
#endif
/* $FFC5CE long: SavageA2 flood line y in 8.8 fixed point, decreased by SavageA2_CamUpdateFlood (SavageA2_InitFlood sets it below the bottom camera limit) */
#ifdef XMEN2_MPW
extern s32 g_FloodY88;
#else
#define g_FloodY88 XM_G(s32, 0xFFC5CE)
#endif
/* $FFC5D2 word: sprite-definition slot of Art_SavageA4Spr2 (SavageA4_LoadResources via SprDef_Add; SavageBigBio_Draw, SavageBio_StateReloadArt10/14) */
#ifdef XMEN2_MPW
extern s16 g_SavageBioSprDef;
#else
#define g_SavageBioSprDef XM_G(s16, 0xFFC5D2)
#endif
/* $FFC5D4 word: sprite-definition slot of Art_LauncherSpr2, the launcher's shots (Launcher_ClassInit via SprDef_Add; Launcher_Draw) */
#ifdef XMEN2_MPW
extern s16 g_LauncherShotSprDef;
#else
#define g_LauncherShotSprDef XM_G(s16, 0xFFC5D4)
#endif
/* $FFC5D6 word: sprite-definition slot of Art_LauncherSpr (Launcher_ClassInit via SprDef_Add; Launcher_Init) */
#ifdef XMEN2_MPW
extern s16 g_LauncherSprDef;
#else
#define g_LauncherSprDef XM_G(s16, 0xFFC5D6)
#endif
/* $FFC5D8 ptr: spore pool records (SporeMine_InitPool allocates; Add/Remove/Update/Draw) */
#ifdef XMEN2_MPW
extern PTR(void) g_SporeMinePool;
#else
#define g_SporeMinePool XM_G(PTR(void), 0xFFC5D8)
#endif
/* $FFC5DC word: spores in use (SporeMine_InitPool clears; SporeMine_AddSpore/RemoveSpore/UpdatePool/DrawPool) */
#ifdef XMEN2_MPW
extern s16 g_SporeMinePoolCount;
#else
#define g_SporeMinePoolCount XM_G(s16, 0xFFC5DC)
#endif
/* $FFC5DE word: sprite-definition slot of Art_SporeMineSpr (SporeMine_ClassInit via SprDef_Add; SporeMine_Init, SporeMine_DrawPool) */
#ifdef XMEN2_MPW
extern s16 g_SporeMineSprDef;
#else
#define g_SporeMineSprDef XM_G(s16, 0xFFC5DE)
#endif
/* $FFC5E0 word: sprite-definition slot of Tbl_SpikeBallVdp (SpikeBall_ClassInit via SprDef_Add; SpikeBall_Init) */
#ifdef XMEN2_MPW
extern s16 g_SpikeBallSprDef;
#else
#define g_SpikeBallSprDef XM_G(s16, 0xFFC5E0)
#endif
/* $FFC5E2 word: frames left in the current jungle palette phase (SavageJungle_PalPhaseUpdate*) */
#ifdef XMEN2_MPW
extern s16 g_JunglePalTimer;
#else
#define g_JunglePalTimer XM_G(s16, 0xFFC5E2)
#endif
/* $FFC5E4 word: current jungle palette phase (SavageJungle_InitPalPhases clears, SavageJungle_PalPhaseUpdate increments); BubbleManager_SetWaveParams and SwampFly_SpawnRecInView read it */
#ifdef XMEN2_MPW
extern s16 g_JunglePalPhase;
#else
#define g_JunglePalPhase XM_G(s16, 0xFFC5E4)
#endif
/* $FFC5E6 ptr: current palette-phase script entry (SavageJungle_InitPalPhases from the BG map's +$12 list; SavageJungle_PalPhaseUpdate cases) */
#ifdef XMEN2_MPW
extern PTR(void) g_JunglePalScript;
#else
#define g_JunglePalScript XM_G(PTR(void), 0xFFC5E6)
#endif
/* $FFC5EA word: sprite-definition slot of Art_SwampPlatformSpr (SwampPlatform_ClassInit via SprDef_Add; SwampPlatform_Init) */
#ifdef XMEN2_MPW
extern s16 g_SwampPlatformSprDef;
#else
#define g_SwampPlatformSprDef XM_G(s16, 0xFFC5EA)
#endif
/* $FFC5EC byte: players below the water surface take damage (Water_Init sets; Water_DrawHook) */
#ifdef XMEN2_MPW
extern u8 g_WaterHurts;
#else
#define g_WaterHurts XM_G(u8, 0xFFC5EC)
#endif
/* $FFC5EE word: hazard water splash y (Water_Init arg, SavageA2_CamUpdateFlood; Water_DrawHook) */
#ifdef XMEN2_MPW
extern s16 g_WaterSplashY;
#else
#define g_WaterSplashY XM_G(s16, 0xFFC5EE)
#endif
/* $FFC5F0 word: hazard water surface y (Water_Init arg; SavageA2_CamUpdateFlood follows the flood line); Water_DrawHook hurts players below it */
#ifdef XMEN2_MPW
extern s16 g_WaterSurfaceY;
#else
#define g_WaterSurfaceY XM_G(s16, 0xFFC5F0)
#endif
/* $FFC5F2 ptr: splash sprite record from SprDef_Add in Water_Init; drawn by Water_DrawSplash */
#ifdef XMEN2_MPW
extern PTR(void) g_WaterSplashSpr;
#else
#define g_WaterSplashSpr XM_G(PTR(void), 0xFFC5F2)
#endif
/* $FFC5F6 word: frames left of player 2's water splash (Water_DrawHook, Water_Init clears) */
#ifdef XMEN2_MPW
extern s16 g_P2SplashTimer;
#else
#define g_P2SplashTimer XM_G(s16, 0xFFC5F6)
#endif
/* $FFC5F8 word: frames left of player 1's water splash ($24 when P1 drops below g_WaterSurfaceY; Water_DrawHook, Water_Init clears) */
#ifdef XMEN2_MPW
extern s16 g_P1SplashTimer;
#else
#define g_P1SplashTimer XM_G(s16, 0xFFC5F8)
#endif
/* $FFC602 word: sprite-definition slot of Art_SentinelsD1Spr (SentinelsD1_LoadResources via SprDef_Add; ControlBox_Init, ControlPanel_Init) */
#ifdef XMEN2_MPW
extern s16 g_ControlPanelSprDef;
#else
#define g_ControlPanelSprDef XM_G(s16, 0xFFC602)
#endif
/* $FFC604 word: sprite-definition slot of Art_Spr_0DBFC0 (SentinelsD1_LoadResources via SprDef_Add; ControlBox_Draw, ControlPanel_Draw) */
#ifdef XMEN2_MPW
extern s16 g_ControlPanelSprDef2;
#else
#define g_ControlPanelSprDef2 XM_G(s16, 0xFFC604)
#endif
/* $FFC606 ptr: flag-bit array allocated by SentinelsD1_InitFlagBits; Blast/ControlBox/ControlPanel/SentinelDoor/SentinelElevator Init link to it */
#ifdef XMEN2_MPW
extern PTR(void) g_SentinelsD1Flags;
#else
#define g_SentinelsD1Flags XM_G(PTR(void), 0xFFC606)
#endif
/* $FFC60A word: sprite-definition slot of Art_BlastSpr (Blast_ClassInit via SprDef_Add; Blast_Draw) */
#ifdef XMEN2_MPW
extern s16 g_BlastSprDef;
#else
#define g_BlastSprDef XM_G(s16, 0xFFC60A)
#endif
/* $FFC60C word: sprite-definition slot of Art_BarrierSpr (Barrier_ClassInit, Blast_ClassInit via SprDef_Add; Barrier_Init, ClosedBarrier_Init, Blast_Init) */
#ifdef XMEN2_MPW
extern s16 g_BarrierSprDef;
#else
#define g_BarrierSprDef XM_G(s16, 0xFFC60C)
#endif
/* $FFC60E 6 words: SentinelsA1 palette-cycle step counters passed by address to the cycle stepper (SentinelsA1_CyclePalettes; SentinelsA1_ResetPalCycles clears) */
#ifdef XMEN2_MPW
extern s16 g_SentinelsA1PalCycles[6];
#else
#define g_SentinelsA1PalCycles XM_GA(s16, 0xFFC60E)
#endif
/* $FFC61A byte: SentinelsA1 exit opened by Hologram_OpenExit; SentinelsA1_CyclePalettes switches palette cycles on it (SentinelsA1_InstallHooks clears) */
#ifdef XMEN2_MPW
extern u8 g_SentinelsA1ExitOpen;
#else
#define g_SentinelsA1ExitOpen XM_G(u8, 0xFFC61A)
#endif
/* $FFC61C word: sprite-definition slot of Art_SentinelsA1Spr24 (SentinelsA1_LoadResources via SprDef_Add; HoloBolt_Init) */
#ifdef XMEN2_MPW
extern s16 g_HoloBoltSprDef;
#else
#define g_HoloBoltSprDef XM_G(s16, 0xFFC61C)
#endif
/* $FFC61E word: sprite-definition slot of Art_SentinelsA1Spr25 (SentinelsA1_LoadResources via SprDef_Add; Hologram_Init) */
#ifdef XMEN2_MPW
extern s16 g_HologramSprDef;
#else
#define g_HologramSprDef XM_G(s16, 0xFFC61E)
#endif
/* $FFC620 byte: hologram flicker armed (Hologram_Init, Hologram_SetPose, Hologram_Draw set; Hologram_VBlankFlicker consumes) */
#ifdef XMEN2_MPW
extern u8 g_HoloFlickerArmed;
#else
#define g_HoloFlickerArmed XM_G(u8, 0xFFC620)
#endif
/* $FFC621 byte: hologram flicker running (Hologram_VBlankFlicker: copy of g_HoloFlickerLinks[1] taken when armed; 0 = off). Not named in the disassembly yet. */
#ifdef XMEN2_MPW
extern u8 g_HoloFlickerOn;
#else
#define g_HoloFlickerOn XM_G(u8, 0xFFC621)
#endif
/* $FFC622 2 bytes: sprite slots / link values set by Hologram_Draw; Hologram_VBlankFlicker copies the word and rewrites that sprite link in VRAM on odd V-blanks */
#ifdef XMEN2_MPW
extern u8 g_HoloFlickerLinks[2];
#else
#define g_HoloFlickerLinks XM_GA(u8, 0xFFC622)
#endif
/* $FFC624 2 bytes: Hologram_VBlankFlicker's copy of g_HoloFlickerLinks taken when armed: [0] the sprite slot before the hologram's, [1] the slot after them. Not named in the disassembly yet. */
#ifdef XMEN2_MPW
extern u8 g_HoloFlickerSlots[2];
#else
#define g_HoloFlickerSlots XM_GA(u8, 0xFFC624)
#endif
/* $FFC626 2 bytes: the size and link bytes Hologram_VBlankFlicker writes to sprite slot g_HoloFlickerSlots[0] in VRAM (size saved when armed; link to the hologram's sprites or past them). Not named in the disassembly yet. */
#ifdef XMEN2_MPW
extern u8 g_HoloFlickerSpr[2];
#else
#define g_HoloFlickerSpr XM_GA(u8, 0xFFC626)
#endif
/* $FFC628 word: sprite-definition slot of Art_SentinelsA1Spr26 (SentinelsA1_LoadResources via SprDef_Add; CoreShields_Init) */
#ifdef XMEN2_MPW
extern s16 g_CoreShieldsSprDef;
#else
#define g_CoreShieldsSprDef XM_G(s16, 0xFFC628)
#endif
/* $FFC62A word: sprite-definition slot of Art_SentinelsA1Spr27 (SentinelsA1_LoadResources via SprDef_Add; CoreElevator_Init) */
#ifdef XMEN2_MPW
extern s16 g_CoreElevatorSprDef;
#else
#define g_CoreElevatorSprDef XM_G(s16, 0xFFC62A)
#endif
/* $FFC62C word: sprite-definition slot of Art_Platform1Spr, loaded once per level (Platform1_ClassInit compares and sets; Platform1_LevelStart clears; Platform1_Init) */
#ifdef XMEN2_MPW
extern s16 g_Platform1SprDef;
#else
#define g_Platform1SprDef XM_G(s16, 0xFFC62C)
#endif
/* $FFC632 ptr: 16-byte record allocated by Switch_ClassInit for the tile Switch_TileHit is looking up */
#ifdef XMEN2_MPW
extern PTR(void) g_SwitchSearchRec;
#else
#define g_SwitchSearchRec XM_G(PTR(void), 0xFFC632)
#endif
/* $FFC636 byte: a SWITCH lookup timer is running; Switch_TileHit only starts one when clear (Switch_ClassInit, Switch_DebounceWait, Switch_FindAndToggle clear) */
#ifdef XMEN2_MPW
extern u8 g_SwitchSearchBusy;
#else
#define g_SwitchSearchBusy XM_G(u8, 0xFFC636)
#endif
/* $FFC638 ptr: $A8-byte zeroed buffer on SpaceA1_RunScene's stack (next to the ELEVATOR); Deathbird_Init keeps it in +$9A */
#ifdef XMEN2_MPW
extern PTR(void) g_SpaceA1Buf;
#else
#define g_SpaceA1Buf XM_G(PTR(void), 0xFFC638)
#endif
/* $FFC63C ptr: SpaceA1 ELEVATOR object, on SpaceA1_RunScene's stack (SpaceA1_CamUpdate) */
#ifdef XMEN2_MPW
extern PTR(Elevator) g_Elevator;
#else
#define g_Elevator XM_G(PTR(Elevator), 0xFFC63C)
#endif
/* $FFC640 word: sprite-definition slot of Art_DeathbirdSpr (Deathbird_ClassInit via SprDef_Add; Deathbird_Init) */
#ifdef XMEN2_MPW
extern s16 g_DeathbirdSprDef;
#else
#define g_DeathbirdSprDef XM_G(s16, 0xFFC640)
#endif
/* $FFC642 word: frames until Deathbird_StateSendHornets spawns the next HORNET from its schedule ($78 at Deathbird_Init) */
#ifdef XMEN2_MPW
extern s16 g_HornetWaveTimer;
#else
#define g_HornetWaveTimer XM_G(s16, 0xFFC642)
#endif
/* $FFC644 byte: P1 character 0 Beast 1 Cyclops 2 Gambit 3 Nightcrawler 4 Psylocke 5 Wolverine 6 Magneto, -1 = not playing (Player_CharIndexOf; TAS RAM vs screenshots); signed: Game_Init stores -1 as move.b #$FFFF */
#ifdef XMEN2_MPW
extern s8 g_P1CharIndex;
#else
#define g_P1CharIndex XM_G(s8, 0xFFC644)
#endif
/* $FFC645 byte: P2 character index (same encoding as g_P1CharIndex), -1 = not playing */
#ifdef XMEN2_MPW
extern s8 g_P2CharIndex;
#else
#define g_P2CharIndex XM_G(s8, 0xFFC645)
#endif
/* $FFC646 byte: SiberiaB1 opening phase of a new game: set by SiberiaB1_PreLoad, cleared when P1 passes x $360 or the stage music starts (SiberiaB1_FrameHook; Level_Load clears); Player1/2_StartLife and Level_FrameLoop handle a joining player differently while set */
#ifdef XMEN2_MPW
extern u8 g_SiberiaIntro;
#else
#define g_SiberiaIntro XM_G(u8, 0xFFC646)
#endif
/* $FFC648 word: first VRAM tile of the shared effect graphics (set from g_VramTileCursor in Scene_AllocCommonVram; used by Explosion_Update and NINJA effects) */
#ifdef XMEN2_MPW
extern s16 g_FxTileBase;
#else
#define g_FxTileBase XM_G(s16, 0xFFC648)
#endif
/* $FFC64A word: VRAM tiles reserved per player = Player_CalcMaxTiles() (Game_InitButtonMasks); Scene_PostLoadVram, Spr_ReloadDefs, PlayerSelect_AllocCharBuffers_Measure */
#ifdef XMEN2_MPW
extern s16 g_PlayerTiles;
#else
#define g_PlayerTiles XM_G(s16, 0xFFC64A)
#endif
/* $FFC64C word: first VRAM tile reserved for player 2 = g_P1VramTile + g_PlayerTiles (Scene_PostLoadVram; character Inits, Spr_ReloadDefs) */
#ifdef XMEN2_MPW
extern s16 g_P2VramTile;
#else
#define g_P2VramTile XM_G(s16, 0xFFC64C)
#endif
/* $FFC64E word: first VRAM tile reserved for player 1's sprite graphics (Scene_PostLoadVram = g_VramTileCursor); the character Inits use it when this == g_Player1 */
#ifdef XMEN2_MPW
extern s16 g_P1VramTile;
#else
#define g_P1VramTile XM_G(s16, 0xFFC64E)
#endif
/* $FFC650 word: pad bit for Mode ($800, Game_InitButtonMasks) */
#ifdef XMEN2_MPW
extern s16 g_PadMaskMode;
#else
#define g_PadMaskMode XM_G(s16, 0xFFC650)
#endif
/* $FFC652 word: pad bit for button Z ($100, Game_InitButtonMasks) */
#ifdef XMEN2_MPW
extern s16 g_PadMaskZ;
#else
#define g_PadMaskZ XM_G(s16, 0xFFC652)
#endif
/* $FFC654 word: pad bit for button Y ($200, Game_InitButtonMasks) */
#ifdef XMEN2_MPW
extern s16 g_PadMaskY;
#else
#define g_PadMaskY XM_G(s16, 0xFFC654)
#endif
/* $FFC656 word: pad bit for button X ($400, Game_InitButtonMasks) */
#ifdef XMEN2_MPW
extern s16 g_PadMaskX;
#else
#define g_PadMaskX XM_G(s16, 0xFFC656)
#endif
/* $FFC658 word: pad bit for button A ($40, Game_InitButtonMasks) */
#ifdef XMEN2_MPW
extern s16 g_PadMaskA;
#else
#define g_PadMaskA XM_G(s16, 0xFFC658)
#endif
/* $FFC65A word: pad bit for button B ($10, Game_InitButtonMasks) */
#ifdef XMEN2_MPW
extern s16 g_PadMaskB;
#else
#define g_PadMaskB XM_G(s16, 0xFFC65A)
#endif
/* $FFC65C word: pad bit for button C ($20, Game_InitButtonMasks) */
#ifdef XMEN2_MPW
extern s16 g_PadMaskC;
#else
#define g_PadMaskC XM_G(s16, 0xFFC65C)
#endif
/* $FFC65E word: Up|Down mask (Game_InitButtonMasks) */
#ifdef XMEN2_MPW
extern s16 g_PadMaskUpDown;
#else
#define g_PadMaskUpDown XM_G(s16, 0xFFC65E)
#endif
/* $FFC660 word: pad bit used for Down (2, Game_InitButtonMasks) */
#ifdef XMEN2_MPW
extern s16 g_PadMaskDown;
#else
#define g_PadMaskDown XM_G(s16, 0xFFC660)
#endif
/* $FFC662 word: pad bit used for Up (1, Game_InitButtonMasks) */
#ifdef XMEN2_MPW
extern s16 g_PadMaskUp;
#else
#define g_PadMaskUp XM_G(s16, 0xFFC662)
#endif
/* $FFC664 word: Left|Right mask (Game_InitButtonMasks) */
#ifdef XMEN2_MPW
extern s16 g_PadMaskLeftRight;
#else
#define g_PadMaskLeftRight XM_G(s16, 0xFFC664)
#endif
/* $FFC666 word: pad bit used for Right (8, Game_InitButtonMasks) */
#ifdef XMEN2_MPW
extern s16 g_PadMaskRight;
#else
#define g_PadMaskRight XM_G(s16, 0xFFC666)
#endif
/* $FFC668 word: pad bit used for Left (4, Game_InitButtonMasks; read by Platform1_SteerByPad) */
#ifdef XMEN2_MPW
extern s16 g_PadMaskLeft;
#else
#define g_PadMaskLeft XM_G(s16, 0xFFC668)
#endif
/* $FFC66A word: P2 health carried between scenes (from g_Player2+$68; $700 on a new life; 0 = out of play) */
#ifdef XMEN2_MPW
extern s16 g_P2Health;
#else
#define g_P2Health XM_G(s16, 0xFFC66A)
#endif
/* $FFC66C word: P1 health carried between scenes (copied from g_Player1+$68 after each scene in Game_SceneLoop; $700 on a new life; 0 = out of play) */
#ifdef XMEN2_MPW
extern s16 g_P1Health;
#else
#define g_P1Health XM_G(s16, 0xFFC66C)
#endif
/* $FFC66E word: scene to resume after the death player-select screen: Game_SceneLoop saves g_SceneIndex here, PlayerSelect_RunScene restores it and sets -1 (= none) */
#ifdef XMEN2_MPW
extern s16 g_ResumeSceneIndex;
#else
#define g_ResumeSceneIndex XM_G(s16, 0xFFC66E)
#endif
/* $FFC670 word: next attract demo 0-3 (Game_SceneLoop switch -> Game_AttractDemo*; incremented by Game_PlayAttractDemo) */
#ifdef XMEN2_MPW
extern s16 g_AttractDemoNum;
#else
#define g_AttractDemoNum XM_G(s16, 0xFFC670)
#endif
/* $FFC672 byte: scene id of "CerebroScreen FinalText" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnFinalText;
#else
#define g_ScnFinalText XM_G(u8, 0xFFC672)
#endif
/* $FFC674 byte: scene id of "CerebroScreen Failure" (Scene_RegisterAll); game-over screen chosen by Game_SceneLoop */
#ifdef XMEN2_MPW
extern u8 g_ScnFailure;
#else
#define g_ScnFailure XM_G(u8, 0xFFC674)
#endif
/* $FFC676 byte: scene id of "CerebroScreen LogoAndDemo" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnLogoAndDemo;
#else
#define g_ScnLogoAndDemo XM_G(u8, 0xFFC676)
#endif
/* $FFC678 byte: scene id of the last "CerebroScreen PlayerSelect" registration (Scene_RegisterAll; registered between stages); scene after a death (Game_SceneLoop) */
#ifdef XMEN2_MPW
extern u8 g_ScnPlayerSelect;
#else
#define g_ScnPlayerSelect XM_G(u8, 0xFFC678)
#endif
/* $FFC67A byte: scene id of "CerebroScreen Credits" (Scene_RegisterAll); Game_SceneLoop resets the game when g_SceneIndex passes it */
#ifdef XMEN2_MPW
extern u8 g_ScnCredits;
#else
#define g_ScnCredits XM_G(u8, 0xFFC67A)
#endif
/* $FFC67C byte: scene id of "CerebroScreen Clone" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnCerebroClone;
#else
#define g_ScnCerebroClone XM_G(u8, 0xFFC67C)
#endif
/* $FFC67E byte: scene id of "CerebroScreen Space" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnCerebroSpace;
#else
#define g_ScnCerebroSpace XM_G(u8, 0xFFC67E)
#endif
/* $FFC680 byte: scene id of "CerebroScreen Savage" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnCerebroSavage;
#else
#define g_ScnCerebroSavage XM_G(u8, 0xFFC680)
#endif
/* $FFC682 byte: scene id of "CerebroScreen BaniMaza" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnCerebroBaniMaza;
#else
#define g_ScnCerebroBaniMaza XM_G(u8, 0xFFC682)
#endif
/* $FFC684 byte: scene id of "CerebroScreen Avalon" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnCerebroAvalon;
#else
#define g_ScnCerebroAvalon XM_G(u8, 0xFFC684)
#endif
/* $FFC686 byte: scene id of "CerebroScreen Sentinels" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnCerebroSentinels;
#else
#define g_ScnCerebroSentinels XM_G(u8, 0xFFC686)
#endif
/* $FFC688 byte: scene id of "SentinelsB2 Exterior_Escape" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnSentinelsB2;
#else
#define g_ScnSentinelsB2 XM_G(u8, 0xFFC688)
#endif
/* $FFC68A byte: scene id of "SentinelsA1 CoolantCore" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnSentinelsA1;
#else
#define g_ScnSentinelsA1 XM_G(u8, 0xFFC68A)
#endif
/* $FFC68C byte: scene id of "SentinelsB1 Exterior_3" (Scene_RegisterAll); also attract demo 1 (Cyclops) */
#ifdef XMEN2_MPW
extern u8 g_ScnSentinelsB1;
#else
#define g_ScnSentinelsB1 XM_G(u8, 0xFFC68C)
#endif
/* $FFC68E byte: scene id of "SentinelsD1 Exterior1" (Scene_RegisterAll); also attract demo 3 (Beast) */
#ifdef XMEN2_MPW
extern u8 g_ScnSentinelsD1;
#else
#define g_ScnSentinelsD1 XM_G(u8, 0xFFC68E)
#endif
/* $FFC690 byte: scene id of "SiberiaB1 Blizzard" (Scene_RegisterAll); first stage of a new game (Game_ResetPlayers) */
#ifdef XMEN2_MPW
extern u8 g_ScnSiberiaB1;
#else
#define g_ScnSiberiaB1 XM_G(u8, 0xFFC690)
#endif
/* $FFC692 byte: scene id of "CloneA5 BroodBoss" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnCloneA5;
#else
#define g_ScnCloneA5 XM_G(u8, 0xFFC692)
#endif
/* $FFC694 byte: scene id of "CloneA2 Vertical_Corridor_1" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnCloneA2;
#else
#define g_ScnCloneA2 XM_G(u8, 0xFFC694)
#endif
/* $FFC696 byte: scene id of "CloneA1 Factory_Floor_1" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnCloneA1;
#else
#define g_ScnCloneA1 XM_G(u8, 0xFFC696)
#endif
/* $FFC698 byte: scene id of "SpaceA1 Space_Elevator" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnSpaceA1;
#else
#define g_ScnSpaceA1 XM_G(u8, 0xFFC698)
#endif
/* $FFC69A byte: scene id of "SpaceB1 Maintenance_Shaft" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnSpaceB1;
#else
#define g_ScnSpaceB1 XM_G(u8, 0xFFC69A)
#endif
/* $FFC69C byte: scene id of "SavageA4 Chamber" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnSavageA4;
#else
#define g_ScnSavageA4 XM_G(u8, 0xFFC69C)
#endif
/* $FFC69E byte: scene id of "SavageA3 Perimeter_2" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnSavageA3;
#else
#define g_ScnSavageA3 XM_G(u8, 0xFFC69E)
#endif
/* $FFC6A0 byte: scene id of "SavageA2 Perimeter_1" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnSavageA2;
#else
#define g_ScnSavageA2 XM_G(u8, 0xFFC6A0)
#endif
/* $FFC6A2 byte: scene id of "SavageB1 Jungle_2" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnSavageB1;
#else
#define g_ScnSavageB1 XM_G(u8, 0xFFC6A2)
#endif
/* $FFC6A4 byte: scene-id slot between g_ScnSavageC1 and g_ScnSavageB1 that Scene_RegisterAll never fills (0 throughout the TAS); FLYBROOD code takes its alternate art/animation path when g_SceneIndex equals it (FlyBrood_ClassInit, _Init, _Draw, _Act*) */
#ifdef XMEN2_MPW
extern u8 g_ScnUnused3B5C;
#else
#define g_ScnUnused3B5C XM_G(u8, 0xFFC6A4)
#endif
/* $FFC6A6 byte: scene id of "SavageC1 Jungle_1" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnSavageC1;
#else
#define g_ScnSavageC1 XM_G(u8, 0xFFC6A6)
#endif
/* $FFC6A8 byte: scene id of "BaniMazaA2 ApocalypseBoss" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnBaniMazaA2;
#else
#define g_ScnBaniMazaA2 XM_G(u8, 0xFFC6A8)
#endif
/* $FFC6AA byte: scene id of "BaniMazaA1 Moving_Blocks" (Scene_RegisterAll); also attract demo 0 (Nightcrawler) */
#ifdef XMEN2_MPW
extern u8 g_ScnBaniMazaA1;
#else
#define g_ScnBaniMazaA1 XM_G(u8, 0xFFC6AA)
#endif
/* $FFC6AC byte: scene id of "BaniMazaB2 Exterior" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnBaniMazaB2;
#else
#define g_ScnBaniMazaB2 XM_G(u8, 0xFFC6AC)
#endif
/* $FFC6AE byte: scene id of "AvalonA4 Throne_Room" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnAvalonA4;
#else
#define g_ScnAvalonA4 XM_G(u8, 0xFFC6AE)
#endif
/* $FFC6B0 byte: scene id of "AvalonA3 Exodus" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnAvalonA3;
#else
#define g_ScnAvalonA3 XM_G(u8, 0xFFC6B0)
#endif
/* $FFC6B2 byte: scene id of "AvalonA2 Falling_Room" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnAvalonA2;
#else
#define g_ScnAvalonA2 XM_G(u8, 0xFFC6B2)
#endif
/* $FFC6B4 byte: scene id of "AvalonB2 InnerShell" (Scene_RegisterAll); also attract demo 2 (Psylocke) */
#ifdef XMEN2_MPW
extern u8 g_ScnAvalonB2;
#else
#define g_ScnAvalonB2 XM_G(u8, 0xFFC6B4)
#endif
/* $FFC6B6 byte: scene id of "AvalonB1 Exterior" (Scene_RegisterAll) */
#ifdef XMEN2_MPW
extern u8 g_ScnAvalonB1;
#else
#define g_ScnAvalonB1 XM_G(u8, 0xFFC6B6)
#endif
/* $FFC6B8 word, scene-local: Cerebro palette animation direction (+1/-1), SentinelsA1 cycle counter, SentinelsB2 alarm, SiberiaB1 wind */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B48[];
#else
#define g_SceneVar3B48 XM_GA(u8, 0xFFC6B8)
#endif
/* $FFC6BA word, scene-local palette-cycle counter (SavageA4, SentinelsA1; Cerebro_Run 1) */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B46[];
#else
#define g_SceneVar3B46 XM_GA(u8, 0xFFC6BA)
#endif
/* $FFC6BC word, scene-local palette-cycle counter (CloneA1, SavageA4, SentinelsA1, SiberiaB1; Cerebro_Run 1) */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B44[];
#else
#define g_SceneVar3B44 XM_GA(u8, 0xFFC6BC)
#endif
/* $FFC6BE word, scene-local palette-cycle counter (CloneA1, SentinelsA1) */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B42[];
#else
#define g_SceneVar3B42 XM_GA(u8, 0xFFC6BE)
#endif
/* $FFC6C0 word, scene-local: SiberiaB1 wind value, Cerebro palette animation step, SentinelsA1 cycle counter, SentinelsB2 alarm */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B40[];
#else
#define g_SceneVar3B40 XM_GA(u8, 0xFFC6C0)
#endif
/* $FFC6C2 word, scene-local palette-cycle counter (SpaceB1, SentinelsA1, SavageA4 cycles, BaniMazaA2 palette animation) */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B3E[];
#else
#define g_SceneVar3B3E XM_GA(u8, 0xFFC6C2)
#endif
/* $FFC6C4 word, scene-local palette-cycle / effect counter (SavageA4/SentinelsA1/SiberiaB1/SpaceB1/CloneA1 cycles, CloneA2 flicker, SavageA2 lights, AvalonA4/BaniMazaA2 palette animation) */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B3C[];
#else
#define g_SceneVar3B3C XM_GA(u8, 0xFFC6C4)
#endif
/* $FFC6C6 word, scene-local animation counter (AvalonB1 fade step, AvalonA4/BaniMazaA2/Cerebro palette animation, CloneA1/SentinelsA1 cycles, CloneA2 flicker, SentinelsD1 light, SavageA4 effect sprite, SiberiaB1 wind sprite) */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B3A[];
#else
#define g_SceneVar3B3A XM_GA(u8, 0xFFC6C6)
#endif
/* $FFC6C8 word, scene-local counter (SentinelsB2 alarm step, Cerebro palette animation, SentinelsA1/SpaceB1 cycles, XClone collapse, BaniMazaA2 palette step) */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B38[];
#else
#define g_SceneVar3B38 XM_GA(u8, 0xFFC6C8)
#endif
/* $FFC6CA word, scene-local palette-cycle / effect counter (SpaceB1, SentinelsA1, SavageA4 cycles, XClone collapse, BaniMazaA2, Cerebro) */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B36[];
#else
#define g_SceneVar3B36 XM_GA(u8, 0xFFC6CA)
#endif
/* $FFC6CC word, scene-local palette-cycle / effect counter (SavageA4/SentinelsA1/SiberiaB1/SpaceB1/CloneA1 cycles, CloneA2 flicker, SavageA2 lights, AvalonA4/BaniMazaA2 palette animation) */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B34[];
#else
#define g_SceneVar3B34 XM_GA(u8, 0xFFC6CC)
#endif
/* $FFC6CE word, scene-local: frame timer or palette-cycle counter: AvalonB1 camera-hook wait timer, AvalonB1/SiberiaB1 frame hook counters, palette cycle counters passed by address (CloneA1/SentinelsA1_CyclePalettes), CloneA2 flicker, SentinelsD1/SavageA4 light animation, Boulder spawner */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B32[];
#else
#define g_SceneVar3B32 XM_GA(u8, 0xFFC6CE)
#endif
/* $FFC6D0 long, scene-local: player select portrait-frame object (PlayerSelect_DrawFrames/PlaceCursor), XClone current maker/balls object (XClone_LockArena, ActLeaveBalls, ActBecomeBalls) */
#ifdef XMEN2_MPW
extern PTR(Actor) g_SceneVar3B30;
#else
#define g_SceneVar3B30 XM_G(PTR(Actor), 0xFFC6D0)
#endif
/* $FFC6D4 long, scene-local: player select P2 cursor object (PlayerSelect_*, CursorInput_*); word: XClone arena camera value (XClone_Init, XClone_LockArena) */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B2C[];
#else
#define g_SceneVar3B2C XM_GA(u8, 0xFFC6D4)
#endif
/* $FFC6D6 word, scene-local: XClone arena camera value (XClone_Init, XClone_LockArena) */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B2A[];
#else
#define g_SceneVar3B2A XM_GA(u8, 0xFFC6D6)
#endif
/* $FFC6D8 long, scene-local: player select P1 cursor object (+$3C character, +$3D saved, +$40 confirmed; PlayerSelect_*, CursorInput_*), Deathbird polar parameter, SiberiaB1 tile-hook buffer */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B28[];
#else
#define g_SceneVar3B28 XM_GA(u8, 0xFFC6D8)
#endif
/* $FFC6DC long, scene-local: CloneA5 camera x lock (word; BroodBoss_Init, CloneA5_CamApply), SavageBigBio position, SiberiaB1 debris pool, Deathbird polar angle */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B24[];
#else
#define g_SceneVar3B24 XM_GA(u8, 0xFFC6DC)
#endif
/* $FFC6DE word, scene-local: CloneA5 camera y lock (BroodBoss_Init; CloneA5_CamApply), SavageBigBio speed */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B22[];
#else
#define g_SceneVar3B22 XM_GA(u8, 0xFFC6DE)
#endif
/* $FFC6E0 long, scene-local: boss object pointer (BroodBoss_Init, SavageA4_RunScene, SentinelsA1_RunScene, Deathbird_Init, Savage bio bosses), Deathbird polar radius, ApocBomb speed timer, XClone maker */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B20[];
#else
#define g_SceneVar3B20 XM_GA(u8, 0xFFC6E0)
#endif
/* $FFC6E4 long, scene-local: data pointer for Apocalypse, XClone, BroodBoss and CloneA5 resources, SavageBigBio second hit mask, Deathbird hornet schedule */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B1C[];
#else
#define g_SceneVar3B1C XM_GA(u8, 0xFFC6E4)
#endif
/* $FFC6E8 long, scene-local: CloneA5 boss-parts object on CloneA5_RunScene's stack (CloneA5_FrameHook writes the health bar into +4), SavageBigBio hit mask, SentinelsA1 zap countdown, Deathbird data block, AvalonA3 second palette pointer, BaniMazaA2 debris group */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B18[];
#else
#define g_SceneVar3B18 XM_GA(u8, 0xFFC6E8)
#endif
/* $FFC6EC long, scene-local: Cerebro text script pointer (set by each Cerebro*_RunScene, read by Cerebro_RunTextScript), Hologram current pose, SiberiaB1 wind script, Deathbird attack script, AvalonB1/AvalonA3 saved palette pointer, BroodBoss/CloneA5 debris pool, SavageA4 boss stage table, BaniMazaA2 debris buffer */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B14[];
#else
#define g_SceneVar3B14 XM_GA(u8, 0xFFC6EC)
#endif
/* $FFC6F0 word, scene-local: SiberiaB1 debris count (SiberiaB1_DebrisInit/Alloc/Free/Update), BroodClaw swing phase 0-4, Cerebro sprite tile base (Cerebro_LoadSpriteTiles, PlayerSelect_DrawFrame), LightBeam/BroodBoss values, XClone arena limit */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B10[];
#else
#define g_SceneVar3B10 XM_GA(u8, 0xFFC6F0)
#endif
/* $FFC6F2 word, scene-local: player select = number of selectable characters (PlayerSelect_Run; Prev/Next/FirstFreeChar, PlaceCursor, DrawFrames), SiberiaB1 debris sprite slot, XClone arena limit, BaniMazaA2 sprite flag */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B0E[];
#else
#define g_SceneVar3B0E XM_GA(u8, 0xFFC6F2)
#endif
/* $FFC6F4 word, scene-local: Cerebro intro palette frame counter / play-intro flag (Cerebro_IntroPalHook, Cerebro_PlayIntro, Cerebro*_RunScene), BROODBOSS state bits (1 arrived, 2 shock on, $10/$80 claw, $20, $40 hit wall; BroodBoss_*, CloneA5_*), XClone arena limit, SiberiaB1 ice debris value, BaniMazaA2 palette step */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B0C[];
#else
#define g_SceneVar3B0C XM_GA(u8, 0xFFC6F4)
#endif
/* $FFC6F6 word, scene-local: Cerebro screens = 1 to run the player select after the text (Cerebro*_RunScene, Cerebro_Run), SiberiaB1 demo timeout ($4B0 frames, SiberiaB1_InstallHooks/FrameHook), XClone arena camera limit, SentinelsA1 zap index, BaniMazaA2 palette flag */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B0A[];
#else
#define g_SceneVar3B0A XM_GA(u8, 0xFFC6F6)
#endif
/* $FFC6F8 word, scene-local: XClone arena flags (bit 0 locked, bit 1 reward given, bit 8+type = clone beaten by the same character), APOCALYPSE live-bomb count, CloneA2 quake flag, SentinelsB2 escape phase 1-3, SiberiaB1 wind gust timer, SentinelsA1 zap index, CloneA5 sprite slot */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B08[];
#else
#define g_SceneVar3B08 XM_GA(u8, 0xFFC6F8)
#endif
/* $FFC6FA word, scene-local: Cerebro text cursor row (Cerebro_PrintText, TextOpSetCursor), SiberiaB1 wind strength ($600), XClone collapse/reward timers ($A0), SavageBigBio tentacle-hit bits, Deathbird hurt-flash timer ($C), SentinelsB2 digit value, AvalonA3 timer; also Nem_Decode's row counter */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B06[];
#else
#define g_SceneVar3B06 XM_GA(u8, 0xFFC6FA)
#endif
/* $FFC6FC word, scene-local: Cerebro text cursor column (Cerebro_PrintText, TextOpSetCursor, ClearTextWindow), SiberiaB1 wind value, SentinelsA1 zap index, Teleporter fade flag, Deathbird/CloneA5 sprite-definition slot, XClone maker state; also Nem_Decode's bit counter (so only valid between decodes) */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B04[];
#else
#define g_SceneVar3B04 XM_GA(u8, 0xFFC6FC)
#endif
/* $FFC6FE word, scene-local: Cerebro text state (Cerebro_RunTextScript 2, TextOpFadeOut 3, TextOpPrint -1; Cerebro_FrameHook/PrintText), AvalonA4 camera-zone event bits (AvalonA4_EventZoneFree*, Set/ClearCameraZone), CloneA2 background palette timer, SentinelsA1 zap-light index, SentinelsB2 alarm countdown ($546), SavageA4 boss stage threshold, Deathbird burst counter ($D), BaniMazaA2 palette reload */
#ifdef XMEN2_MPW
extern u8 g_SceneVar3B02[];
#else
#define g_SceneVar3B02 XM_GA(u8, 0xFFC6FE)
#endif
/* $FFC700 word: 0 at Game_SceneLoop's start; nothing else reaches it A5-relative. Not yet named in the disassembly (-$3B00) */
#ifdef XMEN2_MPW
extern s16 g_Unk3B00;
#else
#define g_Unk3B00 XM_G(s16, 0xFFC700)
#endif
/* $FFC702 ptr: object plane B is locked to when g_BgLockToObj is set; MagnetoBoss_Init stores itself (ThronePhone_StateInit reads it as the boss) */
#ifdef XMEN2_MPW
extern PTR(void) g_BgLockObj;
#else
#define g_BgLockObj XM_G(PTR(void), 0xFFC702)
#endif
/* $FFC706 word: when non-zero an airborne player enters the slow-fall state with this max fall speed (XPlayer_StateControl, XPlayer_StartFloatFall; cleared by XPlayer_StateFloatFall/Land and Scene_PreLoadReset); AvalonA2's retracting floor sets it from g_RetractFloorDepth; CloneA2 reuses it as a $300-frame camera timer (CloneA2_StartCamAltMode, CloneA2_CamAltTimer) */
#ifdef XMEN2_MPW
extern s16 g_FloatFallSpeed;
#else
#define g_FloatFallSpeed XM_G(s16, 0xFFC706)
#endif
/* $FFC708 8 ptrs: map tile hooks indexed by a block's hook type (block word bits 11-13), entries 1-7 called as (x, y, obj, -1, -1) by Tile_TestAt/Tile_RunHooksAtPoints; filled by each *_InstallTileHooks */
#ifdef XMEN2_MPW
extern TileHookFn g_TileHooks[8];
#else
#define g_TileHooks XM_GA(TileHookFn, 0xFFC708)
#endif
/* $FFC728 word: added to g_BgScrollY when uploaded (VDP_UpdateScroll) */
#ifdef XMEN2_MPW
extern s16 g_BgScrollAdjY;
#else
#define g_BgScrollAdjY XM_G(s16, 0xFFC728)
#endif
/* $FFC72A word: added to g_BgScrollX when uploaded (VDP_UpdateScroll) */
#ifdef XMEN2_MPW
extern s16 g_BgScrollAdjX;
#else
#define g_BgScrollAdjX XM_G(s16, 0xFFC72A)
#endif
/* $FFC72C word: added to g_FgScrollY when uploaded (VDP_UpdateScroll; Scroll_ComputeParallax) */
#ifdef XMEN2_MPW
extern s16 g_FgScrollAdjY;
#else
#define g_FgScrollAdjY XM_G(s16, 0xFFC72C)
#endif
/* $FFC72E word: added to g_FgScrollX when uploaded (VDP_UpdateScroll; CloneA5_CamApply moves it by $3C0) */
#ifdef XMEN2_MPW
extern s16 g_FgScrollAdjX;
#else
#define g_FgScrollAdjX XM_G(s16, 0xFFC72E)
#endif
/* $FFC730 word: plane B map streaming y (Level_LoadMaps, Map_StreamRows) */
#ifdef XMEN2_MPW
extern s16 g_BgStreamY;
#else
#define g_BgStreamY XM_G(s16, 0xFFC730)
#endif
/* $FFC732 word: plane B map streaming x (Level_LoadMaps, Map_StreamColumns) */
#ifdef XMEN2_MPW
extern s16 g_BgStreamX;
#else
#define g_BgStreamX XM_G(s16, 0xFFC732)
#endif
/* $FFC734 word: plane A map streaming y: row edge, 8-aligned - 8 at load, + $E0/$100 (Level_LoadMaps, Map_StreamRows, Map_RedrawBlock) */
#ifdef XMEN2_MPW
extern s16 g_FgStreamY;
#else
#define g_FgStreamY XM_G(s16, 0xFFC734)
#endif
/* $FFC736 word: plane A map streaming x: pixel column the written columns reach, 32-aligned - $60 at load, steps by 8 (Level_LoadMaps, Map_StreamColumns, Map_RedrawBlock) */
#ifdef XMEN2_MPW
extern s16 g_FgStreamX;
#else
#define g_FgStreamX XM_G(s16, 0xFFC736)
#endif
/* $FFC738 word: plane B scroll y = g_BgScrollBaseY + g_CameraY * g_BgScrollFactorY >> 8, VSRAM 2 (Cam_SetPos, VDP_UpdateScroll) */
#ifdef XMEN2_MPW
extern s16 g_BgScrollY;
#else
#define g_BgScrollY XM_G(s16, 0xFFC738)
#endif
/* $FFC73A word: plane B scroll x = g_CameraX * g_BgScrollFactorX >> 8 (+ offsets), H-scroll $F402 (Cam_SetPos, VDP_UpdateScroll) */
#ifdef XMEN2_MPW
extern s16 g_BgScrollX;
#else
#define g_BgScrollX XM_G(s16, 0xFFC73A)
#endif
/* $FFC73C word: plane A scroll y (VSRAM 0) (Cam_SetPos, Level_LoadMaps, VDP_UpdateScroll) */
#ifdef XMEN2_MPW
extern s16 g_FgScrollY;
#else
#define g_FgScrollY XM_G(s16, 0xFFC73C)
#endif
/* $FFC73E word: plane A scroll x (= g_CameraX via factor/base; written to H-scroll $F400 as -x) (Cam_SetPos, Level_LoadMaps, VDP_UpdateScroll, scene CamApply hooks) */
#ifdef XMEN2_MPW
extern s16 g_FgScrollX;
#else
#define g_FgScrollX XM_G(s16, 0xFFC73E)
#endif
/* $FFC740 byte: force Cam_SetPos to recompute scroll and redraw even without camera movement (row cursor += $100); cleared after use (AvalonB1_ScrollCameraTo, SiberiaB1_CamApply) */
#ifdef XMEN2_MPW
extern u8 g_ScrollResync;
#else
#define g_ScrollResync XM_G(u8, 0xFFC740)
#endif
/* $FFC742 word: plane B y offset for g_BgLockToObj / g_BgOffsetOn (SiberiaB1_StepWind; Cam_SetPos) */
#ifdef XMEN2_MPW
extern s16 g_BgOffsetY;
#else
#define g_BgOffsetY XM_G(s16, 0xFFC742)
#endif
/* $FFC744 word: plane B x offset for g_BgLockToObj / g_BgOffsetOn (SiberiaB1_StepWind writes the wind drift; Cam_SetPos) */
#ifdef XMEN2_MPW
extern s16 g_BgOffsetX;
#else
#define g_BgOffsetX XM_G(s16, 0xFFC744)
#endif
/* $FFC746 byte: plane B follows the object g_BgLockObj (BG pos = g_BgOffset - (object - camera)) instead of the parallax factors (SiberiaB1_RunScene sets; Cam_SetPos, VDP_UpdateScroll) */
#ifdef XMEN2_MPW
extern u8 g_BgLockToObj;
#else
#define g_BgLockToObj XM_G(u8, 0xFFC746)
#endif
/* $FFC748 byte: add g_BgOffsetX/Y to the computed plane B position (SiberiaB1_InitWind sets; Cam_SetPos, Cam_SetPosScaled) */
#ifdef XMEN2_MPW
extern u8 g_BgOffsetOn;
#else
#define g_BgOffsetOn XM_G(u8, 0xFFC748)
#endif
/* $FFC74A byte: wrap the plane B y scroll to 256 px (Level_LoadMaps arg; stages clear it; Cam_SetPos, streaming) */
#ifdef XMEN2_MPW
extern u8 g_BgWrapY;
#else
#define g_BgWrapY XM_G(u8, 0xFFC74A)
#endif
/* $FFC74C byte: wrap the plane B x scroll to 512 px (Level_LoadMaps arg; BaniMazaA1/SavageC1/SavageB1_LoadLevel set 1, SpaceA1 clears; Cam_SetPos, streaming) */
#ifdef XMEN2_MPW
extern u8 g_BgWrapX;
#else
#define g_BgWrapX XM_G(u8, 0xFFC74C)
#endif
/* $FFC74E byte: when set the streaming routines skip writing map columns/rows (Map_StreamRows, Map_StreamColumns); only Scroll_Init writes it (clear) */
#ifdef XMEN2_MPW
extern u8 g_MapDrawOff;
#else
#define g_MapDrawOff XM_G(u8, 0xFFC74E)
#endif
/* $FFC750 word: first VRAM tile after the BG map tiles (Level_LoadMaps; BaniMazaA1_InitSprites, CloneA5_InitShockSprites, SavageA2_InitLights) */
#ifdef XMEN2_MPW
extern s16 g_BgTilesEnd;
#else
#define g_BgTilesEnd XM_G(s16, 0xFFC750)
#endif
/* $FFC752 word: first VRAM tile after the FG map tiles (Level_LoadMaps); scene effect sprites put their tiles here (AvalonB2/AvalonA3/CloneA1/SentinelsD1/SiberiaB1 InitSprite(s)) */
#ifdef XMEN2_MPW
extern s16 g_FgTilesEnd;
#else
#define g_FgTilesEnd XM_G(s16, 0xFFC752)
#endif
/* $FFC754 word: block count returned by Level_CompactUsedItems in the last Level_LoadUsedTiles call (Level_LoadMaps copies it to g_FgBlockCount/g_BgBlockCount) */
#ifdef XMEN2_MPW
extern s16 g_UsedBlockCount;
#else
#define g_UsedBlockCount XM_G(s16, 0xFFC754)
#endif
/* $FFC756 word: BG blocks kept by Level_LoadUsedTiles (copied from g_UsedBlockCount; BaniMazaA1_InitSprites) */
#ifdef XMEN2_MPW
extern s16 g_BgBlockCount;
#else
#define g_BgBlockCount XM_G(s16, 0xFFC756)
#endif
/* $FFC758 word: FG blocks kept by Level_LoadUsedTiles (copied from g_UsedBlockCount); a scene's extra blocks start at this id (Exodus hot floor tiles, CloneA1/SentinelsA1/SiberiaB1 sprite rows) */
#ifdef XMEN2_MPW
extern s16 g_FgBlockCount;
#else
#define g_FgBlockCount XM_G(s16, 0xFFC758)
#endif
/* $FFC75A ptr: FG block definitions, 4x4 tile words per block (Level_LoadMaps, Enigma-decoded, tag Str_MapBufTags); scene sprites copy rows from it (CloneA1_InitSprites, SentinelsA1_InitZapLights) */
#ifdef XMEN2_MPW
extern PTR(void) g_FgBlockDefs;
#else
#define g_FgBlockDefs XM_G(PTR(void), 0xFFC75A)
#endif
/* $FFC762 word: first free VRAM tile after all map tiles (Level_LoadMaps; Scroll_Init 1); Level_Load copies it to g_VramTileCursor */
#ifdef XMEN2_MPW
extern s16 g_MapTilesEnd;
#else
#define g_MapTilesEnd XM_G(s16, 0xFFC762)
#endif
/* $FFC764 word: target for g_CamMinY, eased by VDP_UpdateScroll (AvalonA4_CamHook, CloneA2_SetCameraZone) */
#ifdef XMEN2_MPW
extern s16 g_CamMinYTarget;
#else
#define g_CamMinYTarget XM_G(s16, 0xFFC764)
#endif
/* $FFC766 word: target for g_CamMaxY, eased by VDP_UpdateScroll (Level_LoadMaps: (map height - 28 cells) * 8; AvalonA2_RaiseCameraFloor, scene zones) */
#ifdef XMEN2_MPW
extern s16 g_CamMaxYTarget;
#else
#define g_CamMaxYTarget XM_G(s16, 0xFFC766)
#endif
/* $FFC768 word: target for g_CamMinX, eased by VDP_UpdateScroll (CloneA2_SetCameraZone, Cam_SetMinXBlocks) */
#ifdef XMEN2_MPW
extern s16 g_CamMinXTarget;
#else
#define g_CamMinXTarget XM_G(s16, 0xFFC768)
#endif
/* $FFC76A word: target for g_CamMaxX, which VDP_UpdateScroll eases toward it 1 px per frame (Level_LoadMaps: (map width - 40 cells) * 8; CloneA2_SetCameraZone and scene camera zones) */
#ifdef XMEN2_MPW
extern s16 g_CamMaxXTarget;
#else
#define g_CamMaxXTarget XM_G(s16, 0xFFC76A)
#endif
/* $FFC76C word: camera y lower clamp (Cam_SetPos, Cam_TrackPlayer) */
#ifdef XMEN2_MPW
extern s16 g_CamMinY;
#else
#define g_CamMinY XM_G(s16, 0xFFC76C)
#endif
/* $FFC76E word: camera y upper clamp (Cam_SetPos, Cam_TrackPlayer) */
#ifdef XMEN2_MPW
extern s16 g_CamMaxY;
#else
#define g_CamMaxY XM_G(s16, 0xFFC76E)
#endif
/* $FFC770 word: camera x lower clamp (Cam_SetPos, Cam_TrackPlayer; set by scene camera zones) */
#ifdef XMEN2_MPW
extern s16 g_CamMinX;
#else
#define g_CamMinX XM_G(s16, 0xFFC770)
#endif
/* $FFC772 word: camera x upper clamp (Cam_SetPos, Cam_TrackPlayer) */
#ifdef XMEN2_MPW
extern s16 g_CamMaxX;
#else
#define g_CamMaxX XM_G(s16, 0xFFC772)
#endif
/* $FFC774 ptr: BG block definitions, 4x4 tile words per block (Level_LoadMaps; shares g_FgBlockDefs when FG and BG use the same tileset) */
#ifdef XMEN2_MPW
extern PTR(void) g_BgBlockDefs;
#else
#define g_BgBlockDefs XM_G(PTR(void), 0xFFC774)
#endif
/* $FFC778 ptr: BG block layout, same word format as g_FgBlockMap (Level_LoadMaps) */
#ifdef XMEN2_MPW
extern PTR(s16) g_BgBlockMap;
#else
#define g_BgBlockMap XM_G(PTR(s16), 0xFFC778)
#endif
/* $FFC77C ptr: FG block layout, one word per 32-px block (bits 0-8 block id, $3800 tile-hook type 1-7, $4000 solid), Enigma-decoded by Level_LoadMaps; read and rewritten by tile hooks */
#ifdef XMEN2_MPW
extern PTR(s16) g_FgBlockMap;
#else
#define g_FgBlockMap XM_G(PTR(s16), 0xFFC77C)
#endif
/* $FFC784 word: BG map height in 32-px blocks (Level_LoadMaps) */
#ifdef XMEN2_MPW
extern s16 g_BgMapH;
#else
#define g_BgMapH XM_G(s16, 0xFFC784)
#endif
/* $FFC786 word: BG map width in 32-px blocks (Level_LoadMaps); < 16 sets g_BgNarrow */
#ifdef XMEN2_MPW
extern s16 g_BgMapW;
#else
#define g_BgMapW XM_G(s16, 0xFFC786)
#endif
/* $FFC788 word: FG map height in 32-px blocks (Level_LoadMaps: map +4 >> 2) */
#ifdef XMEN2_MPW
extern s16 g_FgMapH;
#else
#define g_FgMapH XM_G(s16, 0xFFC788)
#endif
/* $FFC78A ptr: BG row offset table, one word per block row (Level_LoadMaps, tag Str_RowTableTags); index into g_BgBlockMap */
#ifdef XMEN2_MPW
extern PTR(s16) g_BgRowTable;
#else
#define g_BgRowTable XM_G(PTR(s16), 0xFFC78A)
#endif
/* $FFC78E ptr: FG row offset table, one word per block row = row * g_FgMapW (Level_LoadMaps, heap); index into g_FgBlockMap (tile hooks, Tile_TestAt, Map_SetBlockFG) */
#ifdef XMEN2_MPW
extern PTR(s16) g_FgRowTable;
#else
#define g_FgRowTable XM_G(PTR(s16), 0xFFC78E)
#endif
/* $FFC792 word: FG map width in 32-px blocks (Level_LoadMaps: map +2 >> 2); bounds for tile queries (Tile_TestAt, bullets, Launcher_Draw) */
#ifdef XMEN2_MPW
extern s16 g_FgMapW;
#else
#define g_FgMapW XM_G(s16, 0xFFC792)
#endif
/* $FFC794 word: tile-attribute base for plane B map words: first BG tile, +$8000 priority, +$4000 palette line (Level_LoadMaps; Map_PutRowBlockB/ECB2/F036) */
#ifdef XMEN2_MPW
extern s16 g_BgTileAttr;
#else
#define g_BgTileAttr XM_G(s16, 0xFFC794)
#endif
/* $FFC796 word: tile-attribute base ORed into plane A map words: first map tile, $8000 when the FG has priority (Level_LoadMaps; Map_PutRowBlockA/EC0E/EF90, Map_RedrawBlock) */
#ifdef XMEN2_MPW
extern s16 g_FgTileAttr;
#else
#define g_FgTileAttr XM_G(s16, 0xFFC796)
#endif
/* $FFC79C byte: a BG (plane B) map is loaded (Level_LoadMaps); copied into g_BgScrollDirty by Cam_SetPos and the scene CamApply hooks; tested by the streaming routines */
#ifdef XMEN2_MPW
extern u8 g_BgMapPresent;
#else
#define g_BgMapPresent XM_G(u8, 0xFFC79C)
#endif
/* $FFC79E byte: a FG (plane A) map was given to Level_LoadMaps (Scroll_Init clears) */
#ifdef XMEN2_MPW
extern u8 g_FgMapPresent;
#else
#define g_FgMapPresent XM_G(u8, 0xFFC79E)
#endif
/* $FFC7A0 word: (FG map height in cells - 32) * 8 (Level_LoadMaps; row streaming Map_StreamRows) */
#ifdef XMEN2_MPW
extern s16 g_FgMapExtraH;
#else
#define g_FgMapExtraH XM_G(s16, 0xFFC7A0)
#endif
/* $FFC7A2 word: (FG map width in cells - 64) * 8 = pixels beyond one 64-cell plane (Level_LoadMaps; column streaming Map_StreamColumns) */
#ifdef XMEN2_MPW
extern s16 g_FgMapExtraW;
#else
#define g_FgMapExtraW XM_G(s16, 0xFFC7A2)
#endif
/* $FFC7A4 byte: request a VSRAM upload of both plane y scrolls (Scroll_RequestVsram sets, VDP_UpdateScroll clears) */
#ifdef XMEN2_MPW
extern u8 g_VsramReq;
#else
#define g_VsramReq XM_G(u8, 0xFFC7A4)
#endif
/* $FFC7A6 byte: request a full H-scroll + 40-column VSRAM upload (Scroll_RequestColumnVs sets, VDP_UpdateScroll clears) */
#ifdef XMEN2_MPW
extern u8 g_ColumnVsReq;
#else
#define g_ColumnVsReq XM_G(u8, 0xFFC7A6)
#endif
/* $FFC7A8 byte: upload plane B H/V scroll at the next V-blank (Cam_SetPos copies g_BgMapPresent; VDP_UpdateScroll clears) */
#ifdef XMEN2_MPW
extern u8 g_BgScrollDirty;
#else
#define g_BgScrollDirty XM_G(u8, 0xFFC7A8)
#endif
/* $FFC7AA byte: upload plane A H/V scroll at the next V-blank (set by Cam_SetPos and scene CamApply hooks; VDP_UpdateScroll clears) */
#ifdef XMEN2_MPW
extern u8 g_FgScrollDirty;
#else
#define g_FgScrollDirty XM_G(u8, 0xFFC7AA)
#endif
/* $FFC7AC ptr: map column streaming routine, Map_StreamColumns by default (Level_LoadMaps); called by Cam_SetPos/Cam_SetPosScaled before g_MapRowHook */
#ifdef XMEN2_MPW
extern FrameHookFn g_MapColHook;
#else
#define g_MapColHook XM_G(FrameHookFn, 0xFFC7AC)
#endif
/* $FFC7B0 ptr: map row streaming routine, Map_StreamRows by default (Level_LoadMaps); called by Cam_SetPos/Cam_SetPosScaled after a camera move and once by Level_LoadMaps to draw the first screen; scenes' CamApply hooks call it too */
#ifdef XMEN2_MPW
extern FrameHookFn g_MapRowHook;
#else
#define g_MapRowHook XM_G(FrameHookFn, 0xFFC7B0)
#endif
/* $FFC7B4 ptr: per-frame palette hook called by Level_FrameLoop after g_LevelScrollHook; Level_Load sets Pal_RunCycleScript (g_PalCycleScript player) */
#ifdef XMEN2_MPW
extern PTR(void) g_LevelPalHook;
#else
#define g_LevelPalHook XM_G(PTR(void), 0xFFC7B4)
#endif
/* $FFC7B8 ptr: per-frame scroll update called by Level_FrameLoop; Level_Load sets Scroll_UpdateParallax, BaniMazaA1_InitScroll sets RollingDiscs_UpdateScroll */
#ifdef XMEN2_MPW
extern PTR(void) g_LevelScrollHook;
#else
#define g_LevelScrollHook XM_G(PTR(void), 0xFFC7B8)
#endif
/* $FFC7BC ptr: current parallax descriptor (Scroll_SetParallax, Scroll_UpdateParallax) */
#ifdef XMEN2_MPW
extern PTR(ParallaxDesc) g_ParallaxDesc;
#else
#define g_ParallaxDesc XM_G(PTR(ParallaxDesc), 0xFFC7BC)
#endif
/* $FFC7C0 7 bytes: the button mask of actions 0-6 in the control layout (Game_Main fills them from Pad_ConfigButtonMask). Not yet named in the disassembly (-$3A40) */
#ifdef XMEN2_MPW
extern s8 g_ActionButtonMasks[7];
#else
#define g_ActionButtonMasks XM_GA(s8, 0xFFC7C0)
#endif
/* $FFC7C8 word: P2 buttons held this frame (Pad_Update) */
#ifdef XMEN2_MPW
extern s16 g_Pad2Held;
#else
#define g_Pad2Held XM_G(s16, 0xFFC7C8)
#endif
/* $FFC7CA word: P1 buttons held this frame (Pad_Update) */
#ifdef XMEN2_MPW
extern s16 g_Pad1Held;
#else
#define g_Pad1Held XM_G(s16, 0xFFC7CA)
#endif
/* $FFC7CC word: P2 buttons OR-accumulated since last cleared (Pad_Update) */
#ifdef XMEN2_MPW
extern s16 g_Pad2Accum;
#else
#define g_Pad2Accum XM_G(s16, 0xFFC7CC)
#endif
/* $FFC7CE word: P1 buttons OR-accumulated since last cleared (Pad_Update) */
#ifdef XMEN2_MPW
extern s16 g_Pad1Accum;
#else
#define g_Pad1Accum XM_G(s16, 0xFFC7CE)
#endif
/* $FFC7D0 word: P2 presses latched while held (Pad_Update) */
#ifdef XMEN2_MPW
extern s16 g_Pad2Latch;
#else
#define g_Pad2Latch XM_G(s16, 0xFFC7D0)
#endif
/* $FFC7D2 word: P1 presses latched while held; game code clears bits to consume them (Pad_Update, Pause_Check) */
#ifdef XMEN2_MPW
extern s16 g_Pad1Latch;
#else
#define g_Pad1Latch XM_G(s16, 0xFFC7D2)
#endif
/* $FFC7D4 word: P2 buttons released this frame (Pad_Update) */
#ifdef XMEN2_MPW
extern s16 g_Pad2Released;
#else
#define g_Pad2Released XM_G(s16, 0xFFC7D4)
#endif
/* $FFC7D6 word: P1 buttons released this frame (Pad_Update) */
#ifdef XMEN2_MPW
extern s16 g_Pad1Released;
#else
#define g_Pad1Released XM_G(s16, 0xFFC7D6)
#endif
/* $FFC7D8 word: P2 buttons newly pressed this frame (Pad_Update) */
#ifdef XMEN2_MPW
extern s16 g_Pad2Pressed;
#else
#define g_Pad2Pressed XM_G(s16, 0xFFC7D8)
#endif
/* $FFC7DA word: P1 buttons newly pressed this frame (Pad_Update; pause tests $80 Start) */
#ifdef XMEN2_MPW
extern s16 g_Pad1Pressed;
#else
#define g_Pad1Pressed XM_G(s16, 0xFFC7DA)
#endif
/* $FFC7DC word: level frame counter (+1 per Level_FrameLoop frame, cleared by Level_Run; bit 0 = blink phase in Actor_Draw; demo timing) */
#ifdef XMEN2_MPW
extern s16 g_LevelFrame;
#else
#define g_LevelFrame XM_G(s16, 0xFFC7DC)
#endif
/* $FFC7DE word: g_LevelFrame at the last demo input step (Demo_ReadInput); Level_Run and Cerebro_Run clear it */
#ifdef XMEN2_MPW
extern s16 g_DemoStepFrame;
#else
#define g_DemoStepFrame XM_G(s16, 0xFFC7DE)
#endif
/* $FFC7E0 word: current demo pad value for P1 (Demo_ReadInput, Pad_Get) */
#ifdef XMEN2_MPW
extern s16 g_DemoPad;
#else
#define g_DemoPad XM_G(s16, 0xFFC7E0)
#endif
/* $FFC7E2 ptr: demo input stream of (frame, pad byte) pairs (Demo_ReadInput) */
#ifdef XMEN2_MPW
extern PTR(void) g_DemoInput;
#else
#define g_DemoInput XM_G(PTR(void), 0xFFC7E2)
#endif
/* $FFC7E6 byte: demo playback on, P1 input comes from g_DemoPad (Pad_Get; set by Game_PlayAttractDemo) */
#ifdef XMEN2_MPW
extern u8 g_DemoActive;
#else
#define g_DemoActive XM_G(u8, 0xFFC7E6)
#endif
/* $FFC7E8 ptr: game function called from Irq_VBlank unless bytes -$38C0/-$359E are set (Timer_SetVBlankHookCb, Cerebro_PlayIntro) */
#ifdef XMEN2_MPW
extern PTR(void) g_VBlankGameHook;
#else
#define g_VBlankGameHook XM_G(PTR(void), 0xFFC7E8)
#endif
/* $FFC7EC ptr: function called from Irq_VBlank every V-int when non-zero (Sys_InitHardware clears; Intro_SegaLogoFlash saves/sets) */
#ifdef XMEN2_MPW
extern PTR(void) g_VBlankHook;
#else
#define g_VBlankHook XM_G(PTR(void), 0xFFC7EC)
#endif
/* $FFC7F0 long: V-interrupt counter (+1 in Irq_VBlank; zeroed in Sys_InitHardware) */
#ifdef XMEN2_MPW
extern s32 g_VBlankCount;
#else
#define g_VBlankCount XM_G(s32, 0xFFC7F0)
#endif
/* $FFC7F4 byte: VDP_FrameUpdate ran this frame (VDP_WaitVBlank clears) */
#ifdef XMEN2_MPW
extern u8 g_FrameUpdated;
#else
#define g_FrameUpdated XM_G(u8, 0xFFC7F4)
#endif
/* $FFC7F6 byte: VDP_FrameUpdate ran in this V-blank (Irq_VBlank clears it first); the V-blank hooks Hologram_VBlankFlicker and Cyclops_BeamVBlankFlicker act only when set */
#ifdef XMEN2_MPW
extern u8 g_VBlankFrameDone;
#else
#define g_VBlankFrameDone XM_G(u8, 0xFFC7F6)
#endif
/* $FFC7F8 byte: set by VDP_FrameUpdate every V-blank; the level loops clear it at frame start and in two-player half-rate mode spin until it is set again (Level_FrameLoop, PlayerSelect_Run, Cerebro_PlayIntro) */
#ifdef XMEN2_MPW
extern u8 g_FrameTick;
#else
#define g_FrameTick XM_G(u8, 0xFFC7F8)
#endif
/* $FFC7FA byte: upload g_PalBase at the next V-blank (Pal_Update clears) */
#ifdef XMEN2_MPW
extern u8 g_PalBaseDirty;
#else
#define g_PalBaseDirty XM_G(u8, 0xFFC7FA)
#endif
/* $FFC7FC word: frames between fade steps (Pal_StartFade sets 4) */
#ifdef XMEN2_MPW
extern s16 g_PalFadeDelay;
#else
#define g_PalFadeDelay XM_G(s16, 0xFFC7FC)
#endif
/* $FFC7FE byte: palette fade in progress (Pal_StartFade, Pal_Update) */
#ifdef XMEN2_MPW
extern u8 g_PalFadeActive;
#else
#define g_PalFadeActive XM_G(u8, 0xFFC7FE)
#endif
/* $FFC800 64 words: working palette uploaded to CRAM (Pal_Update, Pal_ApplyFade) */
#ifdef XMEN2_MPW
extern s16 g_PalWork[64];
#else
#define g_PalWork XM_GA(s16, 0xFFC800)
#endif
/* $FFC880 64 words: base palette that fades are computed from (Pal_ApplyFade) */
#ifdef XMEN2_MPW
extern s16 g_PalBase[64];
#else
#define g_PalBase XM_GA(s16, 0xFFC880)
#endif
/* $FFC900 ptr: current level class list {word bytes; desc*[]} (ClassList_CallSlot5A/62, ClassList_InitAll) */
#ifdef XMEN2_MPW
extern PTR(void) g_ClassList;
#else
#define g_ClassList XM_G(PTR(void), 0xFFC900)
#endif
/* $FFC904 ptr: called by the scene loop (Level_FrameLoop) when a player presses Start; Level_Load sets jt[$3FB2] = Pause_Check (pause); Teleporter_StateTeleport swaps in Teleporter_NoPause */
#ifdef XMEN2_MPW
extern PTR(void) g_PauseHandler;
#else
#define g_PauseHandler XM_G(PTR(void), 0xFFC904)
#endif
/* $FFC90C ptr: per-frame scene draw hook called by the level loop (Level_FrameLoop) before Spr_EndFrame; Level_Load defaults it to jt[$4F82]; scenes swap in their own (Water_DrawHook, BaniMazaA2_DrawHook) which end with DrawList_DrawAll */
#ifdef XMEN2_MPW
extern PTR(void) g_SceneDrawHook;
#else
#define g_SceneDrawHook XM_G(PTR(void), 0xFFC90C)
#endif
/* $FFC910 ptr: replaces the scene loop's object-activation pass when set (Level_FrameLoop, Scene_ClearActivateHook) */
#ifdef XMEN2_MPW
extern PTR(void) g_ActivateHook;
#else
#define g_ActivateHook XM_G(PTR(void), 0xFFC910)
#endif
/* $FFC914 ptr: called last by Level_Load; Scene_PostLoadVram by default, SiberiaB1_PostLoad for the first stage */
#ifdef XMEN2_MPW
extern PTR(void) g_ScenePostLoadHook;
#else
#define g_ScenePostLoadHook XM_G(PTR(void), 0xFFC914)
#endif
/* $FFC918 ptr: called first by Level_Load; Scene_PreLoadReset by default (Game_SceneLoop, Game_PlayAttractDemo), SiberiaB1_PreLoad for the first stage */
#ifdef XMEN2_MPW
extern PTR(void) g_ScenePreLoadHook;
#else
#define g_ScenePreLoadHook XM_G(PTR(void), 0xFFC918)
#endif
/* $FFC91C byte: when set the level loops skip spawning, updates, timers and scroll and never end (Level_FrameLoop, PlayerSelect_Run); no setter found (debug?) */
#ifdef XMEN2_MPW
extern u8 g_LevelFreeze;
#else
#define g_LevelFreeze XM_G(u8, 0xFFC91C)
#endif
/* $FFC91E byte: the g_PlayersAlive the HUD was drawn for: HUD_UpdateHealthBars redraws both panels (HUD_Redraw) when it changes. Not yet named in the disassembly (-$38E2) */
#ifdef XMEN2_MPW
extern u8 g_HudPlayersAlive;
#else
#define g_HudPlayersAlive XM_G(u8, 0xFFC91E)
#endif
/* $FFC920 byte: number of present+alive players, recounted every frame by Level_FrameLoop (camera switches 1P/2P tracking on it) */
#ifdef XMEN2_MPW
extern u8 g_PlayersAlive;
#else
#define g_PlayersAlive XM_G(u8, 0xFFC920)
#endif
/* $FFC922 byte: 1 when both players are alive (Level_FrameLoop); Actor_Update then steps objects by 2 and g_FrameCounter adds 2 */
#ifdef XMEN2_MPW
extern u8 g_TwoPlayerStep;
#else
#define g_TwoPlayerStep XM_G(u8, 0xFFC922)
#endif
/* $FFC924 ptr: spawn records, 14 bytes each (Spawn_InitRecords fills them via InitSpawnRec; level loop Level_FrameLoop) */
#ifdef XMEN2_MPW
extern PTR(SpawnRec) g_SpawnRecs;
#else
#define g_SpawnRecs XM_G(PTR(SpawnRec), 0xFFC924)
#endif
/* $FFC928 word: number of spawn records / placement entries (Level_Load, Spawn_InitRecords) */
#ifdef XMEN2_MPW
extern s16 g_SpawnRecCount;
#else
#define g_SpawnRecCount XM_G(s16, 0xFFC928)
#endif
/* $FFC92A ptr: level placement entries, 16 bytes, word 0 = class index into g_ClassList (Level_Load, Spawn_InitRecords) */
#ifdef XMEN2_MPW
extern PTR(Placement) g_PlacementList;
#else
#define g_PlacementList XM_G(PTR(Placement), 0xFFC92A)
#endif
/* $FFC92E word: current music track id, -1 = none (Music_Play) */
#ifdef XMEN2_MPW
extern s16 g_MusicTrack;
#else
#define g_MusicTrack XM_G(s16, 0xFFC92E)
#endif
/* $FFC930 word: frames until the next g_PalCycleScript step (Pal_RunCycleScript; Level_Load sets -1) */
#ifdef XMEN2_MPW
extern s16 g_PalCycleDelay;
#else
#define g_PalCycleDelay XM_G(s16, 0xFFC930)
#endif
/* $FFC932 ptr: next word of g_PalCycleScript (Pal_RunCycleScript; Level_Load clears) */
#ifdef XMEN2_MPW
extern PTR(void) g_PalCycleCursor;
#else
#define g_PalCycleCursor XM_G(PTR(void), 0xFFC932)
#endif
/* $FFC936 ptr: palette-cycle script run by Pal_RunCycleScript each level frame {byte ?, byte delay, byte first colour, byte last colour; then colour words, -1 = loop, -2 = end, >= $1000 = delay & $3FF}; 0 = none (Level_Load clears) */
#ifdef XMEN2_MPW
extern PTR(void) g_PalCycleScript;
#else
#define g_PalCycleScript XM_G(PTR(void), 0xFFC936)
#endif
/* $FFC93A byte: a demo was ended with Start (Level_FrameLoop); Game_PlayAttractDemo then sets progress flag 2; cleared by Level_Run, Cerebro_RunTextScript */
#ifdef XMEN2_MPW
extern u8 g_DemoSkipped;
#else
#define g_DemoSkipped XM_G(u8, 0xFFC93A)
#endif
/* $FFC93C byte: alternate floor-segment handling in Actor_Move and Seg_ComputeLength when set; only cleared (Game_SceneLoop), no setter found, meaning unclear */
#ifdef XMEN2_MPW
extern u8 g_SegFlag38C4;
#else
#define g_SegFlag38C4 XM_G(u8, 0xFFC93C)
#endif
/* $FFC93E byte: g_SceneIndex was set explicitly (Game_ResetPlayers, LogoAndDemo_RunScene, PlayerSelect_RunScene resume, pause scene skips, SiberiaB1 demo timeout): Game_SceneLoop skips its advance/death handling, Level_FrameLoop stops; cleared by Level_Run, Cerebro_Run, Game_PlayAttractDemo */
#ifdef XMEN2_MPW
extern u8 g_SceneIndexSet;
#else
#define g_SceneIndexSet XM_G(u8, 0xFFC93E)
#endif
/* $FFC942 byte: set while a frame loop runs its classes (Level_Run and PlayerSelect_Run around ClassList_InitAll..ClassList_CallSlot62; Game_Main clears); Player_SetCharacter places a joining player differently when set */
#ifdef XMEN2_MPW
extern u8 g_InLevelLoop;
#else
#define g_InLevelLoop XM_G(u8, 0xFFC942)
#endif
/* $FFC944 word: SavageA2 flood water line y in world pixels (SavageA2_InitFlood, raised by SavageA2_CamUpdateFlood from g_FloodY88); bubbles surface at it (BubbleManager_*, FloatBubble_Move) */
#ifdef XMEN2_MPW
extern s16 g_FloodLineY;
#else
#define g_FloodLineY XM_G(s16, 0xFFC944)
#endif
/* $FFC946 byte: Actor_Move halves velocity steps while set (slow motion); Level_Load clears it, no setter found */
#ifdef XMEN2_MPW
extern u8 g_HalfSpeed;
#else
#define g_HalfSpeed XM_G(u8, 0xFFC946)
#endif
/* $FFC94A byte: when set (or during a demo) P1 Start ends the scene (Level_FrameLoop -> Scene_EndAfter(1)); Game_SceneLoop clears it, no setter found */
#ifdef XMEN2_MPW
extern u8 g_StartEndsScene;
#else
#define g_StartEndsScene XM_G(u8, 0xFFC94A)
#endif
/* $FFC94C byte: the HUD is shown: Level_Load and the stage loaders set 1, Cerebro_Run clears it; Level_FrameLoop updates and Level_Run redraws the HUD only while set */
#ifdef XMEN2_MPW
extern u8 g_HudOn;
#else
#define g_HudOn XM_G(u8, 0xFFC94C)
#endif
/* $FFC94E byte: 1 = redraw the HUD (set by power-ups and health changes, e.g. PowerUp_Collect, XPlayer_AddHealth; the level loop calls HUD_UpdateHealthBars while set, which clears it) */
#ifdef XMEN2_MPW
extern u8 g_HudDirty;
#else
#define g_HudDirty XM_G(u8, 0xFFC94E)
#endif
/* $FFC950 long: 0 at Game_SceneLoop's start; nothing else reaches it A5-relative. Not yet named in the disassembly (-$38B0) */
#ifdef XMEN2_MPW
extern s32 g_Unk38B0;
#else
#define g_Unk38B0 XM_G(s32, 0xFFC950)
#endif
/* $FFC954 word: 3 at Game_SceneLoop's start (next to the lives words); nothing else reaches it A5-relative. Not yet named in the disassembly (-$38AC) */
#ifdef XMEN2_MPW
extern s16 g_Unk38AC;
#else
#define g_Unk38AC XM_G(s16, 0xFFC954)
#endif
/* $FFC956 word: player 2 lives (9 at new game; decremented on each new life) */
#ifdef XMEN2_MPW
extern s16 g_P2Lives;
#else
#define g_P2Lives XM_G(s16, 0xFFC956)
#endif
/* $FFC958 word: player 1 lives (9 at new game; decremented on each new life) */
#ifdef XMEN2_MPW
extern s16 g_P1Lives;
#else
#define g_P1Lives XM_G(s16, 0xFFC958)
#endif
/* $FFC95A word: control-layout row for Pad_ConfigButtonMask's 8-byte action table (unk_Boot_005458 sets 2) */
#ifdef XMEN2_MPW
extern s16 g_PadLayout;
#else
#define g_PadLayout XM_G(s16, 0xFFC95A)
#endif
/* $FFC95C ptr: camera shake script being played, 0 = none: byte pairs (dx, dy) added to g_CameraX/Y per frame; $64 end, $65 n = back n bytes, $66 x = Sys_Nop1F945C(x) (CloneA5_CamApply); e.g. Tbl_CloneA2QuakeScript */
#ifdef XMEN2_MPW
extern PTR(void) g_CamShake;
#else
#define g_CamShake XM_G(PTR(void), 0xFFC95C)
#endif
/* $FFC964 word: start x minus this = initial g_CameraX (Level_LoadMaps); Level_Load sets $A0 (half the screen) */
#ifdef XMEN2_MPW
extern s16 g_StartCamOffsetX;
#else
#define g_StartCamOffsetX XM_G(s16, 0xFFC964)
#endif
/* $FFC966 word: start y minus this = initial g_CameraY (Level_LoadMaps); BaniMazaB2_LoadLevel $C0, SpaceB1_LoadLevel $BE */
#ifdef XMEN2_MPW
extern s16 g_StartCamOffsetY;
#else
#define g_StartCamOffsetY XM_G(s16, 0xFFC966)
#endif
/* $FFC968 word: set next to g_StartCamOffsetY by BaniMazaB2_LoadLevel ($B8) and SpaceB1_LoadLevel ($82); no reader found, meaning unclear */
#ifdef XMEN2_MPW
extern s16 g_LevelLoadWord3898;
#else
#define g_LevelLoadWord3898 XM_G(s16, 0xFFC968)
#endif
/* $FFC96C byte: the spawn-record pass runs only while set (Level_FrameLoop); 1 at Level_Load, cleared while an arena or boss holds the screen (XClone_LockArena_SetTheme, XClone_EndWave, CloneA5_RunScene), set again by XClone_UnlockArena, BroodBoss_StateFinish */
#ifdef XMEN2_MPW
extern u8 g_SpawnEnabled;
#else
#define g_SpawnEnabled XM_G(u8, 0xFFC96C)
#endif
/* $FFC96E byte: set when the camera changes tracking mode or target; Cam_EaseOffset absorbs the position jump and clears it */
#ifdef XMEN2_MPW
extern u8 g_CamModeSwitched;
#else
#define g_CamModeSwitched XM_G(u8, 0xFFC96E)
#endif
/* $FFC970 byte: Level_FrameLoop calls the camera's update (vtable +$12) only while set; 1 at Level_Load, Cam_DefaultUpdate/SpaceA1_CamUpdate clear it when no player is present and alive and plane B is not locked */
#ifdef XMEN2_MPW
extern u8 g_CamActive;
#else
#define g_CamActive XM_G(u8, 0xFFC970)
#endif
/* $FFC972 ptr: per-frame scene hook installed by each scene's setup (CloneA2_FrameHook, CloneA5_FrameHook) */
#ifdef XMEN2_MPW
extern FrameHookFn g_SceneFrameHook;
#else
#define g_SceneFrameHook XM_G(FrameHookFn, 0xFFC972)
#endif
/* $FFC978 word: g_VramTileCursor saved when a level starts running (Level_Run, Cerebro_Run) */
#ifdef XMEN2_MPW
extern s16 g_LevelVramStart;
#else
#define g_LevelVramStart XM_G(s16, 0xFFC978)
#endif
/* $FFC97A array of $14-byte sprite-definition records {+0 flag bits, +2 def ptr, +6 word, +8 word} (Obj_SetSpriteDef) */
#ifdef XMEN2_MPW
extern SprInst g_SpriteDefs[];
#else
#define g_SpriteDefs XM_GA(SprInst, 0xFFC97A)
#endif
/* $FFCC36 byte: solid-tile probes by projectiles and flung enemies use the BG plane map instead of the FG (passed as !flag to Tile_TestAt); SiberiaB1_InstallTileHooks sets, XClone_InstallTileHooks and Level_Load clear */
#ifdef XMEN2_MPW
extern u8 g_SolidMapIsBg;
#else
#define g_SolidMapIsBg XM_G(u8, 0xFFCC36)
#endif
/* $FFCC38 byte: the scene installed map tile hooks in g_TileHooks (every *_InstallTileHooks sets 1, Level_Load clears); XPlayer_Update only probes hook tiles when set */
#ifdef XMEN2_MPW
extern u8 g_TileHooksOn;
#else
#define g_TileHooksOn XM_G(u8, 0xFFCC38)
#endif
/* $FFCC3A word: active window bottom $13A (Actor_OutsideActiveWindow and others) */
#ifdef XMEN2_MPW
extern s16 g_DeactWinBottom;
#else
#define g_DeactWinBottom XM_G(s16, 0xFFCC3A)
#endif
/* $FFCC3C word: active window top -$32 (Actor_OutsideActiveWindow and others) */
#ifdef XMEN2_MPW
extern s16 g_DeactWinTop;
#else
#define g_DeactWinTop XM_G(s16, 0xFFCC3C)
#endif
/* $FFCC3E word: active window right $1A4 (Actor_OutsideActiveWindow and others) */
#ifdef XMEN2_MPW
extern s16 g_DeactWinRight;
#else
#define g_DeactWinRight XM_G(s16, 0xFFCC3E)
#endif
/* $FFCC40 word: objects left of this (-$64 from the camera) leave the active window (Level_Load; Actor_OutsideActiveWindow, projectiles, ICandy_Update, PowerUp_Update) */
#ifdef XMEN2_MPW
extern s16 g_DeactWinLeft;
#else
#define g_DeactWinLeft XM_G(s16, 0xFFCC40)
#endif
/* $FFCC42 word: activate window bottom $130 (Actor_InActivateWindow, Obj_InScreenWindow) */
#ifdef XMEN2_MPW
extern s16 g_ActWinBottom;
#else
#define g_ActWinBottom XM_G(s16, 0xFFCC42)
#endif
/* $FFCC44 word: activate window top -$28 (Actor_InActivateWindow, Obj_InScreenWindow) */
#ifdef XMEN2_MPW
extern s16 g_ActWinTop;
#else
#define g_ActWinTop XM_G(s16, 0xFFCC44)
#endif
/* $FFCC46 word: activate window right $19A (Actor_InActivateWindow, Obj_InScreenWindow) */
#ifdef XMEN2_MPW
extern s16 g_ActWinRight;
#else
#define g_ActWinRight XM_G(s16, 0xFFCC46)
#endif
/* $FFCC48 word: activate window left -$5A relative to the camera (Level_Load; Actor_InActivateWindow, Obj_InScreenWindow, Platform1_MemberInWindow) */
#ifdef XMEN2_MPW
extern s16 g_ActWinLeft;
#else
#define g_ActWinLeft XM_G(s16, 0xFFCC48)
#endif
/* $FFCC4A ptr: the CAMERA object (class $1F4960), created by a scene via CAMERA +$0A New(desc, per-frame hook); scenes call its +$36/+$3A (CloneA2_InitCamera) */
#ifdef XMEN2_MPW
extern PTR(Camera) g_Camera;
#else
#define g_Camera XM_G(PTR(Camera), 0xFFCC4A)
#endif
/* $FFCC4E word: out-of-view window bottom $1E0 (Actor_SpawnRecOutOfView) */
#ifdef XMEN2_MPW
extern s16 g_DespawnWinBottom;
#else
#define g_DespawnWinBottom XM_G(s16, 0xFFCC4E)
#endif
/* $FFCC50 word: out-of-view window right $240 (Actor_SpawnRecOutOfView) */
#ifdef XMEN2_MPW
extern s16 g_DespawnWinRight;
#else
#define g_DespawnWinRight XM_G(s16, 0xFFCC50)
#endif
/* $FFCC52 word: out-of-view window left -$100 (Actor_SpawnRecOutOfView) */
#ifdef XMEN2_MPW
extern s16 g_DespawnWinLeft;
#else
#define g_DespawnWinLeft XM_G(s16, 0xFFCC52)
#endif
/* $FFCC54 word: spawn records outside this window count as out of view: top -$100 (Level_Load; Actor_SpawnRecOutOfView) */
#ifdef XMEN2_MPW
extern s16 g_DespawnWinTop;
#else
#define g_DespawnWinTop XM_G(s16, 0xFFCC54)
#endif
/* $FFCC56 word: spawn-in-view window bottom ($160; Actor_SpawnRecInView) */
#ifdef XMEN2_MPW
extern s16 g_SpawnWinBottom;
#else
#define g_SpawnWinBottom XM_G(s16, 0xFFCC56)
#endif
/* $FFCC58 word: spawn-in-view window right ($1C0; Actor_SpawnRecInView) */
#ifdef XMEN2_MPW
extern s16 g_SpawnWinRight;
#else
#define g_SpawnWinRight XM_G(s16, 0xFFCC58)
#endif
/* $FFCC5A word: spawn-in-view window left, relative to g_CameraX (-$80; Actor_SpawnRecInView) */
#ifdef XMEN2_MPW
extern s16 g_SpawnWinLeft;
#else
#define g_SpawnWinLeft XM_G(s16, 0xFFCC5A)
#endif
/* $FFCC5C word: spawn-in-view window top, relative to g_CameraY (-$80, Level_Load; Actor_SpawnRecInView) */
#ifdef XMEN2_MPW
extern s16 g_SpawnWinTop;
#else
#define g_SpawnWinTop XM_G(s16, 0xFFCC5C)
#endif
/* $FFCC5E long: game frame counter (+1 per frame, +2 when -$38DE set; TAS RAM +60 per 60 frames).
   Unsigned: every shift of it in the ROM is lsr, its compares bls (SavageBio_StateFireTip). */
#ifdef XMEN2_MPW
extern u32 g_FrameCounter;
#else
#define g_FrameCounter XM_G(u32, 0xFFCC5E)
#endif
/* $FFCC64 word: current scene, index into g_SceneTable (Cerebro screens and stage parts; TAS steps 0,1,2,... per screen) */
#ifdef XMEN2_MPW
extern s16 g_SceneIndex;
#else
#define g_SceneIndex XM_G(s16, 0xFFCC64)
#endif
/* $FFCC66 word: frames until the current scene ends, 0 = not ending (Scene_EndAfter; counted down in PlayerSelect_Run) */
#ifdef XMEN2_MPW
extern s16 g_SceneExitTimer;
#else
#define g_SceneExitTimer XM_G(s16, 0xFFCC66)
#endif
/* $FFCC68 word: next free VRAM tile for preloaded sprite definitions (SprDef_Add) */
#ifdef XMEN2_MPW
extern s16 g_VramTileCursor;
#else
#define g_VramTileCursor XM_G(s16, 0xFFCC68)
#endif
/* $FFCC6A 12 x $14-byte sprite instances (object +$2E layout) for scene effects (lights, shock sprites, animated blocks), Spr_Init/Spr_Draw by the scenes' InitSprite(s)/Anim* functions */
#ifdef XMEN2_MPW
extern SprInst g_SceneSprites[12];
#else
#define g_SceneSprites XM_GA(SprInst, 0xFFCC6A)
#endif
/* $FFCD5A ptr: object pointer table, slot 0 = g_Player1, 1 = g_Player2, then level objects (Obj_AllocPool; indexed by object bytes +$3A/+$3B/+$3C) */
#ifdef XMEN2_MPW
extern PTR(PTR(Actor)) g_ObjTable;
#else
#define g_ObjTable XM_G(PTR(PTR(Actor)), 0xFFCD5A)
#endif
/* $FFCD5E word: entries used in g_SpriteDefs (SprDef_Add) */
#ifdef XMEN2_MPW
extern s16 g_SpriteDefCount;
#else
#define g_SpriteDefCount XM_G(s16, 0xFFCD5E)
#endif
/* $FFCD60 word: plane B y scroll base (SpaceB1 $7A0, SavageA4 $64; Cam_SetPos, Scroll_ComputeParallax) */
#ifdef XMEN2_MPW
extern s16 g_BgScrollBaseY;
#else
#define g_BgScrollBaseY XM_G(s16, 0xFFCD60)
#endif
/* $FFCD62 word: plane A y scroll base (SpaceB1_LoadLevel $1960; Cam_SetPos) */
#ifdef XMEN2_MPW
extern s16 g_FgScrollBaseY;
#else
#define g_FgScrollBaseY XM_G(s16, 0xFFCD62)
#endif
/* $FFCD64 word: plane B x scroll base (CloneA5_CamApply moves it; SiberiaB1_CamApply) */
#ifdef XMEN2_MPW
extern s16 g_BgScrollBaseX;
#else
#define g_BgScrollBaseX XM_G(s16, 0xFFCD64)
#endif
/* $FFCD66 word: plane A x scroll base added to the scaled camera x (Scroll_Init 0; SavageA2_CamApply) */
#ifdef XMEN2_MPW
extern s16 g_FgScrollBaseX;
#else
#define g_FgScrollBaseX XM_G(s16, 0xFFCD66)
#endif
/* $FFCD68 word: plane B y parallax factor, 8.8 (each stage's LoadLevel; Cam_SetPos) */
#ifdef XMEN2_MPW
extern s16 g_BgScrollFactorY;
#else
#define g_BgScrollFactorY XM_G(s16, 0xFFCD68)
#endif
/* $FFCD6A word: plane B x parallax factor, 8.8 (each stage's LoadLevel: $80 Sentinels, $100 Siberia, $140 SavageA2...; Cam_SetPos) */
#ifdef XMEN2_MPW
extern s16 g_BgScrollFactorX;
#else
#define g_BgScrollFactorX XM_G(s16, 0xFFCD6A)
#endif
/* $FFCD6C word: plane A y scroll factor, 8.8 (Scroll_Init $100) */
#ifdef XMEN2_MPW
extern s16 g_FgScrollFactorY;
#else
#define g_FgScrollFactorY XM_G(s16, 0xFFCD6C)
#endif
/* $FFCD6E word: plane A x scroll factor, 8.8 ($100 = 1:1) (Scroll_Init; CloneA5_RunScene clears; Level_LoadMaps, Cam_SetPosScaled) */
#ifdef XMEN2_MPW
extern s16 g_FgScrollFactorX;
#else
#define g_FgScrollFactorX XM_G(s16, 0xFFCD6E)
#endif
/* $FFCD70 word: camera top edge in world pixels (ACTOR Draw: screen y = (y >> 8) - g_CameraY; CAMERA centre = g_CameraY + $70) */
#ifdef XMEN2_MPW
extern s16 g_CameraY;
#else
#define g_CameraY XM_G(s16, 0xFFCD70)
#endif
/* $FFCD72 word: camera left edge in world pixels (ACTOR Draw: screen x = (x >> 8) - g_CameraX; CAMERA centre = g_CameraX + $A0) */
#ifdef XMEN2_MPW
extern s16 g_CameraX;
#else
#define g_CameraX XM_G(s16, 0xFFCD72)
#endif
/* $FFCD74 ptr: top of the Mem_StackAlloc heap ($FF0000 at Sys_InitHardware) */
#ifdef XMEN2_MPW
extern PTR(void) g_HeapTop;
#else
#define g_HeapTop XM_G(PTR(void), 0xFFCD74)
#endif
/* $FFCD78 ptr: called by Obj_CollideSegs as (segment, obj, contact) to resolve the chosen floor contact when the segment type hook returns 0; Level_Load sets jt[$4EFA], Beast_RollPathClear swaps in Beast_RollProbeCb around its own Obj_CollideSegs call */
#ifdef XMEN2_MPW
extern PTR(void) g_SegResolveHook;
#else
#define g_SegResolveHook XM_G(PTR(void), 0xFFCD78)
#endif
/* $FFCD7C word: segment grid height in cells (Level_Load; Seg_GridCell, Seg_BuildGrid, Seg_GatherNearObj) */
#ifdef XMEN2_MPW
extern s16 g_SegGridH;
#else
#define g_SegGridH XM_G(s16, 0xFFCD7C)
#endif
/* $FFCD7E word: segment grid width in cells (Level_Load; Seg_GridCell, Seg_BuildGrid, Seg_GatherNearObj) */
#ifdef XMEN2_MPW
extern s16 g_SegGridW;
#else
#define g_SegGridW XM_G(s16, 0xFFCD7E)
#endif
/* $FFCD80 word: log2 of the segment grid cell height (Level_Load arg $3E; Seg_GridCell, Seg_FindBelow/Above) */
#ifdef XMEN2_MPW
extern s16 g_SegGridShiftY;
#else
#define g_SegGridShiftY XM_G(s16, 0xFFCD80)
#endif
/* $FFCD82 word: log2 of the segment grid cell width (Level_Load arg $3A; Seg_GridCell, Seg_FindLeft/Right) */
#ifdef XMEN2_MPW
extern s16 g_SegGridShiftX;
#else
#define g_SegGridShiftX XM_G(s16, 0xFFCD82)
#endif
/* $FFCD84 ptr: word grid, per cell the offset of its list in g_SegGridPool (Seg_BuildGrid; Seg_GridCell, Seg_GatherNearObj) */
#ifdef XMEN2_MPW
extern PTR(s16) g_SegGrid;
#else
#define g_SegGrid XM_G(PTR(s16), 0xFFCD84)
#endif
/* $FFCD88 ptr: pool of $FF-terminated segment-index lists, one per grid cell (Seg_BuildGrid builds it on the heap; Seg_FindAt/Below/Above/Left/Right, Boulder_FindFloorBelow, Ninja_FindSegInBox) */
#ifdef XMEN2_MPW
extern PTR(u8) g_SegGridPool;
#else
#define g_SegGridPool XM_G(PTR(u8), 0xFFCD88)
#endif
/* $FFCD8C word: highest segment index in use (Seg_Alloc, Seg_LoadList) */
#ifdef XMEN2_MPW
extern s16 g_SegHighWater;
#else
#define g_SegHighWater XM_G(s16, 0xFFCD8C)
#endif
/* $FFCD8E word: segments created from the level's FG list (Seg_LoadList; Seg_ClearAll clears); Seg_GatherNearObj skips them when indexing the grid */
#ifdef XMEN2_MPW
extern s16 g_SegLevelCount;
#else
#define g_SegLevelCount XM_G(s16, 0xFFCD8E)
#endif
/* $FFCD90 word: segment index where Seg_Alloc starts searching (Seg_LoadList sets it past the level lists) */
#ifdef XMEN2_MPW
extern s16 g_SegFirstFree;
#else
#define g_SegFirstFree XM_G(s16, 0xFFCD90)
#endif
/* $FFCD92 ptr: the g_SegTypeHooks entry fetched before calling it (Seg_ApplyObjPos, Seg_AlignObjVelocity, Seg_ComputeObjPos, Seg_ObjPosUnclamped, Seg_ComputeLength) */
#ifdef XMEN2_MPW
extern SegHookFn g_SegHookTmp;
#else
#define g_SegHookTmp XM_G(SegHookFn, 0xFFCD92)
#endif
/* $FFCD96 array of long: per floor-segment-type callback (mode, segment, object, 0), indexed by the segment's type byte +1; ACTOR Move (Actor_Move) and the floor code call it when non-zero; scenes install their own (CloneA5_SegHook15/16/17 at -$3416/-$3412/-$340E) */
#ifdef XMEN2_MPW
extern SegHookFn g_SegTypeHooks[];
#else
#define g_SegTypeHooks XM_GA(SegHookFn, 0xFFCD96)
#endif
/* $FFCE2A ptr: g_HeapTop after the level's floor-segment grid (Seg_LoadList writes it, nothing reads it by name); name proposed by the Seg worker, not yet in the disassembly */
#ifdef XMEN2_MPW
extern PTR(void) g_SegHeapEnd;
#else
#define g_SegHeapEnd XM_G(PTR(void), 0xFFCE2A)
#endif
/* $FFCE2E array: floor-segment records, $14 bytes each ($FFCE2E) (Seg_Alloc, Seg_LoadList) */
#ifdef XMEN2_MPW
extern Seg g_Segs[];
#else
#define g_Segs XM_GA(Seg, 0xFFCE2E)
#endif
/* $FFE21A ptr: current hardware-sprite slot in g_SpriteTable while emitting (Spr_BeginFrame, Spr_Emit*) */
#ifdef XMEN2_MPW
extern PTR(HwSprite) g_SprWritePtr;
#else
#define g_SprWritePtr XM_G(PTR(HwSprite), 0xFFE21A)
#endif
/* $FFE21E word: index of the current hardware-sprite slot (Spr_BeginFrame $51, Spr_EndFrame) */
#ifdef XMEN2_MPW
extern s16 g_SprIndex;
#else
#define g_SprIndex XM_G(s16, 0xFFE21E)
#endif
/* $FFE220 word: cleared by Spr_InitLinks, never read */
#ifdef XMEN2_MPW
extern s16 g_SprUnusedE220;
#else
#define g_SprUnusedE220 XM_G(s16, 0xFFE220)
#endif
/* $FFE222 word: last slot used last frame, $51 = none (Spr_EndFrame, Spr_BeginFrame) */
#ifdef XMEN2_MPW
extern s16 g_SprLast;
#else
#define g_SprLast XM_G(s16, 0xFFE222)
#endif
/* $FFE224 word: cleared by Spr_InitLinks, never read */
#ifdef XMEN2_MPW
extern s16 g_SprUnusedE224;
#else
#define g_SprUnusedE224 XM_G(s16, 0xFFE224)
#endif
/* $FFE226 array: sprite attribute buffer ($FFE226, $280 bytes) DMAed to VRAM $F000 every V-blank (VDP_FlushDmaQueue) */
#ifdef XMEN2_MPW
extern HwSprite g_SpriteTable[];
#else
#define g_SpriteTable XM_GA(HwSprite, 0xFFE226)
#endif
/* $FFE4B6 byte: set by game code when g_SpriteTable/g_DmaQueue are ready; VDP_FrameUpdate flushes and clears it */
#ifdef XMEN2_MPW
extern u8 g_DmaFlushReq;
#else
#define g_DmaFlushReq XM_G(u8, 0xFFE4B6)
#endif
/* $FFE4B8 word: VDP mode register 1 command shadow $81xx ($40 display, $20 V-int, $10 DMA) (Sys_InitHardware, VDP_DmaToVram) */
#ifdef XMEN2_MPW
extern s16 g_VdpReg1;
#else
#define g_VdpReg1 XM_G(s16, 0xFFE4B8)
#endif
/* $FFE4BA word: VRAM tile limit $700 for actor sprite graphics (Scene_PreLoadReset); Actor_Activate/MagnetoBoss_Activate compare the tile end from Spr_Init against it */
#ifdef XMEN2_MPW
extern s16 g_VramTileLimit;
#else
#define g_VramTileLimit XM_G(s16, 0xFFE4BA)
#endif
/* $FFE4BC word: number of free VRAM tiles from g_HScrollFreeTile to $800 ($5F default, $42 with line scroll; Scene_PreLoadReset, Scroll_SetParallax) */
#ifdef XMEN2_MPW
extern s16 g_HScrollFreeCount;
#else
#define g_HScrollFreeCount XM_G(s16, 0xFFE4BC)
#endif
/* $FFE4BE word: first free VRAM tile after the H-scroll table: $7A1 by default (Scene_PreLoadReset), $7BE with a line-scroll parallax that fills $380 bytes (Scroll_SetParallax, BaniMazaA1_InitScroll) */
#ifdef XMEN2_MPW
extern s16 g_HScrollFreeTile;
#else
#define g_HScrollFreeTile XM_G(s16, 0xFFE4BE)
#endif
/* $FFE4C0 10 x 14-byte explosion slots {word x, word y, byte step, byte delay, long timer, word attr, word damage} (Explosion_SpawnEx) */
#ifdef XMEN2_MPW
extern u8 g_Explosions[];
#else
#define g_Explosions XM_GA(u8, 0xFFE4C0)
#endif
/* $FFE54C word: ICandy pool slot size in bytes (ICandy_PoolInit) */
#ifdef XMEN2_MPW
extern s16 g_ICandyStride;
#else
#define g_ICandyStride XM_G(s16, 0xFFE54C)
#endif
/* $FFE54E ptr: ICandy pool base (ICandy_PoolInit/ICandy_PoolAlloc) */
#ifdef XMEN2_MPW
extern PTR(void) g_ICandyPool;
#else
#define g_ICandyPool XM_G(PTR(void), 0xFFE54E)
#endif
/* $FFE552 word: ICandy pool slot count, 0 = no pool (ICandy_PoolInit) */
#ifdef XMEN2_MPW
extern s16 g_ICandyCount;
#else
#define g_ICandyCount XM_G(s16, 0xFFE552)
#endif
/* $FFE554 ptr: GAUSSBULLET pool {word count, word max 6, long anim table, 32-byte records} (GaussBullet_InitPool) */
#ifdef XMEN2_MPW
extern PTR(void) g_GaussPool;
#else
#define g_GaussPool XM_G(PTR(void), 0xFFE554)
#endif
/* $FFE558 word: sprite-definition slot of the gauss bullet graphics (GaussBullet_InitPool, GaussBullet_Update) */
#ifdef XMEN2_MPW
extern s16 g_GaussSprDef;
#else
#define g_GaussSprDef XM_G(s16, 0xFFE558)
#endif
/* $FFE55A ptr: BBOARD trigger-flag board, 100 word entries (Switch_ClassInit, Synchronizer_SetFlag) */
#ifdef XMEN2_MPW
extern PTR(void) g_BBoard;
#else
#define g_BBoard XM_G(PTR(void), 0xFFE55A)
#endif
/* $FFE55E ptr: pool of 4 SYNCHRONIZER objects (Synchronizer_New) */
#ifdef XMEN2_MPW
extern PTR(void) g_SyncPool;
#else
#define g_SyncPool XM_G(PTR(void), 0xFFE55E)
#endif
/* $FFE562 byte: cleared by BabyBio_LevelStart; no other code reads or writes it (A5 -$1C9E; not yet in the disassembly's ram.asm) */
#ifdef XMEN2_MPW
extern u8 g_BabyBioUnused;
#else
#define g_BabyBioUnused XM_G(u8, 0xFFE562)
#endif
/* $FFE564 128 longs: sine table built by BioBalls_AllocBallList, read by BioBalls_MotionOrbit/Ring/Helix */
#ifdef XMEN2_MPW
extern s32 g_BioBallsSine[128];
#else
#define g_BioBallsSine XM_GA(s32, 0xFFE564)
#endif
/* $FFE768 ptr: ball list shared by BIOBALLS objects {word count; records}, allocated by BioBalls_AllocBallList and copied to each object's +$92 (BioBalls_Init, UpdateBox, SetFormation) */
#ifdef XMEN2_MPW
extern PTR(s16) g_BioBallsList;
#else
#define g_BioBallsList XM_G(PTR(s16), 0xFFE768)
#endif
/* $FFE76C word: sprite-definition slot of Art_BigBioSpr (BigBio_ClassInit via SprDef_Add; BigBio_Draw, BigBio_SpitUpdateAll) */
#ifdef XMEN2_MPW
extern s16 g_BigBioSprDef;
#else
#define g_BigBioSprDef XM_G(s16, 0xFFE76C)
#endif
/* $FFE76E word: spit projectiles in use (BigBio_ClassInit clears; BigBio_SpitSpawn/Free/UpdateAll) */
#ifdef XMEN2_MPW
extern s16 g_BigBioSpitCount;
#else
#define g_BigBioSpitCount XM_G(s16, 0xFFE76E)
#endif
/* $FFE770 ptr: BIGBIO spit projectile pool (BigBio_ClassInit; BigBio_SpitSpawn/Free/UpdateAll) */
#ifdef XMEN2_MPW
extern PTR(BigBioSpit) g_BigBioSpitPool;
#else
#define g_BigBioSpitPool XM_G(PTR(BigBioSpit), 0xFFE770)
#endif
/* $FFE774 ptr: BIGBIO's 16 orb records (BigBio_ClassInit allocates; BigBio_PlaceOrbs, BigBio_UpdateOrbRing, BigBio_State*, BigBio_Draw) */
#ifdef XMEN2_MPW
extern PTR(BigBioOrb) g_BigBioOrbs;
#else
#define g_BigBioOrbs XM_G(PTR(BigBioOrb), 0xFFE774)
#endif
/* $FFE778 word: sprite-definition slot of Art_CloneA1Spr, the BIOFLAME graphics (CloneA1/CloneA2/SavageA2/SavageA3/SiberiaB1/XClone_LoadResources via SprDef_Add; BioFlame_Init, ToxicCan_DrawLeak) */
#ifdef XMEN2_MPW
extern s16 g_BioFlameSprDef;
#else
#define g_BioFlameSprDef XM_G(s16, 0xFFE778)
#endif
/* $FFE77A word: decremented by FlyBrood_Deactivate; no code increments or reads it (a live FLYBROOD count left unused; A5 -$1A86, not yet in the disassembly's ram.asm) */
#ifdef XMEN2_MPW
extern s16 g_FlyBroodCount;
#else
#define g_FlyBroodCount XM_G(s16, 0xFFE77A)
#endif
/* $FFE77C word: sprite-definition slot of Art_FlyBroodSpr (FlyBrood_ClassInit, XClone_LoadResources via SprDef_Add; FlyBrood_Init, FlyBrood_Draw) */
#ifdef XMEN2_MPW
extern s16 g_FlyBroodSprDef;
#else
#define g_FlyBroodSprDef XM_G(s16, 0xFFE77C)
#endif
/* $FFE77E word: sprite-definition slot of Art_HornetSpr (Hornet_ClassInit via SprDef_Add; Hornet_Init) */
#ifdef XMEN2_MPW
extern s16 g_HornetSprDef;
#else
#define g_HornetSprDef XM_G(s16, 0xFFE77E)
#endif
/* $FFE780 word: sprite-definition slot of Art_HornetBulletSpr (HornetBullet_ClassInit via SprDef_Add; HornetBullet_Init) */
#ifdef XMEN2_MPW
extern s16 g_HornetBulletSprDef;
#else
#define g_HornetBulletSprDef XM_G(s16, 0xFFE780)
#endif
/* $FFE782 word: sprite-definition slot of Art_MaintRobotSpr (CloneA1/CloneA2/SentinelsD1_LoadResources via SprDef_Add; SpaceB1_LoadResources sets -1; MaintRobot_Init tests and uses it) */
#ifdef XMEN2_MPW
extern s16 g_MaintRobotSprDef;
#else
#define g_MaintRobotSprDef XM_G(s16, 0xFFE782)
#endif
/* $FFE784 ptr: mine pool records allocated by Mine_InitPool (Mine_PoolAdd/Remove/Update/Draw) */
#ifdef XMEN2_MPW
extern PTR(void) g_MinePool;
#else
#define g_MinePool XM_G(PTR(void), 0xFFE784)
#endif
/* $FFE788 word: mines in use in g_MinePool (Mine_InitPool clears; Mine_PoolAdd/Remove, Mine_PoolUpdate/Draw) */
#ifdef XMEN2_MPW
extern s16 g_MinePoolCount;
#else
#define g_MinePoolCount XM_G(s16, 0xFFE788)
#endif
/* $FFE78A word: sprite-definition slot of Art_MineNem (Mine_ClassInit via SprDef_Add; Mine_Init, Mine_PoolDraw) */
#ifdef XMEN2_MPW
extern s16 g_MineSprDef;
#else
#define g_MineSprDef XM_G(s16, 0xFFE78A)
#endif
/* $FFE78C word: sprite-definition slot of Art_NinjaSpr2 (SiberiaB1_LoadResources via SprDef_Add; SentinelsD1_LoadResources sets -1 = not loaded; Ninja_Init tests and uses it) */
#ifdef XMEN2_MPW
extern s16 g_NinjaSprDef2;
#else
#define g_NinjaSprDef2 XM_G(s16, 0xFFE78C)
#endif
/* $FFE78E word: sprite-definition slot of Art_SpaceB1Spr2 (SpaceB1_LoadResources via SprDef_Add; Spark_Init) */
#ifdef XMEN2_MPW
extern s16 g_SparkSprDef;
#else
#define g_SparkSprDef XM_G(s16, 0xFFE78E)
#endif
/* $FFE790 word: sprite-definition slot of Art_Spr_14018E (TANK ClassInit Tank_ClassInit via SprDef_Add; TANK Init Tank_Init) */
#ifdef XMEN2_MPW
extern s16 g_TankSprDef;
#else
#define g_TankSprDef XM_G(s16, 0xFFE790)
#endif
/* $FFE792 word: sprite-definition slot of Art_Spr_1445D2 (TECHWALLGUN ClassInit TechWallGun_ClassInit via SprDef_Add; read by TechWallGun_UpdateBullet_Draw) */
#ifdef XMEN2_MPW
extern s16 g_TechWallGunSprDef;
#else
#define g_TechWallGunSprDef XM_G(s16, 0xFFE792)
#endif
/* $FFE794 ptr: 16 x 16-byte records allocated by TECHWALLGUN's ClassInit TechWallGun_ClassInit (Mem_StackAlloc); scanned by TechWallGun_FindFreeBullet */
#ifdef XMEN2_MPW
extern PTR(WallGunBullet) g_TechWallGunRecs;
#else
#define g_TechWallGunRecs XM_G(PTR(WallGunBullet), 0xFFE794)
#endif
/* $FFE798 sprite instance (object +$2E layout) shared by FLUXBOMB: Spr_Init by Scene_AllocCommonVram, frame set by FluxBomb_SetFrame, drawn by FluxBomb_Draw; +6/+8 give the particle tiles */
#ifdef XMEN2_MPW
extern SprInst g_FluxBombSpr;
#else
#define g_FluxBombSpr XM_G(SprInst, 0xFFE798)
#endif
/* $FFE954 byte: non-zero = Snd_Play/Stop/etc. send nothing (set while demos play, Game_PlayAttractDemo, and around some init calls) */
#ifdef XMEN2_MPW
extern u8 g_SndMute;
#else
#define g_SndMute XM_G(u8, 0xFFE954)
#endif
/* $FFE956 byte: player 2's character uses the second HUD meter (same writers/readers as g_P1HasMeter) */
#ifdef XMEN2_MPW
extern u8 g_P2HasMeter;
#else
#define g_P2HasMeter XM_G(u8, 0xFFE956)
#endif
/* $FFE958 byte: player 1's character uses the second HUD meter (player +$9E): set by Nightcrawler_Init, Gambit_Init, Cyclops_Init, cleared by XPlayer_Init; HUD_UpdateHealthBars and Level_FrameLoop draw it */
#ifdef XMEN2_MPW
extern u8 g_P1HasMeter;
#else
#define g_P1HasMeter XM_G(u8, 0xFFE958)
#endif
/* $FFE95A ptr: Psylocke's second sprite instance (player +$C6, psi effect), stored by Psylocke_Init; BioNpc_DrawStunned draws it over a stunned enemy */
#ifdef XMEN2_MPW
extern PTR(void) g_PsylockeFxSpr;
#else
#define g_PsylockeFxSpr XM_G(PTR(void), 0xFFE95A)
#endif
/* $FFE95E 3 x $10 bytes: GAMBIT's thrown cards (Gambit_Init points player +$BC at them; Gambit_ClearCards, Gambit_UpdateCard). Not in src/ram.asm yet (A5 -$18A2). */
#ifdef XMEN2_MPW
extern GambitCard g_GambitCards[3];
#else
#define g_GambitCards XM_GA(GambitCard, 0xFFE95E)
#endif
/* $FFE98E byte: a new g_BeamFlickerNew is ready (set by Cyclops_DrawBeam_Emit/Cyclops_DrawBeam_None, Cyclops_ResetBeamFlicker clears the word); Cyclops_BeamVBlankFlicker copies it when the frame was updated */
#ifdef XMEN2_MPW
extern u8 g_BeamFlickerPending;
#else
#define g_BeamFlickerPending XM_G(u8, 0xFFE98E)
#endif
/* $FFE98F byte: active splice enabled (copied from g_BeamFlickerCur+3 by Cyclops_BeamVBlankFlicker; Cyclops_DrawBeam_None tests it) */
#ifdef XMEN2_MPW
extern u8 g_BeamFlickerOn;
#else
#define g_BeamFlickerOn XM_G(u8, 0xFFE98F)
#endif
/* $FFE990 4 bytes {first sprite slot - 1, first slot, byte from the record +3, enable}: sprite-link splice built while emitting a sprite group (Cyclops_DrawBeam_Emit, Cyclops_DrawBeam_None; Cyclops_ResetBeamFlicker clears); copied to g_BeamFlickerCur in V-blank */
#ifdef XMEN2_MPW
extern u8 g_BeamFlickerNew[4];
#else
#define g_BeamFlickerNew XM_GA(u8, 0xFFE990)
#endif
/* $FFE994 4 bytes: active copy of g_BeamFlickerNew used by Cyclops_BeamVBlankFlicker (from VDP_FrameUpdate) to rewrite one sprite's link word in VRAM, alternating between two links every V-blank so a sprite group flickers */
#ifdef XMEN2_MPW
extern u8 g_BeamFlickerCur[4];
#else
#define g_BeamFlickerCur XM_GA(u8, 0xFFE994)
#endif
/* $FFE998 4 bytes: two {size, link} words of the sprite attribute table that Cyclops_BeamVBlankFlicker writes to VRAM: [0..1] the slot before the beam's sprites (linked to the first or the second piece list by V-blank parity), [2..3] the last sprite of the first list (Cyclops_DrawBeam_Emit sets [3]: past the second list). Not in src/ram.asm yet (A5 -$1868). */
#ifdef XMEN2_MPW
extern u8 g_BeamFlickerSpr[4];
#else
#define g_BeamFlickerSpr XM_GA(u8, 0xFFE998)
#endif
/* $FFE99C ptr: player-2 start placement entry (XPlayer_InitSpawnRec, placement +6 == 2; Players_PlaceAtStart) */
#ifdef XMEN2_MPW
extern PTR(Placement) g_P2StartRec;
#else
#define g_P2StartRec XM_G(PTR(Placement), 0xFFE99C)
#endif
/* $FFE9A0 ptr: player-1 start placement entry (XPlayer_InitSpawnRec, placement +6 != 2); Players_PlaceAtStart and CloneA2_StartCamera take the start x/y from +2/+4, then it is cleared */
#ifdef XMEN2_MPW
extern PTR(Placement) g_P1StartRec;
#else
#define g_P1StartRec XM_G(PTR(Placement), 0xFFE9A0)
#endif
/* $FFE9A4 sprite instance (same layout as object +$2E) for the player hit spark: Spr_Init by Scene_AllocCommonVram, drawn by XPlayer_Draw, h-flip (bit 2) set by XPlayer_Attack from the attacker */
#ifdef XMEN2_MPW
extern SprInst g_HitSparkSpr;
#else
#define g_HitSparkSpr XM_G(SprInst, 0xFFE9A4)
#endif
/* $FFE9B8 obj: player 2 object ($FFE9B8), +$9D=1; vtable 0 until P2 joins (TAS frame 49140) */
#ifdef XMEN2_MPW
extern XPlayer g_Player2;
#else
#define g_Player2 XM_G(XPlayer, 0xFFE9B8)
#endif
/* $FFEAA8 obj: player 1 object ($FFEAA8), vtable at +0 = character class (Player_SetCharacter sets +$9D=0 for it; TAS: Nightcrawler class from frame 180) */
#ifdef XMEN2_MPW
extern XPlayer g_Player1;
#else
#define g_Player1 XM_G(XPlayer, 0xFFEAA8)
#endif
/* $FFEB9A ptr: position in a pause-screen button sequence: each new press (& $7F) must equal the next byte (Pause_HandleInput); sequences from jt data $E0A-$E62 (Pause_HandleInput_EndCode); 0 = none (Pause_Check clears) */
#ifdef XMEN2_MPW
extern PTR(void) g_PauseCheatPtr;
#else
#define g_PauseCheatPtr XM_G(PTR(void), 0xFFEB9A)
#endif
/* $FFEB9E byte: set to leave the pause loop (Pause_Loop waits on it): a pause cheat changed the scene, or a player pressed a button (Pause_HandleInput_EndCode); cleared by the pause handler Pause_Check and Level_Load */
#ifdef XMEN2_MPW
extern u8 g_PauseExit;
#else
#define g_PauseExit XM_G(u8, 0xFFEB9E)
#endif
/* $FFEBA2 ptr: head of the player-side attacker hit list (HitList_Add from Actor_Activate, +6 flag $10; Collide_RunAll vs g_HitListEnemyBody) */
#ifdef XMEN2_MPW
extern PTR(ListNode) g_HitListPlayerAtk;
#else
#define g_HitListPlayerAtk XM_G(PTR(ListNode), 0xFFEBA2)
#endif
/* $FFEBA6 ptr: head of the player-side body hit list (Actor_Activate +$84 node; enemy attackers and projectiles test it) */
#ifdef XMEN2_MPW
extern PTR(ListNode) g_HitListPlayerBody;
#else
#define g_HitListPlayerBody XM_G(PTR(ListNode), 0xFFEBA6)
#endif
/* $FFEBAA ptr: head of the enemy attacker hit list (Actor_Activate +$80 node; Collide_RunAll vs g_HitListPlayerBody) */
#ifdef XMEN2_MPW
extern PTR(ListNode) g_HitListEnemyAtk;
#else
#define g_HitListEnemyAtk XM_G(PTR(ListNode), 0xFFEBAA)
#endif
/* $FFEBAE ptr: head of the enemy body hit list (Actor_Activate +$84 node; player attacks and projectiles test it) */
#ifdef XMEN2_MPW
extern PTR(ListNode) g_HitListEnemyBody;
#else
#define g_HitListEnemyBody XM_G(PTR(ListNode), 0xFFEBAE)
#endif
/* $FFEBB2 array: 50 hit-list nodes x $C {next, prev, owner} (HitList_Init, HitList_AllocNode) */
#ifdef XMEN2_MPW
extern u8 g_HitNodes[];
#else
#define g_HitNodes XM_GA(u8, 0xFFEBB2)
#endif
/* $FFEE0A ptr: storage for level objects, $C2 bytes each (Obj_AllocPool) */
#ifdef XMEN2_MPW
extern PTR(void) g_ObjStorage;
#else
#define g_ObjStorage XM_G(PTR(void), 0xFFEE0A)
#endif
/* $FFEE0E word: entries in g_ObjTable (level objects + 2) (Obj_AllocPool, Level_FrameLoop) */
#ifdef XMEN2_MPW
extern s16 g_ObjCount;
#else
#define g_ObjCount XM_G(s16, 0xFFEE0E)
#endif
/* $FFEE10 byte: 1 (Level_Load) = normal map streaming margin (10 blocks, 24 px); AvalonB1_InitCamera and SiberiaB1_PreLoad/PostLoad clear it for a wide one (15 blocks, $B8 px) (Map_RedrawBlock, Map_DrawRowA, Map_StreamColumns) */
#ifdef XMEN2_MPW
extern u8 g_MapStreamNarrow;
#else
#define g_MapStreamNarrow XM_G(u8, 0xFFEE10)
#endif
/* $FFEE12 byte: BG map narrower than 16 blocks, so it is repeated horizontally (Level_LoadMaps; SiberiaB1_PreLoad sets; streaming Map_RedrawBlock/Map_DrawRowB/Map_StreamColumns) */
#ifdef XMEN2_MPW
extern u8 g_BgNarrow;
#else
#define g_BgNarrow XM_G(u8, 0xFFEE12)
#endif
/* $FFEE14 ptr: saved g_PalCycleScript (Pal_SaveCycle saves, Pal_RestoreCycle restores after a fade) */
#ifdef XMEN2_MPW
extern PTR(void) g_PalCycleSave;
#else
#define g_PalCycleSave XM_G(PTR(void), 0xFFEE14)
#endif
/* $FFEE18 64 words: copy of g_PalBase saved by Pal_SaveBase and restored by Pal_RestoreBase (around palette fades, Pal_FadeInWait) */
#ifdef XMEN2_MPW
extern s16 g_PalSave[64];
#else
#define g_PalSave XM_GA(s16, 0xFFEE18)
#endif
/* $FFEE58 16 words: palette line 2 saved by BroodBoss_StateHurt before the hurt flash and restored by BroodBoss_StateHurtEnd */
#ifdef XMEN2_MPW
extern s16 g_BroodBossPalSave[16];
#else
#define g_BroodBossPalSave XM_GA(s16, 0xFFEE58)
#endif
/* $FFEE98 long: y (8 fraction bits) where Obj_CollideSegs last resolved a player (both players share it); a player still stuck after 30 passes is put back there (once per call: g_CollideReset) */
#ifdef XMEN2_MPW
extern s32 g_CollideSafeY;
#else
#define g_CollideSafeY XM_G(s32, 0xFFEE98)
#endif
/* $FFEE9C long: x of that position (Obj_CollideSegs) */
#ifdef XMEN2_MPW
extern s32 g_CollideSafeX;
#else
#define g_CollideSafeX XM_G(s32, 0xFFEE9C)
#endif
/* $FFEEA0 byte: $FF once Obj_CollideSegs has put a stuck player back at g_CollideSafeX/Y; cleared at the start of each call */
#ifdef XMEN2_MPW
extern u8 g_CollideReset;
#else
#define g_CollideReset XM_G(u8, 0xFFEEA0)
#endif
/* $FFEEA2 ptr: called by the object-vs-floor-segment pass Obj_CollideSegs as (obj, contact list, contact) when a contact is blocked and the object is not in hit cooldown (segment +0 |= $40, +3 = 30); Level_Load sets Sys_Nop1F949E */
#ifdef XMEN2_MPW
extern PTR(void) g_SegBlockedHook;
#else
#define g_SegBlockedHook XM_G(PTR(void), 0xFFEEA2)
#endif
/* $FFEEA6 byte: cleared by DrawList_Init and never set; DrawList_Remove/DrawList_SetPriority return it as their failure result (a C 'false' global) */
#ifdef XMEN2_MPW
extern u8 g_DrawListZero;
#else
#define g_DrawListZero XM_G(u8, 0xFFEEA6)
#endif
/* $FFEEA8 ptr: round-robin search start for DrawList_AllocNode */
#ifdef XMEN2_MPW
extern PTR(void) g_DrawNodeCursor;
#else
#define g_DrawNodeCursor XM_G(PTR(void), 0xFFEEA8)
#endif
/* $FFEEAC ptr: last draw-list node (DrawList_Add, DrawList_Remove) */
#ifdef XMEN2_MPW
extern PTR(void) g_DrawListTail;
#else
#define g_DrawListTail XM_G(PTR(void), 0xFFEEAC)
#endif
/* $FFEEB0 ptr: first (lowest key) draw-list node (DrawList_Add, DrawList_DrawAll) */
#ifdef XMEN2_MPW
extern PTR(void) g_DrawListHead;
#else
#define g_DrawListHead XM_G(PTR(void), 0xFFEEB0)
#endif
/* $FFEEB4 30 x $14-byte draw-list nodes {next, prev, func, object, word key, word flags} (DrawList_Init) */
#ifdef XMEN2_MPW
extern u8 g_DrawNodes[];
#else
#define g_DrawNodes XM_GA(u8, 0xFFEEB4)
#endif
/* $FFF10C ptr: per-class pointer array, one long per class in g_ClassList (ClassList_InitAll) */
#ifdef XMEN2_MPW
extern PTR(PTR(void)) g_ClassData;
#else
#define g_ClassData XM_G(PTR(PTR(void)), 0xFFF10C)
#endif
/* $FFF110 ptr: timer whose callback is running (Timer_RunAll) */
#ifdef XMEN2_MPW
extern PTR(void) g_TimerCurrent;
#else
#define g_TimerCurrent XM_G(PTR(void), 0xFFF110)
#endif
/* $FFF114 ptr: head of the active timer list (Timer_Create, Timer_RunAll) */
#ifdef XMEN2_MPW
extern PTR(Timer) g_TimerList;
#else
#define g_TimerList XM_G(PTR(Timer), 0xFFF114)
#endif
/* $FFF118 30 x $16-byte timer slots {+0 state, +2 period, +4 next, +8 prev, +$C func, +$10 arg, +$14 byte} (Timer_Create, Timer_RunAll) */
#ifdef XMEN2_MPW
extern Timer g_Timers[30];
#else
#define g_Timers XM_GA(Timer, 0xFFF118)
#endif
/* $FFF3AC byte: number of registered scenes (next free g_SceneTable slot, Scene_Register) */
#ifdef XMEN2_MPW
extern u8 g_SceneCount;
#else
#define g_SceneCount XM_G(u8, 0xFFF3AC)
#endif
/* $FFF3AE 8-byte entries {+0 name string, +4 scene function}, filled by Scene_Register (scene register) */
#ifdef XMEN2_MPW
extern SceneEntry g_SceneTable[52];
#else
#define g_SceneTable XM_GA(SceneEntry, 0xFFF3AE)
#endif
/* $FFF54E ptr: BG map blob passed to Level_Load (3rd argument); RollingDiscs_ClassInit and SavageJungle_InitPalPhases read a per-level list at the offset in its word +$12 */
#ifdef XMEN2_MPW
extern PTR(void) g_LevelBgMap;
#else
#define g_LevelBgMap XM_G(PTR(void), 0xFFF54E)
#endif
/* $FFF552 ptr: FG map blob passed to Level_Load (header +2/+4 = width/height in cells); CloneA2_SetCameraLimitsToMap and SwampPlatform_Move read the map size from it */
#ifdef XMEN2_MPW
extern PTR(void) g_LevelFgMap;
#else
#define g_LevelFgMap XM_G(PTR(void), 0xFFF552)
#endif
/* $FFF556 word: y delta applied by Obj_ShiftTree (set by Spr_PlaceAttached) */
#ifdef XMEN2_MPW
extern s16 g_ShiftDY;
#else
#define g_ShiftDY XM_G(s16, 0xFFF556)
#endif
/* $FFF558 word: x delta applied by Obj_ShiftTree (set by Spr_PlaceAttached) */
#ifdef XMEN2_MPW
extern s16 g_ShiftDX;
#else
#define g_ShiftDX XM_G(s16, 0xFFF558)
#endif
/* $FFF55A ptr: $DC0-byte scaler work area allocated on the first Scaler_Start call (Mem_StackAlloc tag Str_ScalerData); Level_Load clears it */
#ifdef XMEN2_MPW
extern PTR(void) g_ScalerWork;
#else
#define g_ScalerWork XM_G(PTR(void), 0xFFF55A)
#endif
/* $FFF55E ptr: result of Scaler_InitParams = &g_ScalerParams (0 on failure), passed back as the scale record by BigAcolyte_ActGrow/StateBurstWall, PlayerSelect_Run, Cerebro_PlayIntro */
#ifdef XMEN2_MPW
extern PTR(ScaleParams) g_ScaleParamsPtr;
#else
#define g_ScaleParamsPtr XM_G(PTR(ScaleParams), 0xFFF55E)
#endif
/* $FFF562 ptr: heap tile buffer the scaler decodes into, then DMAed to VRAM (BigAcolyte_RenderScaled, Cerebro_BigSpriteDecode/Draw, PlayerSelect_AllocCharBuffers_Measure) */
#ifdef XMEN2_MPW
extern PTR(void) g_ScaleTileBuf;
#else
#define g_ScaleTileBuf XM_G(PTR(void), 0xFFF562)
#endif
/* $FFF566 ptr: source picture for the scaler: the big acolyte picture record (BigAcolyte_ActAppear) or a heap bitmap filled by Spr_RenderToBitmap (Cerebro_PlayIntro, PlayerSelect_AllocCharBuffers_Measure); freed after the Cerebro intro */
#ifdef XMEN2_MPW
extern PTR(void) g_ScaleSrcBitmap;
#else
#define g_ScaleSrcBitmap XM_G(PTR(void), 0xFFF566)
#endif
/* $FFF56A $26-byte scaler parameter block, copied from the caller's record by Scaler_Start (9 longs + word) or set field by field by Scaler_InitParams: +0/+2 source size words, +$C/+$E output width/height in tiles, +$10/+$12 output size in pixels, +$14 byte 0 keep/1 clear buffer/2 centre, +$15 byte table key, +$19 byte 0 = emit hardware sprites directly, +$1A byte packed/streamed upload, +$1C word rows per call */
#ifdef XMEN2_MPW
extern ScaleParams g_ScalerParams;
#else
#define g_ScalerParams XM_G(ScaleParams, 0xFFF56A)
#endif
/* $FFF590 ptr: 2nd argument of Scaler_Start, a packed 4bpp source (two pixels per byte): Scaler_BeginPass expands its rows as it goes (g_ScalerHalfWidth), or, given with g_ScalerSrcB, Scaler_ExpandSource unpacks it into g_ScalerSrcB at once and it is zeroed */
#ifdef XMEN2_MPW
extern PTR(void) g_ScalerSrcA;
#else
#define g_ScalerSrcA XM_G(PTR(void), 0xFFF590)
#endif
/* $FFF594 ptr: 3rd argument of Scaler_Start, a source of one byte per pixel (Scaler_BeginPass; Scaler_ExpandSource's output) */
#ifdef XMEN2_MPW
extern PTR(void) g_ScalerSrcB;
#else
#define g_ScalerSrcB XM_G(PTR(void), 0xFFF594)
#endif
/* $FFF598 ptr: output tile buffer (4th argument of Scaler_Start; Scaler_BeginPass fills it, Scaler_RowEnd DMAs from it) */
#ifdef XMEN2_MPW
extern PTR(void) g_ScalerDest;
#else
#define g_ScalerDest XM_G(PTR(void), 0xFFF598)
#endif
/* $FFF59C word: VRAM address for the output (5th argument of Scaler_Start); >>5 = first tile for directly emitted sprites (Scaler_BeginPass, Scaler_EmitSprites) */
#ifdef XMEN2_MPW
extern s16 g_ScalerVram;
#else
#define g_ScalerVram XM_G(s16, 0xFFF59C)
#endif
/* $FFF59E byte: set by Scaler_Start when a source is given without its second argument; Scaler_BeginPass then halves g_ScalerParams+0 */
#ifdef XMEN2_MPW
extern u8 g_ScalerHalfWidth;
#else
#define g_ScalerHalfWidth XM_G(u8, 0xFFF59E)
#endif
/* $FFF59F byte: scaler call accepted; Scaler_Start returns &g_ScalerParams when set, else 0 (Scaler_Start, Scaler_Step) */
#ifdef XMEN2_MPW
extern u8 g_ScalerReady;
#else
#define g_ScalerReady XM_G(u8, 0xFFF59F)
#endif
/* $FFF5A0 13 longs: d0-d7/a0-a4 of a yielded scaler pass (movem by Scaler_RowEnd, restored by Scaler_Resume) */
#ifdef XMEN2_MPW
extern ScalerRegs g_ScalerRegSave;
#else
#define g_ScalerRegSave XM_G(ScalerRegs, 0xFFF5A0)
#endif
/* $FFF5D4 ptr: code address where a yielded scaler pass resumes (saved by Scaler_RowEnd, pushed and returned to by Scaler_Resume) */
#ifdef XMEN2_MPW
extern PTR(void) g_ScalerResumePC;
#else
#define g_ScalerResumePC XM_G(PTR(void), 0xFFF5D4)
#endif
/* $FFF5D8 word: rows left before the scaler yields, reloaded from g_ScalerParams+$1C (Scaler_BeginPass, Scaler_Resume); Scaler_RowEnd saves state when it reaches 0 */
#ifdef XMEN2_MPW
extern s16 g_ScalerBudget;
#else
#define g_ScalerBudget XM_G(s16, 0xFFF5D8)
#endif
/* $FFF5DA word: the source's bytes per row, srcW or (srcW + 1) / 2 for a packed source (Scaler_BeginPass). Not named in the disassembly yet (-$C26). */
#ifdef XMEN2_MPW
extern s16 g_ScalerSrcStride;
#else
#define g_ScalerSrcStride XM_G(s16, 0xFFF5DA)
#endif
/* $FFF5DC word: row counter that picks g_ScalerLine2 or g_ScalerLine3 (Scaler_BuildStepTables sets from -$BFA, Scaler_ClearRows/Scaler_BeginPass count down) */
#ifdef XMEN2_MPW
extern s16 g_ScalerRowCount;
#else
#define g_ScalerRowCount XM_G(s16, 0xFFF5DC)
#endif
/* $FFF5DE word: rows of the current chunk still to emit before the next upload (Scaler_BeginPass sets, Scaler_RowEnd reads) */
#ifdef XMEN2_MPW
extern s16 g_ScalerChunkLeft;
#else
#define g_ScalerChunkLeft XM_G(s16, 0xFFF5DE)
#endif
/* $FFF5E0 word: rows already in the current 32-row chunk (Scaler_BeginPass sets, Scaler_RowEnd clears) */
#ifdef XMEN2_MPW
extern s16 g_ScalerChunkRow;
#else
#define g_ScalerChunkRow XM_G(s16, 0xFFF5E0)
#endif
/* $FFF5E2 word: VRAM address of the next streamed chunk, starts at g_ScalerVram and advances by each DMA (Scaler_BeginPass, Scaler_RowEnd) */
#ifdef XMEN2_MPW
extern s16 g_ScalerVramCursor;
#else
#define g_ScalerVramCursor XM_G(s16, 0xFFF5E2)
#endif
/* $FFF5E4 word: x offset of the picture inside the output, (width*8 - pixel width)/2 when g_ScalerParams+$14 = 2 (centre) else 0 (Scaler_BeginPass; read by Scaler_RowEnd) */
#ifdef XMEN2_MPW
extern s16 g_ScalerOffsetX;
#else
#define g_ScalerOffsetX XM_G(s16, 0xFFF5E4)
#endif
/* $FFF5E6 word: output rows still to produce in the current pass (Scaler_BeginPass sets, Scaler_RowEnd consumes in 32-row chunks) */
#ifdef XMEN2_MPW
extern s16 g_ScalerRowsLeft;
#else
#define g_ScalerRowsLeft XM_G(s16, 0xFFF5E6)
#endif
/* $FFF600 long: g_ScalerParams+$C (tiles wide/high) the step tables were built for; Scaler_BuildStepTables rebuilds them when it differs (Scaler_BeginPass sets -1) */
#ifdef XMEN2_MPW
extern s32 g_ScalerSizeKey;
#else
#define g_ScalerSizeKey XM_G(s32, 0xFFF600)
#endif
/* $FFF604 byte: cleared by Scaler_BeginPass when the table key changes; nothing reads or sets it. Not named in the disassembly yet (-$BFC). */
#ifdef XMEN2_MPW
extern u8 g_ScalerUnusedFlag;
#else
#define g_ScalerUnusedFlag XM_G(u8, 0xFFF604)
#endif
/* $FFF605 byte: last g_ScalerParams+$15 seen; a change resets g_ScalerSizeKey to -1 (Scaler_BeginPass; Scaler_InitParams sets $FF) */
#ifdef XMEN2_MPW
extern u8 g_ScalerTableKey;
#else
#define g_ScalerTableKey XM_G(u8, 0xFFF605)
#endif
/* $FFF606 word: output rows in full 4-tile sprite bands, (tilesH & ~3) * 8, or 1000 when tilesH is a multiple of 4 (Scaler_BuildStepTables); each pass counts g_ScalerRowCount down from it, the rows after it use g_ScalerLine3. Not named in the disassembly yet (-$BFA). */
#ifdef XMEN2_MPW
extern s16 g_ScalerFullRows;
#else
#define g_ScalerFullRows XM_G(s16, 0xFFF606)
#endif
/* $FFF608 ptr: walks the word offsets in g_ScalerLine4 added to the output pointer (Scaler_BeginPass sets; Scaler_ClearRows, Scaler_RowEnd step) */
#ifdef XMEN2_MPW
extern PTR(void) g_ScalerRowStep;
#else
#define g_ScalerRowStep XM_G(PTR(void), 0xFFF608)
#endif
/* $FFF60C ptr: walks the word offsets in g_ScalerLine1, one per output row, added to the source pointer (Scaler_BeginPass sets, Scaler_RowEnd steps) */
#ifdef XMEN2_MPW
extern PTR(void) g_ScalerColStep;
#else
#define g_ScalerColStep XM_G(PTR(void), 0xFFF60C)
#endif
/* $FFF610 ptr: g_ScalerWork + 0, first of six $280-byte scaler line/step tables (Scaler_Start sets; Scaler_BeginPass) */
#ifdef XMEN2_MPW
extern PTR(void) g_ScalerLine0;
#else
#define g_ScalerLine0 XM_G(PTR(void), 0xFFF610)
#endif
/* $FFF614 ptr: g_ScalerWork + $280 (Scaler_Start); Scaler_BeginPass starts the column step pointer g_ScalerColStep here */
#ifdef XMEN2_MPW
extern PTR(void) g_ScalerLine1;
#else
#define g_ScalerLine1 XM_G(PTR(void), 0xFFF614)
#endif
/* $FFF618 ptr: g_ScalerWork + $500 (Scaler_Start); read by Scaler_BuildStepTables/Scaler_ClearRows/Scaler_BeginPass */
#ifdef XMEN2_MPW
extern PTR(void) g_ScalerLine2;
#else
#define g_ScalerLine2 XM_G(PTR(void), 0xFFF618)
#endif
/* $FFF61C ptr: g_ScalerWork + $780 (Scaler_Start); read by Scaler_BuildStepTables/Scaler_ClearRows/Scaler_BeginPass */
#ifdef XMEN2_MPW
extern PTR(void) g_ScalerLine3;
#else
#define g_ScalerLine3 XM_G(PTR(void), 0xFFF61C)
#endif
/* $FFF620 ptr: g_ScalerWork + $A00: step table rebuilt by Scaler_BuildStepTables when the output size changes; start of g_ScalerRowStep (Scaler_BeginPass) */
#ifdef XMEN2_MPW
extern PTR(void) g_ScalerLine4;
#else
#define g_ScalerLine4 XM_G(PTR(void), 0xFFF620)
#endif
/* $FFF624 ptr: g_ScalerWork + $C80 (Scaler_Start; Scaler_BeginPass) */
#ifdef XMEN2_MPW
extern PTR(void) g_ScalerLine5;
#else
#define g_ScalerLine5 XM_G(PTR(void), 0xFFF624)
#endif
/* $FFF628 word: spawn records examined per frame (Spawn_InitRecords sets 5, CloneA1_RunScene 10; Level_FrameLoop) */
#ifdef XMEN2_MPW
extern s16 g_SpawnScanPerFrame;
#else
#define g_SpawnScanPerFrame XM_G(s16, 0xFFF628)
#endif
/* $FFF62A ptr: end of the spawn records = g_SpawnRecs + count*14, where each scan round restarts (Spawn_InitRecords, Level_FrameLoop) */
#ifdef XMEN2_MPW
extern PTR(void) g_SpawnRecEnd;
#else
#define g_SpawnRecEnd XM_G(PTR(void), 0xFFF62A)
#endif
/* $FFF62E ptr: spawn record being examined; walks backwards 14 bytes per step from g_SpawnRecEnd (Level_FrameLoop, Spawn_InitRecords) */
#ifdef XMEN2_MPW
extern PTR(void) g_SpawnScanPtr;
#else
#define g_SpawnScanPtr XM_G(PTR(void), 0xFFF62E)
#endif
/* $FFF632 word: spawn records left in the current round-robin scan; reloaded from g_SpawnRecCount at 0 (Level_FrameLoop; Spawn_InitRecords clears) */
#ifdef XMEN2_MPW
extern s16 g_SpawnScanLeft;
#else
#define g_SpawnScanLeft XM_G(s16, 0xFFF632)
#endif
/* $FFF634 ptr: when non-zero the level loop calls it instead of the spawn-record scan (Level_FrameLoop); Spawn_InitRecords sets Spawn_Nop when the level has no spawn records, else 0 */
#ifdef XMEN2_MPW
extern PTR(void) g_SpawnPassHook;
#else
#define g_SpawnPassHook XM_G(PTR(void), 0xFFF634)
#endif
/* $FFF638 word: H-interrupt counter (Irq_HBlank) */
#ifdef XMEN2_MPW
extern s16 g_HBlankCount;
#else
#define g_HBlankCount XM_G(s16, 0xFFF638)
#endif
/* $FFF63A 224 longs: per-line H-scroll {plane A, plane B} words, $380 bytes DMAed to VRAM $F400 (Scroll_UpdateParallax); a column descriptor's V-scroll longs per 2-cell column (Scroll_ComputeParallax fills, Scroll_SetParallax clears) */
#ifdef XMEN2_MPW
extern u32 g_HScrollBuf[224];
#else
#define g_HScrollBuf XM_GA(u32, 0xFFF63A)
#endif
/* $FFF9BA 32 band states of the current parallax descriptor (Scroll_SetParallax resets them, Scroll_ComputeParallax runs them) */
#ifdef XMEN2_MPW
extern ParallaxState g_ParallaxBands[32];
#else
#define g_ParallaxBands XM_GA(ParallaxState, 0xFFF9BA)
#endif
/* $FFFA3A byte: upload g_PalWork at the next V-blank (Pal_Update clears) */
#ifdef XMEN2_MPW
extern u8 g_PalWorkDirty;
#else
#define g_PalWorkDirty XM_G(u8, 0xFFFA3A)
#endif
/* $FFFA3C word: frames until the next fade step (Pal_Update) */
#ifdef XMEN2_MPW
extern s16 g_PalFadeTimer;
#else
#define g_PalFadeTimer XM_G(s16, 0xFFFA3C)
#endif
/* $FFFA3E ptr: fade component table, 8 bytes per level (Pal_StartFade) */
#ifdef XMEN2_MPW
extern PTR(void) g_PalFadeTable;
#else
#define g_PalFadeTable XM_G(PTR(void), 0xFFFA3E)
#endif
/* $FFFA42 word: palette line being faded, -1 = all four (Pal_ApplyFade) */
#ifdef XMEN2_MPW
extern s16 g_PalFadeLine;
#else
#define g_PalFadeLine XM_G(s16, 0xFFFA42)
#endif
/* $FFFA44 word: current fade level 0-7 (Pal_StartFade, Pal_Update) */
#ifdef XMEN2_MPW
extern s16 g_PalFadeLevel;
#else
#define g_PalFadeLevel XM_G(s16, 0xFFFA44)
#endif
/* $FFFA46 byte: 1 = fade level counts up, 0 = down (Pal_StartFade) */
#ifdef XMEN2_MPW
extern u8 g_PalFadeUp;
#else
#define g_PalFadeUp XM_G(u8, 0xFFFA46)
#endif
/* $FFFA48 word: scratch VDP address command low word for a DMA (VDP_DmaPalette) */
#ifdef XMEN2_MPW
extern s16 g_VdpCmdLo;
#else
#define g_VdpCmdLo XM_G(s16, 0xFFFA48)
#endif
/* $FFFA4A word: scratch VDP address command high word for a DMA (VDP_DmaPalette) */
#ifdef XMEN2_MPW
extern s16 g_VdpCmdHi;
#else
#define g_VdpCmdHi XM_G(s16, 0xFFFA4A)
#endif
/* $FFFA4C word: entries in g_DmaQueue (VDP_QueueDma, VDP_ResetDmaQueue) */
#ifdef XMEN2_MPW
extern s16 g_DmaQueueCount;
#else
#define g_DmaQueueCount XM_G(s16, 0xFFFA4C)
#endif
/* $FFFA4E word: byte index into Tbl_RandXor1, steps down by 2 and wraps to $1D0 (Rand_Next; Rand_Seed bits 10-15) */
#ifdef XMEN2_MPW
extern s16 g_RandIdx1;
#else
#define g_RandIdx1 XM_G(s16, 0xFFFA4E)
#endif
/* $FFFA50 word: byte index into Tbl_RandXor2, steps down by 2 and wraps to $12C (Rand_Next_Mix2; Rand_Seed bits 5-9) */
#ifdef XMEN2_MPW
extern s16 g_RandIdx2;
#else
#define g_RandIdx2 XM_G(s16, 0xFFFA50)
#endif
/* $FFFA52 word: byte index into Tbl_RandXor3, steps down by 2 and wraps to $FC (Rand_Next; Rand_Seed bits 0-4 x4) */
#ifdef XMEN2_MPW
extern s16 g_RandIdx3;
#else
#define g_RandIdx3 XM_G(s16, 0xFFFA52)
#endif
/* $FFFA54 word: last Rand_Next output (Rand_Seed stores the seed here) */
#ifdef XMEN2_MPW
extern s16 g_RandPrev;
#else
#define g_RandPrev XM_G(s16, 0xFFFA54)
#endif
/* $FFFA56 word: Rand_Next output before the last one (gets the old g_RandPrev each call; Rand_Seed zeroes it); XORed into the next value */
#ifdef XMEN2_MPW
extern s16 g_RandPrev2;
#else
#define g_RandPrev2 XM_G(s16, 0xFFFA56)
#endif
/* $FFFA58 word: pad 1 buttons read in V-int (Pad_ReadBoth/Pad_ReadPort: bits 0-3 UDLR, 4-7 B C A Start, 8-11 Z Y X Mode, $4000 6-button, $8000 none) */
#ifdef XMEN2_MPW
extern s16 g_Pad1Raw;
#else
#define g_Pad1Raw XM_G(s16, 0xFFFA58)
#endif
/* $FFFA5A word: pad 2 buttons read in V-int (same encoding as g_Pad1Raw; Pad_IsP2Connected tests bit 15) */
#ifdef XMEN2_MPW
extern s16 g_Pad2Raw;
#else
#define g_Pad2Raw XM_G(s16, 0xFFFA5A)
#endif
/* $FFFA5C array: V-blank DMA queue, $18-byte records of VDP register words (VDP_QueueDma, VDP_FlushDmaQueue) */
#ifdef XMEN2_MPW
extern DmaEntry g_DmaQueue[];
#else
#define g_DmaQueue XM_GA(DmaEntry, 0xFFFA5C)
#endif
/* $FFFFFC word: VDP mode register 3 command shadow $8Bxx, scroll modes (Sys_InitHardware, Scroll_SetParallax) */
#ifdef XMEN2_MPW
extern s16 g_VdpReg11;
#else
#define g_VdpReg11 XM_G(s16, 0xFFFFFC)
#endif
/* $FFFFFE byte ($FFFFFE): when set Sys_InitHardware enables external interrupts (VDP reg 11 = $8B08 instead of $8B00, port 3 control $A1000D = $80); Game_Main clears it at boot, so normally 0 */
#ifdef XMEN2_MPW
extern u8 g_ExtIntMode[];
#else
#define g_ExtIntMode XM_GA(u8, 0xFFFFFE)
#endif

#endif /* XMEN2_GLOBALS_H */
