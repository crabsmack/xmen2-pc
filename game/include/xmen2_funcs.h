/* xmen2_funcs.h - a prototype for every function of the disassembly (hand-written C, phase 4).
 *
 * The calling convention is MPW C's for all of them: every argument in a 4-byte slot,
 * pushed right to left, the caller pops; result in d0; d3-d7/a2-a4 preserved. C calls a
 * function by these names whether it is written in C yet or still generated code (the
 * bridge decides), so converting a callee never changes its callers. Functions marked
 * `fragment` are pieces the disassembly split off a C function (switch cases, blocks):
 * in C they are code inside their parent, never called. `asm` ones have no MPW frame.
 * The first draft's signatures are inferred from the frame accesses (arguments: their
 * count and widths; `void *` where the slot was loaded into an address register) and the
 * asm header's Out line; a function's C fixes its own prototype here.
 */
#ifndef XMEN2_FUNCS_H
#define XMEN2_FUNCS_H

#include "xmen2_types.h"

void unk_Boot_005458(void);	/* $005458 */
long Game_Main(void);	/* $005466 */
void Scene_RegisterAll(void);	/* $0054C6 */
void Game_ResetPlayers(void);	/* $005C7C */
void Game_SceneLoop(void);	/* $005CC6 */
void Game_AttractDemoNightcrawler(void);	/* $005E4E fragment */
void Game_AttractDemoCyclops(void);	/* $005E6C fragment */
void Game_PlayAttractDemo(void);	/* $005EB8 fragment */
void Player2_MakeHostile(void);	/* $005F18 */
void Player_SetCharacter(long, void *);	/* $005FA2 */
void Player1_SetBeast(void);	/* $0060B0 */
void Player1_SetWolverine(void);	/* $0060C8 */
void Player1_SetPsylocke(void);	/* $0060E0 */
void Player1_SetNightcrawler(void);	/* $0060F8 */
void Player1_SetGambit(void);	/* $006110 */
void Player1_SetCyclops(void);	/* $006128 */
void Player2_SetBeast(void);	/* $006140 */
void Player2_SetWolverine(void);	/* $006158 */
void Player2_SetPsylocke(void);	/* $006170 */
void Player2_SetNightcrawler(void);	/* $006188 */
void Player2_SetGambit(void);	/* $0061A0 */
void Player2_SetCyclops(void);	/* $0061B8 */
short Player_CharIndexOf(void *);	/* $0061D0 */
long Player1_StartLife(void);	/* $006242 */
void Player1_StartLife_Beast(void);	/* $0062AE fragment */
void Player1_StartLife_Gambit(void);	/* $0062BA fragment */
void Player1_StartLife_Nightcrawler(void);	/* $0062C0 fragment */
void Player1_StartLife_Psylocke(void);	/* $0062C6 fragment */
void Player1_StartLife_Wolverine(void);	/* $0062CC fragment */
long Player2_StartLife(void);	/* $0062E4 */
void Player2_StartLife_Beast(void);	/* $006350 fragment */
void Player2_StartLife_Psylocke(void);	/* $006368 fragment */
void Player2_StartLife_Wolverine(void);	/* $00636E fragment */
void HUD_UpdateHealthBars(void);	/* $006386 */
void HUD_Redraw(void);	/* $006584 */
void Game_Init(void);	/* $006636 */
short Player_CalcMaxTiles(void);	/* $006672 */
void Scene_AllocCommonVram(void);	/* $006A20 */
void Game_InitButtonMasks(void);	/* $006ABA */
void Scene_PreLoadReset(void);	/* $006B2C */
void Scene_PostLoadVram(void);	/* $006B64 */
void Players_PlaceAtStart(long, long, long, long);	/* $006BD0 */
void AvalonB1_InitIntroPalette(void);	/* $00F59A */
void AvalonB1_RestorePalette(void);	/* $00F60E */
void AvalonB1_PalFadeStep(short, long);	/* $00F648 */
void AvalonB1_CamHookWait2(void);	/* $00F778 */
void AvalonB1_CamHookWait60(void);	/* $00F80A */
void AvalonB1_CamHookScroll120(void);	/* $00F896 */
void AvalonB1_CamHookFadeIn(void);	/* $00F942 */
void AvalonB1_CamHookFinish(void *);	/* $00FA24 */
void AvalonB1_InitCamera(void);	/* $00FB54 */
void AvalonB1_ScrollCameraTo(short, short);	/* $00FC12 */
void AvalonB1_CyclePalettes(void);	/* $00FCA2 */
long AvalonB1_TileHook(short, short);	/* $00FCE2 */
void AvalonB1_InstallTileHooks(void);	/* $00FD44 */
void AvalonB1_FrameHook(void);	/* $00FD78 */
void AvalonB1_RunScene(void);	/* $00FDAA */
void BigAcolyte_RenderScaled(ScalePicture *pic, ScaleParams *sp, short vram, char first);	/* $00FEF6 */
long BigAcBullet_GetSize(void);	/* $00FFAE */
void BigAcBullet_Init(void *, long, long, long);	/* $00FFBC */
void BigAcBullet_Update(void *);	/* $010050 */
void BigAcBullet_Attach(void *, void *owner, short, short, short);	/* $010098 */
long BigAcBullet_StateLaunch(void *);	/* $0100D4 */
void BigAcBullet_StateFly(void *);	/* $01014C */
long BigAcolyte_GetSize(void);	/* $0101CE */
void BigAcolyte_Nop6A(BigAcolyte *self);	/* $0101DC */
void BigAcolyte_ClassInit(void);	/* $0101E8 */
void BigAcolyte_Init(BigAcolyte *self, long x, long y, Placement *pl);	/* $0101F4 */
void BigAcolyte_Activate(BigAcolyte *self);	/* $01033E */
void BigAcolyte_Deactivate(BigAcolyte *self);	/* $01038A */
char BigAcolyte_OutsideActiveWindow(BigAcolyte *self);	/* $0103B6 */
void BigAcolyte_OnDeath(BigAcolyte *self);	/* $0103F0 */
void BigAcolyte_Move(BigAcolyte *self, short steps);	/* $01042C */
void BigAcolyte_Draw(BigAcolyte *self);	/* $0106B8 */
void BigAcolyte_TakeDamage(BigAcolyte *self, Actor *attacker, char type, char damage, short hx, short hy);	/* $0107EA */
void BigAcolyte_SetTargetY(BigAcolyte *self, short y);	/* $0108CE */
void BigAcolyte_SetTargetX(BigAcolyte *self, short x);	/* $01091E */
void BigAcolyte_LoadSprite(void);	/* $01096E */
void BigAcolyte_ActAppear(BigAcolyte *self);	/* $01098E */
void BigAcolyte_DieEndScene(BigAcolyte *self);	/* $010B00 */
short BigAcolyte_ActWaitPlayers(BigAcolyte *self);	/* $010B3C */
short BigAcolyte_ActMoveToWall(BigAcolyte *self);	/* $010D76 */
short BigAcolyte_StateWaitArrive(BigAcolyte *self);	/* $010DB2 */
short BigAcolyte_ActGrow(BigAcolyte *self);	/* $010E26 */
short BigAcolyte_StateBurstWall(BigAcolyte *self);	/* $010EE6 */
short BigAcolyte_StateBecomeSprite(BigAcolyte *self);	/* $011238 */
void BigAcolyte_StateJoinList(BigAcolyte *self);	/* $0112E8 */
void BigAcolyte_ActStartFight(BigAcolyte *self);	/* $01134E */
short BigAcolyte_StateWaitYShoot(BigAcolyte *self);	/* $0113A6 */
short BigAcolyte_StateWaitYNext(BigAcolyte *self);	/* $0113E6 */
short BigAcolyte_StateShootAnim(BigAcolyte *self);	/* $011452 */
short BigAcolyte_StateShoot(BigAcolyte *self);	/* $011488 */
short BigAcolyte_ActRise2(BigAcolyte *self);	/* $011598 */
short BigAcolyte_ActRise1(BigAcolyte *self);	/* $0115EA */
short BigAcolyte_StateCheckPlayersLow(BigAcolyte *self);	/* $01163C */
short BigAcolyte_ActGoTop(BigAcolyte *self);	/* $011702 */
short BigAcolyte_ActGoLowShoot(BigAcolyte *self);	/* $011736 */
short BigAcolyte_ActGoMidShoot(BigAcolyte *self);	/* $01176C */
short BigAcolyte_ActGoMid2(BigAcolyte *self);	/* $0117A2 */
short BigAcolyte_ActGoLow1(BigAcolyte *self);	/* $0117D8 */
void BigAcolyte_ActWait30(BigAcolyte *self);	/* $01180E */
void BigAcolyte_ActWait60(BigAcolyte *self);	/* $011862 */
void BigAcolyte_NopStatic(void);	/* $0118B6 */
void BigAcolyte_IntroScript(void);	/* $0119E6 fragment */
void BigAcolyte_FightScript(void);	/* $011A00 fragment */
long CapedAcolyte_GetSize(void);	/* $013342 */
void CapedAcolyte_Nop6A(void);	/* $013350 */
void CapedAcolyte_ClassInit(void);	/* $013358 */
void CapedAcolyte_Init(void *, long, long, void *);	/* $013360 */
void CapedAcolyte_Move(void *, short);	/* $013596 */
void CapedAcolyte_OnDeath(void *, char, char);	/* $013698 */
void CapedAcolyte_EndHurt(void *);	/* $0136F0 */
void CapedAcolyte_OnHurt(void *, long, char);	/* $013722 */
void CapedAcolyte_OnHurt_KnockY(void);	/* $01376A fragment */
void CapedAcolyte_OnHurt_KnockRight(void);	/* $013782 fragment */
void CapedAcolyte_OnHurt_SetDecel(void);	/* $013788 fragment */
long CapedAcolyte_StateKnockback(void *);	/* $0137C4 */
long CapedAcolyte_StateKnockbackRun(void *);	/* $0137EE */
void CapedAcolyte_StateReturnHome(void *);	/* $0138D4 */
void CapedAcolyte_ActStartMainScript(void *);	/* $01396E */
long CapedAcolyte_ActFacePlayer(void *);	/* $0139BE */
long CapedAcolyte_ActShootBurst(void *);	/* $013A5C */
long CapedAcolyte_StateShoot(void *);	/* $013A86 */
void CapedAcolyte_StateHoverNearPlayer(void *);	/* $013BAE */
long CapedAcolyte_ActHover(void *);	/* $013D26 */
void CapedAcolyte_ActAnimHigh(void *);	/* $013D50 */
void CapedAcolyte_ActAnimLow(void *);	/* $013DB0 */
void CapedAcolyte_IntroScript(void);	/* $013EC6 fragment */
void CapedAcolyte_MainScript(void);	/* $013ED4 fragment */
void CapedAcolyte_MainScriptEnd(void);	/* $013EE4 fragment */
long FarAcolyte_GetSize(void);	/* $017218 */
void FarAcolyte_Nop66(FarAcolyte *self);	/* $017226 */
void FarAcolyte_Nop6A(FarAcolyte *self);	/* $01723C */
void FarAcolyte_ClassInit(void);	/* $017244 */
void FarAcolyte_Init(FarAcolyte *self, long x, long y, Placement *pl);	/* $017274 */
long FarAcolyte_OutsideActiveWindow(FarAcolyte *self);	/* $01738E */
void FarAcolyte_Draw(FarAcolyte *self);	/* $017506 */
void FarAcolyte_Move(FarAcolyte *self, short steps);	/* $017706 */
void FarAcolyte_StartOrb(FarAcolyte *self);	/* $0178C6 */
char FarAcolyte_MoveOrb(FarAcolyte *self);	/* $01790E */
void FarAcolyte_StartBolt(FarAcolyte *self);	/* $0179F8 */
char FarAcolyte_MoveBolt(FarAcolyte *self);	/* $017A72 */
short FarAcolyte_StateWaitOnScreen(FarAcolyte *self);	/* $017AF4 */
short FarAcolyte_StateAppear(FarAcolyte *self);	/* $017BB6 */
short FarAcolyte_StateWaitPlayer(FarAcolyte *self);	/* $017BEC */
short FarAcolyte_StateOrbChase(FarAcolyte *self);	/* $017C60 */
short FarAcolyte_StateCast(FarAcolyte *self);	/* $017CEA */
short FarAcolyte_StateCastEnd(FarAcolyte *self);	/* $017D2C */
short FarAcolyte_StateBoltFly(FarAcolyte *self);	/* $017D60 */
void FarAcolyte_StateDie(FarAcolyte *self);	/* $017DDC */
long ShootAcolyte_GetSize(void);	/* $017FD6 */
void ShootAcolyte_Nop66(void *);	/* $017FE4 */
void ShootAcolyte_Nop6A(void);	/* $017FFA */
void ShootAcolyte_ClassInit(void);	/* $018002 */
void ShootAcolyte_Init(void *, long, long, void *);	/* $01800A */
void ShootAcolyte_Move(void *, short);	/* $01818A */
void ShootAcolyte_ActRestartScript(void *);	/* $0181F8 */
void ShootAcolyte_ActWait30(void *);	/* $01823C */
void ShootAcolyte_ActFacePlayer(void *);	/* $018290 */
long ShootAcolyte_ActWalk(void *);	/* $01830A */
long ShootAcolyte_StateWalkToPlayer(void *);	/* $018344 */
long ShootAcolyte_ActShootBurst(void *);	/* $0184A4 */
long ShootAcolyte_StateAim(void *);	/* $0184CE */
long ShootAcolyte_StateFire(void *);	/* $018504 */
void ShootAcolyte_StateNextShot(void *);	/* $0185BE */
void ShootAcolyte_Script(void);	/* $0186FA fragment */
void ShootAcolyte_ScriptEnd(void);	/* $018706 fragment */
long Tower_GetSize(void);	/* $01AAA2 */
void Tower_ClassInit(void);	/* $01AAB0 */
void Tower_Init(void *, long, long, void *);	/* $01AABC */
long Tower_StateIdle(void *);	/* $01AB96 */
void Tower_NopStatic(void);	/* $01ABBC */
long YoyoAcolyte_GetSize(void);	/* $01BFF8 */
void YoyoAcolyte_Nop66(void *);	/* $01C006 */
void YoyoAcolyte_Nop6A(void);	/* $01C01C */
void YoyoAcolyte_ClassInit(void);	/* $01C024 */
void YoyoAcolyte_Init(void *, long, long, void *);	/* $01C042 */
void YoyoAcolyte_Init_Script0(void);	/* $01C176 fragment */
void YoyoAcolyte_Init_Script1to4(void);	/* $01C182 fragment */
void YoyoAcolyte_Init_SetScript(void);	/* $01C1B0 fragment */
void YoyoAcolyte_Update(void *);	/* $01C1DA */
long YoyoAcolyte_OutsideActiveWindow(void *);	/* $01C24C */
void YoyoAcolyte_Draw(void *);	/* $01C282 */
long YoyoAcolyte_ThrowYoyo(void *, short);	/* $01C552 */
long YoyoAcolyte_ActIdle100(void *);	/* $01C612 */
long YoyoAcolyte_StateIdleRestart(void *);	/* $01C64C */
long YoyoAcolyte_ActIdle(void *);	/* $01C6BE */
long YoyoAcolyte_StateIdleNext(void *);	/* $01C6E8 */
long YoyoAcolyte_ActThrowLevel(void *);	/* $01C77C */
long YoyoAcolyte_StateThrowLevel(void *);	/* $01C7B2 */
long YoyoAcolyte_ActThrowHigh(void *);	/* $01C7E6 */
long YoyoAcolyte_StateThrowHigh(void *);	/* $01C81C */
long YoyoAcolyte_ActThrowLow(void *);	/* $01C850 */
long YoyoAcolyte_StateThrowLow(void *);	/* $01C886 */
long YoyoAcolyte_StateYoyoOut(void *);	/* $01C8BA */
void YoyoAcolyte_StateDie(void *);	/* $01C94A */
void YoyoAcolyte_ScriptLevel(void);	/* $01CA50 fragment */
void YoyoAcolyte_ScriptHigh(void);	/* $01CA5A fragment */
void YoyoAcolyte_ScriptLow(void);	/* $01CA64 fragment */
void YoyoAcolyte_ScriptMixA(void);	/* $01CA6E fragment */
void YoyoAcolyte_ScriptMixB(void);	/* $01CA88 fragment */
void AvalonB2_CamHook(void *);	/* $0226B2 */
void AvalonB2_AnimSprite(void);	/* $0226D0 */
void AvalonB2_FrameHook(void);	/* $0226F4 */
void AvalonB2_InitSprite(void);	/* $022716 */
long AvalonB2_TileHook(short, short);	/* $022742 */
void AvalonB2_InstallTileHooks(void);	/* $0227A4 */
void AvalonB2_NopStatic(void);	/* $0227D8 */
void AvalonB2_RunScene(void);	/* $0227E0 */
void AvalonA2_EmptyStub1(void);	/* $0228C2 */
void AvalonA2_ResetCounters(void);	/* $0228CA */
void AvalonA2_DisabledFrameEffect(void);	/* $0228DC */
void AvalonA2_DisabledFrameEffect_exit(void);	/* $022958 fragment */
long AvalonA2_ToggleTileHook(short, short);	/* $02295C */
void AvalonA2_InstallTileHooks(void);	/* $0229E8 */
void AvalonA2_FrameHook(void);	/* $022A1C */
void AvalonA2_LoadLevel(void);	/* $022A4E */
void AvalonA2_EmptyStub2(void);	/* $022ADA */
void AvalonA2_RaiseCameraFloor(void);	/* $022AE2 */
void AvalonA2_PlacePlayers(void);	/* $022AF6 */
long AvalonA2_HookReturn0(short);	/* $022B18 */
long AvalonA2_HookEndOnEvent3(short);	/* $022B2E */
void AvalonA2_InstallEventHooks(void);	/* $022B4E */
void AvalonA2_CallEmptyStubs(void);	/* $022B66 */
void AvalonA2_EmptyStub3(void);	/* $022B76 */
void AvalonA2_InstallHooks(void);	/* $022B7E */
void AvalonA2_RunScene(void);	/* $022BA2 */
long RetractFloor_GetSize(void);	/* $0268FE */
long RetractFloor_TriggerNearest(short);	/* $02690C */
void RetractFloor_Nop66(void *);	/* $0269CE */
void RetractFloor_Nop6A(void);	/* $0269E4 */
void RetractFloor_ClassInit(void);	/* $0269EC */
void RetractFloor_Init(void *, long, long, long);	/* $026A0A */
void RetractFloor_Draw(void *);	/* $026AA0 */
void RetractFloor_StartRetract(void *);	/* $026CFA */
long RetractFloor_StateInit(void *);	/* $026D8E */
long RetractFloor_StateWait(void *);	/* $026DB2 */
long RetractFloor_StateDrop(void *);	/* $026E10 */
long RetractFloor_StateSlide(void *);	/* $026ED8 */
void RetractFloor_StateKill(void *);	/* $026F2E */
long Zapper_GetSize(void);	/* $027230 */
void Zapper_Nop66(void *);	/* $02723E */
void Zapper_Nop6A(void);	/* $027254 */
void Zapper_ClassInit(void);	/* $02725C */
void Zapper_Init(void *, long, long, void *);	/* $02727A */
long Zapper_StateLoopAnim0(void *);	/* $027352 */
long Zapper_StateLoopAnim1(void *);	/* $027388 */
long Zapper_StateLoopAnim2(void *);	/* $0273BE */
void AvalonA3_EmptyStub2(void);	/* $02C902 */
void AvalonA3_PsiFlashStep(void);	/* $02C90A */
void AvalonA3_FrameHook(void);	/* $02C9AA */
void AvalonA3_LoadLevel(void);	/* $02CAF0 */
void AvalonA3_PlacePlayers(void);	/* $02CBA8 */
void AvalonA3_CamHook(void *);	/* $02CBBE */
void AvalonA3_InitCamera(void);	/* $02CBD0 */
void AvalonA3_SavePalettes(long, long);	/* $02CBF6 */
void AvalonA3_InitSprites(void);	/* $02CC5C */
void AvalonA3_EmptyStub3(void);	/* $02CD30 */
void AvalonA3_InstallHooks(void);	/* $02CD38 */
void AvalonA3_EmptyStub1(void);	/* $02CD48 */
void AvalonA3_RunScene(void);	/* $02CD50 */
long Exodus_GetSize(void);	/* $02CD96 */
long Exodus_StateStart(void *);	/* $02CDA4 */
long Exodus_StateDescend(void *);	/* $02CDCA */
long Exodus_StateWaitLanded(void *);	/* $02CE1A */
void Exodus_NextPattern(void *);	/* $02CE52 */
void Exodus_NextPattern_part1(void);	/* $02CE94 fragment */
void Exodus_NextPattern_part2(void);	/* $02CEBC fragment */
void Exodus_NextPattern_part3(void);	/* $02CEF6 fragment */
long Exodus_StateNextAttack(void *);	/* $02CF04 */
void Exodus_StateNextAttack_part1(void);	/* $02CF34 fragment */
void Exodus_StateNextAttack_part2(void);	/* $02CF64 fragment */
void Exodus_StateNextAttack_part3(void);	/* $02CFE8 fragment */
void Exodus_StateNextAttack_part4(void);	/* $02CFF4 fragment */
void Exodus_StateNextAttack_part5(void);	/* $02D0B8 fragment */
void Exodus_StateNextAttack_part6(void);	/* $02D13E fragment */
void Exodus_HurtPlayer(void *);	/* $02D146 */
long Exodus_AtkBlast17(void *);	/* $02D1A6 */
long Exodus_AtkBlast2(void *);	/* $02D21C */
long Exodus_AtkBlast3(void *);	/* $02D27A */
long Exodus_AtkBlast4(void *);	/* $02D2D8 */
long Exodus_AtkSwoopA(void *);	/* $02D336 */
long Exodus_AtkSwoopB(void *);	/* $02D372 */
long Exodus_StateSwoopStart(void *);	/* $02D3AE */
long Exodus_StateSwoopToY(void *);	/* $02D456 */
long Exodus_StateSwoopBlast(void *);	/* $02D4BA */
long Exodus_StateRiseStart(void *);	/* $02D522 */
long Exodus_StateRise(void *);	/* $02D552 */
long Exodus_StateRiseUntil(void *);	/* $02D5EA */
long Exodus_StateRiseEnd(void *);	/* $02D61C */
long Exodus_StateVulnerable(void *);	/* $02D658 */
long Exodus_StateEndVulnerable(void *);	/* $02D6D8 */
void Exodus_StateWaitBlastsGone(void *);	/* $02D720 */
void Exodus_NopStatic(void);	/* $02D756 */
void Exodus_InitSpawnRec(void *, long);	/* $02D75E */
void Exodus_ClassInit(long);	/* $02D78E */
void Exodus_Nop66(void *);	/* $02D7B2 */
void Exodus_Move(void *);	/* $02D7C8 */
void Exodus_Draw(void *);	/* $02D91C */
void Exodus_HurtOnHotTiles(void *);	/* $02D96C */
void Exodus_Update(void *);	/* $02DA16 */
void Exodus_TakeDamage(void *, long, long, char);	/* $02DADE */
void Exodus_TakeDamage_part1(void);	/* $02DB62 fragment */
void Exodus_TakeDamage_part2(void);	/* $02DB70 fragment */
void Exodus_TakeDamage_part3(void);	/* $02DB84 fragment */
void Exodus_TakeDamage_part4(void);	/* $02DB8E fragment */
void Exodus_TakeDamage_part5(void);	/* $02DB98 fragment */
void Exodus_Init(void *, long, long, void *);	/* $02DBCE */
void Exodus_StartPsiFlash(void);	/* $02DC80 */
long Exodus_DropPlayerFromSeg(void *, long);	/* $02DC9E */
void Exodus_BreakFloorTiles(char);	/* $02DCE4 */
void Exodus_CheckFloorHit(void);	/* $02DDDC */
long ExodusBlast_GetSize(void);	/* $02DF96 */
long ExodusBlast_StateStart(void *);	/* $02DFA4 */
long ExodusBlast_StateLaunch(void *);	/* $02DFD8 */
void ExodusBlast_StateLaunch_part1(void);	/* $02E026 fragment */
void ExodusBlast_StateLaunch_part2(void);	/* $02E04E fragment */
void ExodusBlast_StateLaunch_part3(void);	/* $02E062 fragment */
void ExodusBlast_StateLaunch_part4(void);	/* $02E0B0 fragment */
long ExodusBlast_LaunchRight(void *);	/* $02E0B8 */
long ExodusBlast_LaunchLeft(void *);	/* $02E0F0 */
long ExodusBlast_LaunchDownLeft(void *);	/* $02E128 */
long ExodusBlast_LaunchDownRight(void *);	/* $02E168 */
long ExodusBlast_LaunchDown(void *);	/* $02E1A8 */
long ExodusBlast_LaunchDropA(void *);	/* $02E1E0 */
long ExodusBlast_LaunchDropB(void *);	/* $02E218 */
void ExodusBlast_StateFlyRight(void *);	/* $02E250 */
void ExodusBlast_StateFlyLeft(void *);	/* $02E282 */
void ExodusBlast_StateFlyDown(void *);	/* $02E2B4 */
void ExodusBlast_StateDie(void *);	/* $02E2E6 */
void ExodusBlast_NopStatic(void);	/* $02E306 */
void ExodusBlast_ClassInit(long);	/* $02E30E */
void ExodusBlast_Draw(void *);	/* $02E332 */
void ExodusBlast_Nop66(void *);	/* $02E492 */
void ExodusBlast_Init(void *, long, long, void *);	/* $02E4A8 */
long ExodusBlast_Spawn(long, long, char);	/* $02E526 */
void AvalonA4_NopStatic(void);	/* $034D4E */
void AvalonA4_ResetEffectCounters(void);	/* $034D56 */
long AvalonA4_AnimatePalettes(void);	/* $034D7A */
void AvalonA4_AnimatePalettes_part1(void);	/* $034DF8 fragment */
void AvalonA4_AnimatePalettes_part2(void);	/* $034EA0 fragment */
void AvalonA4_AnimatePalettes_part3(void);	/* $034EC6 fragment */
void AvalonA4_AnimatePalettes_part4(void);	/* $034F62 fragment */
void AvalonA4_AnimatePalettes_part5(void);	/* $034FDC fragment */
void AvalonA4_AnimatePalettes_part6(void);	/* $035174 fragment */
long AvalonA4_SwitchTileHook(short, short);	/* $03518C */
long AvalonA4_GlobCrackBlockHook(short, short, void *);	/* $0351F8 */
long AvalonA4_GlobCrackColumnHook(short, short, void *);	/* $035322 */
long AvalonA4_CrackFloorHook(short, short, void *);	/* $0355A6 */
void AvalonA4_InstallTileHooks(void);	/* $035710 */
void AvalonA4_FrameHook(void);	/* $035750 */
void AvalonA4_LoadLevel(void);	/* $035782 */
void AvalonA4_CamHook(void *);	/* $035802 */
void AvalonA4_InitCamera(void);	/* $03582E */
void AvalonA4_PlacePlayers(void);	/* $03585C */
void AvalonA4_ClearCameraZone(short);	/* $035872 */
void AvalonA4_ClearCameraZone_case0(void);	/* $03589C fragment */
void AvalonA4_ClearCameraZone_case3(void);	/* $0358A8 fragment */
void AvalonA4_ClearCameraZone_case4(void);	/* $0358AC fragment */
void AvalonA4_ClearCameraZone_case5(void);	/* $0358B0 fragment */
void AvalonA4_ClearCameraZone_apply(void);	/* $0358B2 fragment */
void AvalonA4_SetCameraZone(short, short, short, short, short);	/* $0358D8 */
void AvalonA4_SetCameraZone_case0(void);	/* $035956 fragment */
void AvalonA4_SetCameraZone_case3(void);	/* $035962 fragment */
void AvalonA4_SetCameraZone_case4(void);	/* $035966 fragment */
void AvalonA4_SetCameraZone_apply(void);	/* $03596C fragment */
long AvalonA4_EventZoneFree0(short);	/* $03597A */
long AvalonA4_EventZoneFree1(short);	/* $03599A */
long AvalonA4_EventZoneFree2(short);	/* $0359BA */
long AvalonA4_EventZoneFree3(short);	/* $0359DA */
long AvalonA4_EventZoneFree4(short);	/* $0359FA */
long AvalonA4_EventZoneFree5(short);	/* $035A1A */
void AvalonA4_InstallEventHooks(void);	/* $035A3A */
void AvalonA4_EmptyStub1(void);	/* $035A76 */
void AvalonA4_EmptyStub2(void);	/* $035A7E */
void AvalonA4_InstallHooks(void);	/* $035A86 */
void AvalonA4_RunScene(void);	/* $035A9E */
long MagnetoBoss_GetSize(void);	/* $035BDA */
long MagnetoBoss_RubbleTimer(void *, long);	/* $035BE8 */
void MagnetoBoss_SpawnRubble(short, short, short, short, char, short, short);	/* $035DC2 */
void MagnetoBoss_Nop66(void *);	/* $03603E */
void MagnetoBoss_Nop6A(void);	/* $036054 */
void MagnetoBoss_ClassInit(long, short);	/* $03605C */
void MagnetoBoss_Init(void *, long, long, void *);	/* $0360A6 */
void MagnetoBoss_Activate(void *);	/* $03628E */
void MagnetoBoss_Update(void *);	/* $0363A2 */
void MagnetoBoss_Move(void *, short);	/* $03645C */
void MagnetoBoss_Move_part1(void);	/* $03653E fragment */
void MagnetoBoss_Move_part2(void);	/* $0366E6 fragment */
void MagnetoBoss_TakeDamage(void *, long, char, char);	/* $036730 */
long MagnetoBoss_Return0V6E(void);	/* $0367B6 */
long MagnetoBoss_SmashSegment(void *, short);	/* $0367C0 */
long MagnetoBoss_SmashSegmentStaggered(void *, short);	/* $0368D0 */
long MagnetoBoss_CrackWall(short, short, short, char);	/* $0369F0 */
void MagnetoBoss_EnterSpinState(void *);	/* $036AC6 */
void MagnetoBoss_EnterOutroState(void *);	/* $036B10 */
short MagnetoBoss_DirFromTo(void *, void *);	/* $036B34 */
long MagnetoBoss_StateIntroStart(void *);	/* $036C0A */
long MagnetoBoss_StateIntroShake1(void *);	/* $036C58 */
void MagnetoBoss_StateIntroWait1(void *);	/* $036C9E */
long MagnetoBoss_StateIntroShake2(void *);	/* $036CCA */
void MagnetoBoss_StateIntroWait2(void *);	/* $036D10 */
long MagnetoBoss_StateIntroShake3(void *);	/* $036D3C */
void MagnetoBoss_StateIntroWait3(void *);	/* $036DC4 */
long MagnetoBoss_StateBurstIn(void *);	/* $036DF0 */
long MagnetoBoss_Zone0Setup(void *);	/* $036E82 */
void MagnetoBoss_StateZone0(void *);	/* $036EC0 */
long MagnetoBoss_Zone1Setup(void *);	/* $036F50 */
long MagnetoBoss_StateZone1(void *);	/* $036F9A */
long MagnetoBoss_Zone1Exit(void *);	/* $0370CC */
long MagnetoBoss_StateFlyToSmash1(void *);	/* $0370FE */
long MagnetoBoss_Zone2Setup(void *);	/* $03713C */
void MagnetoBoss_StateZone2Approach(void *);	/* $037180 */
long MagnetoBoss_Zone2Hover(void *);	/* $037210 */
long MagnetoBoss_StateZone2(void *);	/* $03725A */
long MagnetoBoss_Zone2Exit(void *);	/* $03738C */
long MagnetoBoss_StateFlyToSmash2(void *);	/* $0373BE */
long MagnetoBoss_StateSmash2b(void *);	/* $0373FE */
long MagnetoBoss_Zone4Setup(void *);	/* $03742E */
void MagnetoBoss_StateZone4Approach(void *);	/* $037472 */
long MagnetoBoss_StateResumeShoot(void *);	/* $03750A */
long MagnetoBoss_Zone4Hover(void *);	/* $03755A */
long MagnetoBoss_StateZone4(void *);	/* $0375A4 */
long MagnetoBoss_Zone4Exit(void *);	/* $0376C2 */
long MagnetoBoss_StateBreakWall(void *);	/* $037700 */
long MagnetoBoss_StateFlyToFinal(void *);	/* $037772 */
void MagnetoBoss_StateWaitCamera(void *);	/* $0377D6 */
long MagnetoBoss_FinalSetup(void *);	/* $03781C */
void MagnetoBoss_StateFinalFight(void *);	/* $03784E */
long MagnetoBoss_StateShoot(void *);	/* $0378A8 */
void MagnetoBoss_StateAimShoot(void *);	/* $037924 */
void MagnetoBoss_StateShootRecover(void *);	/* $03795E */
void MagnetoBoss_StateSpin(void *);	/* $03799C */
long MagnetoBoss_StateOutro(void *);	/* $0379CA */
void MagnetoBoss_StateOutroLoop(long);	/* $037A00 */
long ThronePhone_GetSize(void);	/* $037A96 */
void ThronePhone_Nop66(void *);	/* $037AA4 */
void ThronePhone_Nop6A(void);	/* $037ABA */
void ThronePhone_ClassInit(void);	/* $037AC2 */
void ThronePhone_Init(void *, long, long, long);	/* $037AE0 */
long ThronePhone_SetMode(void *, short);	/* $037B74 */
void ThronePhone_Draw(void *);	/* $037BFE */
long ThronePhone_StateInit(void *);	/* $037E58 */
long ThronePhone_StateWaitMagneto(void *);	/* $037E82 */
long ThronePhone_StateCatch(void *);	/* $037F26 */
long ThronePhone_StatePullIn(void *);	/* $037F7E */
void ThronePhone_StateDone(void);	/* $038036 */
void BaniMazaA1_EmptyStub(void);	/* $0383D4 */
void BaniMazaA1_LoadLevel(void);	/* $0383DC */
void BaniMazaA1_InitSprites(void);	/* $03845C */
long BaniMazaA1_SwitchTileHook(short, short);	/* $038742 */
void BaniMazaA1_InstallTileHooks(void);	/* $0387D0 */
void BaniMazaA1_InitTablePtrs(void);	/* $038808 */
void BaniMazaA1_UploadBlockTiles(void *, short);	/* $038838 */
void BaniMazaA1_AnimateBlocks(void);	/* $0388D4 */
void BaniMazaA1_FrameHook(void);	/* $0389B6 */
void BaniMazaA1_FrameHook_PulseColor(void);	/* $038A0E fragment */
void BaniMazaA1_FrameHook_tail(void);	/* $038A58 fragment */
void BaniMazaA1_CamHook(void *);	/* $038A6C */
void BaniMazaA1_InitCamera(void);	/* $038A7E */
void BaniMazaA1_InitPalette(void);	/* $038AA4 */
void BaniMazaA1_InstallFrameHook(void);	/* $038AEE */
void BaniMazaA1_PlacePlayers(void);	/* $038AFE */
void BaniMazaA1_InitScroll(void);	/* $038BAC */
void BaniMazaA1_RunScene(void);	/* $038BFA */
long BanimazaDoor_GetSize(void);	/* $039D16 */
long BanimazaDoor_PushCallback(short, void *, void *, void *);	/* $039D24 */
void BanimazaDoor_InitDebris(void *, short, long, long, char);	/* $039DBA */
void BanimazaDoor_DrawDebris(void *, short, short);	/* $039E94 */
long BanimazaDoor_PlayerClear(void *, void *);	/* $039F80 */
long BanimazaDoor_StateReset(void *);	/* $03A01C */
long BanimazaDoor_StateWaitPlayer(void *);	/* $03A05A */
long BanimazaDoor_StateStartRise(void *);	/* $03A0AA */
long BanimazaDoor_StateRising(void *);	/* $03A11A */
void BanimazaDoor_StateRaised(void *);	/* $03A156 */
long BanimazaDoor_Shatter(void *);	/* $03A172 */
long BanimazaDoor_StateShattered(void *);	/* $03A210 */
long BanimazaDoor_StateWaitRespawn(void *);	/* $03A246 */
void BanimazaDoor_NopStatic(void);	/* $03A29E */
void BanimazaDoor_ClassInit(long);	/* $03A2A6 */
void BanimazaDoor_DrawColumn(void *);	/* $03A2F4 */
void BanimazaDoor_Draw(void *);	/* $03A4EE */
void BanimazaDoor_Draw_part1(void);	/* $03A584 fragment */
void BanimazaDoor_Draw_part2(void);	/* $03A5B4 fragment */
void BanimazaDoor_TakeDamage(void *, long, long, char);	/* $03A5BE */
long BanimazaDoor_InActivateWindow(void *);	/* $03A632 */
long BanimazaDoor_OutsideActiveWindow(void *);	/* $03A69A */
void BanimazaDoor_Nop66(void *);	/* $03A714 */
void BanimazaDoor_Init(void *, long, long, void *);	/* $03A72A */
long RollingDiscs_GetSize(void);	/* $03A86C */
void RollingDiscs_InitStub(void);	/* $03A87A */
void RollingDiscs_UpdateScroll(void);	/* $03A882 */
long RollingDiscs_ReverseAtY(short);	/* $03ABCE */
void RollingDiscs_NopStatic(void);	/* $03AC36 */
void RollingDiscs_ClassInit(long);	/* $03AC3E */
void RollingDiscs_Init(void *, long, long, void *);	/* $03AD14 */
void RollingDiscs_Update(void *);	/* $03ADC2 */
void RollingDiscs_Move(long, short);	/* $03ADEE */
void RollingDiscs_Draw(void);	/* $03AE14 */
void RollingDiscs_Nop66(void *);	/* $03AE1C */
void BaniMazaA2_InitDebrisBuf(void *);	/* $0472CC */
void BaniMazaA2_UpdateDebrisGroup(void *);	/* $0472E0 */
void BaniMazaA2_UpdateDebris(void);	/* $0473D4 */
void BaniMazaA2_NopStatic(void);	/* $047402 */
void BaniMazaA2_ResetCounters(void);	/* $04740A */
void BaniMazaA2_AnimatePalettes(void);	/* $04744E */
void BaniMazaA2_AnimatePalettes_cases(void);	/* $04755E fragment */
void BaniMazaA2_AnimatePalettes_case3(void);	/* $047572 fragment */
void BaniMazaA2_AnimatePalettes_case4(void);	/* $04757C fragment */
void BaniMazaA2_AnimatePalettes_copy(void);	/* $047586 fragment */
void BaniMazaA2_InitSprite(void);	/* $0475B4 */
void BaniMazaA2_AnimSprite(void);	/* $0475E0 */
long BaniMazaA2_SetPaletteStep(void);	/* $04760A */
void unk_BaniMazaA2_04766A(void);	/* $04766A */
void BaniMazaA2_PlacePlayers(void);	/* $047712 */
void BaniMazaA2_FrameHook(void);	/* $0477B4 */
void BaniMazaA2_DrawHook(void);	/* $0477E4 */
void BaniMazaA2_CamHook(void *);	/* $047834 */
void BaniMazaA2_InitCamera(void);	/* $047858 */
void BaniMazaA2_CallEmptyStub(void);	/* $047886 */
void BaniMazaA2_LoadLevel(void);	/* $047892 */
void BaniMazaA2_RunScene(void);	/* $04791A */
void Apocalypse_ScriptA(void);	/* $04B186 fragment */
void Apocalypse_ScriptBomb(void);	/* $04B1A0 fragment */
void Apocalypse_ScriptB(void);	/* $04B1A6 fragment */
void Apocalypse_ScriptC(void);	/* $04B1C2 fragment */
long Apocalypse_GetSize(void);	/* $04B336 */
void Apocalypse_SelectBombPattern(void *);	/* $04B344 */
void Apocalypse_SelectBombPattern_part1(void);	/* $04B374 fragment */
void Apocalypse_SelectBombPattern_part2(void);	/* $04B3A6 fragment */
void Apocalypse_SelectBombPattern_part3(void);	/* $04B3B0 fragment */
void Apocalypse_NopStatic1(void);	/* $04B3C6 */
void Apocalypse_MoveTo(void *, long, long, long);	/* $04B3CE */
void Apocalypse_StartJitter(void *);	/* $04B462 */
long Apocalypse_StateEntrance(void *);	/* $04B52E */
void Apocalypse_StateWaitEntrance(void *);	/* $04B59E */
long Apocalypse_ActFlyOff(void *);	/* $04B616 */
void Apocalypse_StateFlyOffWait(void *);	/* $04B654 */
void Apocalypse_NopStatic2(void);	/* $04B6E2 */
void Apocalypse_ActStartMainScript(void *);	/* $04B6EA */
long Apocalypse_ActStartBombs(void *);	/* $04B73A */
long Apocalypse_StateDropBombs(void *);	/* $04B790 */
void Apocalypse_StateBombsDone(void *);	/* $04B848 */
long Apocalypse_ActOrbitFrom180(void *);	/* $04B996 */
long Apocalypse_ActOrbitFrom0(void *);	/* $04B9D0 */
long Apocalypse_StateOrbitStep(void *);	/* $04BA08 */
void Apocalypse_StateWaitArrive(void *);	/* $04BA9C */
long Apocalypse_ActRotateTo0(void *);	/* $04BAF8 */
void Apocalypse_StateRotateTo0(void *);	/* $04BB22 */
void Apocalypse_StateRotateTo180(void *);	/* $04BB98 */
void Apocalypse_NopStatic3(void);	/* $04BC10 */
void Apocalypse_ActSetMoveRates(void *);	/* $04BC18 */
void Apocalypse_ActPickScript(void *);	/* $04BC74 */
void Apocalypse_ActPickScript_part1(void);	/* $04BC9E fragment */
void Apocalypse_ActPickScript_part2(void);	/* $04BCB2 fragment */
void Apocalypse_ActPickScript_part3(void);	/* $04BCEC fragment */
long Apocalypse_ActNextScriptChoice(void *);	/* $04BD22 */
void Apocalypse_ActGoLeft(void *);	/* $04BD5A */
void Apocalypse_ActGoCentre(void *);	/* $04BDB4 */
void Apocalypse_ActGoRight(void *);	/* $04BE1C */
void Apocalypse_ActGoNearPlayer(void *);	/* $04BE76 */
long Apocalypse_TargetWithin(void *, short);	/* $04BEFA */
long Apocalypse_StateApproach(void *);	/* $04BF52 */
long Apocalypse_StateLunge(void *);	/* $04C030 */
void Apocalypse_StateLungeEnd(void *);	/* $04C0B2 */
long Apocalypse_StateDropStart(void *);	/* $04C114 */
long Apocalypse_StateDrop(void *);	/* $04C168 */
void Apocalypse_StateDropFall(void *);	/* $04C1D0 */
long Apocalypse_StateDefeated(void *);	/* $04C240 */
long Apocalypse_StateDefeatedWait(void *);	/* $04C2AE */
long Apocalypse_StateDefeatedRise(void *);	/* $04C306 */
void Apocalypse_StateVanish(void *);	/* $04C332 */
long Apocalypse_StateHurt(void *);	/* $04C36C */
long Apocalypse_StateKnockback(void *);	/* $04C432 */
void Apocalypse_StateResumeScript(void *);	/* $04C4C6 */
void Apocalypse_CallNopStatics(void);	/* $04C4F0 */
void Apocalypse_Nop6A(void);	/* $04C504 */
void Apocalypse_Nop66(void *);	/* $04C510 */
void Apocalypse_ClassInit(void);	/* $04C526 */
void Apocalypse_Init(void *, long, long, long);	/* $04C584 */
void Apocalypse_SetFrame(void *, short);	/* $04C76C */
void Apocalypse_Draw(void *);	/* $04C832 */
void Apocalypse_Move(void *, short);	/* $04C9D8 */
void Apocalypse_Update(void *);	/* $04CC42 */
void Apocalypse_Deactivate(void *);	/* $04CCA0 */
void Apocalypse_TakeDamage(void *, long, long, char);	/* $04CD3A */
long ApocBomb_GetSize(void);	/* $04DF1A */
void ApocBomb_NopStatic(void);	/* $04DF28 */
void ApocBomb_Nop6A(void);	/* $04DF30 */
void ApocBomb_Nop66(void *);	/* $04DF3C */
long ApocBomb_SetSpeed(short, short, short);	/* $04DF52 */
void ApocBomb_AccelTimer(void);	/* $04DF7A */
void ApocBomb_StartAccelTimer(void);	/* $04DF92 */
void ApocBomb_ClassInit(void);	/* $04DFDC */
void ApocBomb_Init(void *, long, long, long);	/* $04E000 */
void ApocBomb_Move(void *, short);	/* $04E0AC */
void ApocBomb_Update(void *);	/* $04E238 */
void ApocBomb_Draw(void *);	/* $04E3F6 */
void ApocBomb_DrawShadow(void *);	/* $04E644 */
void ApocBomb_Explode(void *);	/* $04E81A */
void ApocBomb_TakeDamage(void *);	/* $04E87C */
void ApocBomb_Attack(void *, void *, char, char, short, short);	/* $04E8F2 */
long ApocBomb_Spawn(short, short);	/* $04E94A */
void ApocBomb_Spawn_part1(void);	/* $04E9C2 fragment */
void ApocBomb_Spawn_part2(void);	/* $04E9C8 fragment */
void ApocBomb_Spawn_part3(void);	/* $04E9D0 fragment */
void ApocBomb_Spawn_part4(void);	/* $04E9EA fragment */
long BeamScooter_GetSize(void);	/* $04EABA */
void BeamScooter_Spawn(long, long);	/* $04EAC8 */
void BeamScooter_NopStatic(void);	/* $04EAF4 */
long BeamScooter_StateEnter(void *);	/* $04EAFC */
long BeamScooter_StateDescend(void *);	/* $04EB3C */
long BeamScooter_StatePickUp(void *);	/* $04EB8A */
void BeamScooter_Nop6A(void);	/* $04EC64 */
void BeamScooter_ClassInit(void);	/* $04EC70 */
void BeamScooter_Init(void *, long, long, long);	/* $04EC7C */
void BaniMazaB2_SetParallax(void);	/* $05A8E4 */
long Seq_NextWrap(void *, void *, void *);	/* $05A8F8 */
void BaniMazaB2_NopStatic(void);	/* $05A938 */
long BaniMazaB2_CrackFloorHook(short, short);	/* $05A940 */
void BaniMazaB2_InstallTileHooks(void);	/* $05AB58 */
void BaniMazaB2_FrameHook(void);	/* $05AB8C */
void BaniMazaB2_LoadLevel(void);	/* $05ABAA */
void BaniMazaB2_PlacePlayers(void);	/* $05AC24 */
void BaniMazaB2_CamHook(void *);	/* $05AC76 */
void BaniMazaB2_InitCamera(void);	/* $05AC88 */
void BaniMazaB2_InitPalette(void);	/* $05ACAE */
void BaniMazaB2_InstallFrameHook(void);	/* $05ACDC */
void BaniMazaB2_RunScene(void);	/* $05ACEC */
long Boulder_GetSize(void);	/* $05AD20 */
void Boulder_FindFloorBelow(void *);	/* $05AD2E */
long Boulder_SplitIntoPieces(void *);	/* $05AE10 */
long Boulder_SpawnerTick(long);	/* $05AED2 */
long Boulder_InitDebrisPool(void *);	/* $05B016 */
long Boulder_AllocDebris(void);	/* $05B058 */
void Boulder_FreeDebris(long);	/* $05B08E */
void Boulder_UpdateDebris(void);	/* $05B0C8 */
long Boulder_SpawnDebris(short, short, short);	/* $05B322 */
long Boulder_StateWaitGround(void *);	/* $05B58E */
long Boulder_StateStart(void *);	/* $05B5D4 */
long Boulder_StateStartRoll(void *);	/* $05B614 */
long Boulder_StateBounce(void *);	/* $05B65A */
long Boulder_StatePiecesFall(void *);	/* $05B98C */
long Boulder_StateShatter(void *);	/* $05BAAC */
void Boulder_Nop6A(void);	/* $05BB06 */
void Boulder_ClassInit(long);	/* $05BB0E */
void Boulder_Deactivate(long);	/* $05BB86 */
void Boulder_Kill(long);	/* $05BBA4 */
void Boulder_Update(void *);	/* $05BBC2 */
void Boulder_Move(void *, short);	/* $05BC9C */
void Boulder_TakeDamage(void *, long, char, char, short, short);	/* $05BD34 */
void Boulder_Draw(void *);	/* $05BD92 */
void Boulder_Init(void *, long, long, void *);	/* $05BF4A */
Boulder *Boulder_Spawn(long, long, long);	/* $05C07C */
long Tusk_GetSize(void);	/* $05C170 */
short Tusk_HookIsEvent3(short);	/* $05C17E */
short Tusk_HookReturn0(short);	/* $05C19C */
char Tusk_PlayerOnPlatform(Tusk *, XPlayer *);	/* $05C1B2 */
void Tusk_LockArena(Tusk *);	/* $05C1F4 */
void Tusk_UnlockArena(Tusk *);	/* $05C204 */
void Tusk_PlayerToLayer0(XPlayer *);	/* $05C22E */
short Tusk_StateIntro(Tusk *);	/* $05C286 */
short Tusk_StateIntroWait(Tusk *);	/* $05C302 */
short Tusk_StateBoulderRain(Tusk *);	/* $05C324 */
short Tusk_StateWaitP1Platform(Tusk *);	/* $05C3C2 */
short Tusk_StateWaitP2Platform(Tusk *);	/* $05C3EE */
short Tusk_StateStartFight(Tusk *);	/* $05C41A */
short Tusk_StateChooseAttack(Tusk *);	/* $05C4A0 */
void Tusk_StateChooseAttack_part1(void);	/* $05C538 fragment */
void Tusk_StateChooseAttack_part2(void);	/* $05C548 fragment */
void Tusk_StateChooseAttack_part3(void);	/* $05C5C4 fragment */
short Tusk_AtkVulnerable(Tusk *);	/* $05C5CE */
short Tusk_StateVulnerableWait(Tusk *);	/* $05C604 */
short Tusk_AtkThrowBoulder(Tusk *);	/* $05C626 */
short Tusk_StateThrow(Tusk *);	/* $05C6AA */
short Tusk_AtkCharge(Tusk *);	/* $05C8F4 */
short Tusk_StateChargeStart(Tusk *);	/* $05C92A */
short Tusk_StateCharging(Tusk *);	/* $05C9A2 */
short Tusk_StateChargeStop(Tusk *);	/* $05CA12 */
short Tusk_StateReturnStart(Tusk *);	/* $05CA4A */
short Tusk_StateReturnHome(Tusk *);	/* $05CAA0 */
short Tusk_StateAtHome(Tusk *);	/* $05CB04 */
short Tusk_AtkSpawnTuskette2(Tusk *);	/* $05CB32 */
short Tusk_AtkSpawnTuskette3(Tusk *);	/* $05CB72 */
short Tusk_AtkSpawnTusketteBoth(Tusk *);	/* $05CBB2 */
short Tusk_HurtFlash(Tusk *);	/* $05CBF2 */
short Tusk_StateHurtFlash(Tusk *);	/* $05CC3A */
short Tusk_HurtCharge(Tusk *);	/* $05CC92 */
void Tusk_Defeated(Tusk *);	/* $05CCDA */
void Tusk_Defeated_part1(void);	/* $05CD0A fragment */
void Tusk_Defeated_part2(void);	/* $05CD18 fragment */
void Tusk_Defeated_part3(void);	/* $05CD2C fragment */
void Tusk_Defeated_part4(void);	/* $05CD36 fragment */
void Tusk_Defeated_part5(void);	/* $05CD3E fragment */
void Tusk_Nop6A(Tusk *);	/* $05CD46 */
void Tusk_InitSpawnRec(void *, Placement *);	/* $05CD4E */
void Tusk_ClassInit(void);	/* $05CD7E */
void Tusk_TakeDamage(Tusk *self, Actor *attacker, char type, char damage, short hx, short hy);	/* $05CD86 */
void Tusk_Nop66(Tusk *);	/* $05CE28 */
void Tusk_Update(Tusk *);	/* $05CE3A */
void Tusk_Init(Tusk *, long, long, Placement *);	/* $05CF68 */
long Tuskette_GetSize(void);	/* $05D0AE */
Tuskette *Tuskette_Spawn(long, long, short);	/* $05D0BC */
short Tuskette_StatePatrolStart(Tuskette *);	/* $05D11C */
short Tuskette_StateWaitNear(Tuskette *);	/* $05D150 */
short Tuskette_StateWalkStart(Tuskette *);	/* $05D1DA */
short Tuskette_StateWalk(Tuskette *);	/* $05D26E */
short Tuskette_StateHopInit(Tuskette *);	/* $05D2F6 */
short Tuskette_StateHopIdle(Tuskette *);	/* $05D31A */
short Tuskette_StateHopWait(Tuskette *);	/* $05D34E */
short Tuskette_StateHopOut(Tuskette *);	/* $05D3C8 */
short Tuskette_StateHopRise(Tuskette *);	/* $05D430 */
short Tuskette_StateHopLand(Tuskette *);	/* $05D45A */
short Tuskette_StateHopPause(Tuskette *);	/* $05D49C */
short Tuskette_StateHopBack(Tuskette *);	/* $05D4D2 */
short Tuskette_StateHopBackRise(Tuskette *);	/* $05D556 */
short Tuskette_StateHopBackLand(Tuskette *);	/* $05D580 */
short Tuskette_StateHopDone(Tuskette *);	/* $05D5C2 */
short Tuskette_StateWaitSameFloor(Tuskette *);	/* $05D5FC */
short Tuskette_StateChaseStart(Tuskette *);	/* $05D63A */
short Tuskette_StateChaseTarget(Tuskette *);	/* $05D67C */
void Tuskette_StateHitTarget(Tuskette *);	/* $05D704 */
short Tuskette_StateTargetGone(Tuskette *);	/* $05D714 */
short Tuskette_StateWanderOut(Tuskette *);	/* $05D738 */
short Tuskette_StateStop(Tuskette *);	/* $05D786 */
short Tuskette_StateRestIdle(Tuskette *);	/* $05D7C2 */
short Tuskette_StateWaitCharge(Tuskette *);	/* $05D7F6 */
short Tuskette_StateRunRight(Tuskette *);	/* $05D898 */
short Tuskette_StateRunRightUntil(Tuskette *);	/* $05D8E8 */
short Tuskette_StateRunRightEnd(Tuskette *);	/* $05D91A */
short Tuskette_StateRunLeft(Tuskette *);	/* $05D956 */
short Tuskette_StateRunLeftUntil(Tuskette *);	/* $05D9AA */
short Tuskette_StateRunLeftEnd(Tuskette *);	/* $05D9DC */
short Tuskette_StateExitLeft(Tuskette *);	/* $05DA18 */
short Tuskette_StateExitRight(Tuskette *);	/* $05DA58 */
short Tuskette_StateExitWalk(Tuskette *);	/* $05DA98 */
short Tuskette_StateExitDrop(Tuskette *);	/* $05DAEE */
short Tuskette_StateExitFall(Tuskette *);	/* $05DB4C */
short Tuskette_StateExitTurn(Tuskette *);	/* $05DBA2 */
short Tuskette_StateExitBack(Tuskette *);	/* $05DBE6 */
short Tuskette_StateExitPast(Tuskette *);	/* $05DC4A */
short Tuskette_StateExitDrop2(Tuskette *);	/* $05DCA0 */
short Tuskette_StateExitFall2(Tuskette *);	/* $05DCD0 */
short Tuskette_StateRemove(Tuskette *);	/* $05DD0E */
void Tuskette_Nop6A(Tuskette *);	/* $05DD40 */
void Tuskette_ClassInit(void);	/* $05DD48 */
void Tuskette_Nop66(Tuskette *);	/* $05DD66 */
void Tuskette_Move(Tuskette *, short);	/* $05DD7C */
void Tuskette_Init(Tuskette *, long, long, Placement *);	/* $05DDBE */
void Tuskette_Init_part1(void);	/* $05DF06 fragment */
void Tuskette_Init_part2(void);	/* $05DF16 fragment */
void Tuskette_Init_part3(void);	/* $05DF26 fragment */
void Tuskette_Init_part4(void);	/* $05DF6E fragment */
void Tuskette_Init_part5(void);	/* $05DF84 fragment */
void Tuskette_Init_part6(void);	/* $05E038 fragment */
void Cerebro_BigSpriteDecode(void *);	/* $0644D4 */
void Cerebro_BigSpriteDraw(void *);	/* $06451E */
void Cerebro_BigSpriteInit(void *, void *);	/* $064588 */
long PlayerSelect_AllocCharBuffers(void);	/* $064638 */
void PlayerSelect_AllocCharBuffers_Cases(void);	/* $064674 fragment */
void PlayerSelect_AllocCharBuffers_Measure(void);	/* $0646D6 fragment */
void PlayerSelect_LoadCharFrame(void *);	/* $0647D0 */
void PlayerSelect_LoadCharFrame_Beast(void);	/* $064806 fragment */
void PlayerSelect_LoadCharFrame_Cyclops(void);	/* $064814 fragment */
void PlayerSelect_LoadCharFrame_Nightcrawler(void);	/* $064830 fragment */
void PlayerSelect_LoadCharFrame_Psylocke(void);	/* $06483E fragment */
void PlayerSelect_LoadCharFrame_Wolverine(void);	/* $06484C fragment */
void PlayerSelect_LoadCharFrame_Magneto(void);	/* $06485A fragment */
void PlayerSelect_LoadCharFrame_part1(void);	/* $064868 fragment */
long PlayerSelect_CharSpriteDef(void *);	/* $06489A */
void PlayerSelect_CharSpriteDef_Beast(void);	/* $0648CC fragment */
void PlayerSelect_CharSpriteDef_Cyclops(void);	/* $0648D4 fragment */
void PlayerSelect_CharSpriteDef_Nightcrawler(void);	/* $0648E4 fragment */
void PlayerSelect_CharSpriteDef_Psylocke(void);	/* $0648EC fragment */
void PlayerSelect_CharSpriteDef_Wolverine(void);	/* $0648F4 fragment */
void PlayerSelect_CharSpriteDef_Magneto(void);	/* $0648FC fragment */
void PlayerSelect_CharSpriteDef_None(void);	/* $064904 fragment */
short PlayerSelect_CharFrameNum(void *);	/* $064912 */
void PlayerSelect_CharFrameNum_Beast(void);	/* $064944 fragment */
void PlayerSelect_CharFrameNum_Cyclops(void);	/* $06494C fragment */
void PlayerSelect_CharFrameNum_Nightcrawler(void);	/* $06495C fragment */
void PlayerSelect_CharFrameNum_Psylocke(void);	/* $064964 fragment */
void PlayerSelect_CharFrameNum_Wolverine(void);	/* $06496C fragment */
void PlayerSelect_CharFrameNum_Magneto(void);	/* $064974 fragment */
void PlayerSelect_CharFrameNum_None(void);	/* $06497C fragment */
void PlayerSelect_DrawFrame(short, short, short, short, char);	/* $06498A */
void PlayerSelect_DrawFrames(void);	/* $064AA2 */
void PlayerSelect_PlaceCursor(void *);	/* $064AEC */
void PlayerSelect_RevertCursor(void *);	/* $064B54 */
short PlayerSelect_PrevFreeChar(short);	/* $064B76 */
short PlayerSelect_NextFreeChar(short);	/* $064BF0 */
short PlayerSelect_FirstFreeChar(void);	/* $064C66 */
void Cerebro_AnimatePalettes(void);	/* $064CA2 */
void Cerebro_AnimatePalettes_CasesA(void);	/* $064CF2 fragment */
void Cerebro_AnimatePalettes_StepB(void);	/* $064DD2 fragment */
void Cerebro_AnimatePalettes_CasesB(void);	/* $064E14 fragment */
void Cerebro_AnimatePalettes_EndB(void);	/* $064E82 fragment */
void Cerebro_FrameHook(void);	/* $064E9C */
void Cerebro_SetupPalCycles(void);	/* $064EDE */
void Cerebro_PrintText(void *, short);	/* $064F70 */
void Cerebro_PrintText_part1(void);	/* $065326 fragment */
void Cerebro_PrintText_part2(void);	/* $065374 fragment */
void PlayerSelect_CheckDone(void);	/* $06539C */
void Cerebro_ClearTextWindow(void);	/* $065400 */
void Cerebro_ClearScreen(void);	/* $065494 */
void Cerebro_LoadSpriteTiles(void);	/* $0654F4 */
void Cerebro_RunTextScript(void);	/* $065532 */
void Cerebro_TextOpSetCursor(void);	/* $0655C4 fragment */
void Cerebro_TextOpFadeOut(void);	/* $06563A fragment */
void Cerebro_TextOpPlaySound(void);	/* $065662 fragment */
void Cerebro_TextOpPrint(void);	/* $065682 fragment */
void PlayerSelect_Run(void);	/* $0656B0 */
void PlayerSelect_Run_P1Beast(void);	/* $065C68 fragment */
void PlayerSelect_Run_P1Cyclops(void);	/* $065C74 fragment */
void PlayerSelect_Run_P1Nightcrawler(void);	/* $065C8C fragment */
void PlayerSelect_Run_P1Psylocke(void);	/* $065C98 fragment */
void PlayerSelect_Run_P1Wolverine(void);	/* $065CA4 fragment */
void PlayerSelect_Run_P1Magneto(void);	/* $065CB0 fragment */
void PlayerSelect_Run_SetP2(void);	/* $065CBC fragment */
void PlayerSelect_Run_P2Beast(void);	/* $065D20 fragment */
void PlayerSelect_Run_P2Cyclops(void);	/* $065D2C fragment */
void PlayerSelect_Run_P2Gambit(void);	/* $065D38 fragment */
void PlayerSelect_Run_P2None(void);	/* $065D74 fragment */
void Cerebro_IntroPalHook(void);	/* $065DB6 */
void Cerebro_PlayIntro(void);	/* $065E6C */
void Cerebro_Run(void);	/* $066B26 */
void CerebroSentinels_RunScene(void);	/* $066E80 */
void CerebroAvalon_RunScene(void);	/* $066EA8 */
void CerebroBaniMaza_RunScene(void);	/* $066EC8 */
void CerebroSavage_RunScene(void);	/* $066EE8 */
void CerebroSpace_RunScene(void);	/* $066F08 */
void CerebroClone_RunScene(void);	/* $066F28 */
void PlayerSelect_RunScene(void);	/* $066F44 */
void LogoAndDemo_RunScene(void);	/* $066F7E */
void FinalText_RunScene(void);	/* $066FBA */
void Credits_RunScene(void);	/* $066FDE */
void Failure_RunScene(void);	/* $067020 */
long ScalingActor_GetSize(void);	/* $06717E */
void ScalingActor_Init(void *, long, long, long);	/* $06718C */
void ScalingActor_Activate(void *);	/* $067202 */
long ScalingActor_OutsideActiveWindow(void);	/* $067244 */
void ScalingActor_Move(void *);	/* $06724E */
void ScalingActor_Draw(void *);	/* $06726C */
void ScalingActor_SetFrame(void *, short);	/* $0672CE */
void ScalingActor_SetFrame_part1(void);	/* $06730C fragment */
void ScalingActor_SetFrame_part2(void);	/* $067360 fragment */
void ScalingActor_SetFrame_part3(void);	/* $06736C fragment */
void PlayerSelect_CalcPortraitPos(void *);	/* $067376 */
long PlayerSelect_PlayPickAnim(void *);	/* $067420 */
void PlayerSelect_PlayPickAnim_part1(void);	/* $06745C fragment */
void PlayerSelect_PlayPickAnim_part2(void);	/* $067478 fragment */
void PlayerSelect_PlayPickAnim_part3(void);	/* $06750E fragment */
void PlayerSelect_PlayIdleAnim(void *);	/* $067542 */
void PlayerSelect_PlayIdleAnim_part1(void);	/* $06757C fragment */
void PlayerSelect_PlayIdleAnim_part2(void);	/* $06758E fragment */
void PlayerSelect_PlayIdleAnim_part3(void);	/* $0675B2 fragment */
void PlayerSelect_PlayIdleAnim_part4(void);	/* $0675C4 fragment */
void PlayerSelect_PlayIdleAnim_part5(void);	/* $0675D6 fragment */
void PlayerSelect_PlayIdleAnim_part6(void);	/* $0675E8 fragment */
void PlayerSelect_PlayIdleAnim_part7(void);	/* $0675F8 fragment */
long PortraitActor_StateInit(void *);	/* $067600 */
long PortraitActor_StateIdle(void *);	/* $0676F0 */
long PortraitActor_StartFlyOut(void *);	/* $06777A */
long PortraitActor_StateFlyOut(void *);	/* $06789C */
long PortraitActor_StateSwapChar(void *);	/* $067936 */
long PortraitActor_StateWaitTurn(void *);	/* $067984 */
long PortraitActor_StateFirstChar(void *);	/* $067A56 */
long PortraitActor_StartFlyIn(void *);	/* $067AE0 */
long PortraitActor_StateFlyIn(void *);	/* $067C32 */
long PortraitActor_StateLanded(void *);	/* $067CC4 */
long BoxInputActor_GetSize(void);	/* $067D6C */
void BoxInputActor_Init(void *, long, long, long);	/* $067D7A */
long BoxInputActor_OutsideActiveWindow(void);	/* $067DF0 */
void BoxInputActor_Move(void *);	/* $067DFA */
void BoxInputActor_Draw(void *);	/* $067E18 */
long CursorInput_StatePoll(void *);	/* $067E5E */
long CursorInput_MoveLeft(void *);	/* $067FCA */
long CursorInput_MoveRight(void *);	/* $068078 */
long CursorInput_Confirm(void *);	/* $068126 */
void Brood_Script(void);	/* $06D126 fragment */
long Brood_GetSize(void);	/* $06D1C2 */
void Brood_Nop66(Brood *);	/* $06D1D0 */
void Brood_Nop6A(Brood *);	/* $06D1E6 */
void Brood_ClassInit(void);	/* $06D1EE */
void Brood_Init(Brood *, long, long, Placement *);	/* $06D1F6 */
void Brood_TakeDamage(Brood *, Actor *, char, char, short, short);	/* $06D472 */
short Brood_StateStart(Brood *);	/* $06D4F6 */
short Brood_StateLand(Brood *);	/* $06D51A */
void Brood_ActIdle(Brood *);	/* $06D69E */
short Brood_ActApproach(Brood *);	/* $06D70E */
short Brood_StateApproach(Brood *);	/* $06D848 */
short Brood_ActShoot(Brood *);	/* $06D92C */
short Brood_StateFire(Brood *);	/* $06D990 */
void Brood_StateFireEnd(Brood *);	/* $06D9DE */
short Brood_ActCharge(Brood *);	/* $06DA42 */
short Brood_StateWalkToPlayer(Brood *);	/* $06DB86 */
short Brood_StateLimitWait(Brood *);	/* $06DCB2 */
short Brood_StateRetreatStart(Brood *);	/* $06DCDC */
void Brood_StateRetreat(Brood *);	/* $06DD32 */
short Brood_StateCornered(Brood *);	/* $06DDA0 */
short Brood_StateCorneredFlash(Brood *);	/* $06DDE0 */
short Brood_StateBreakOut(Brood *);	/* $06DE3A */
void Brood_StateBreakOutRun(Brood *);	/* $06DE90 */
void CloneA1_AnimTilesTimer(long, long, void *);	/* $077256 asm */
void CloneA1_ResetPalCycle(void);	/* $077402 */
void CloneA1_CyclePalettes(void);	/* $077422 */
void CloneA1_NopStatic(void);	/* $077462 */
void CloneA1_InitSprites(void);	/* $07746A */
void CloneA1_FrameHook(void);	/* $07754A */
void CloneA1_LoadResources(void);	/* $07757C */
void CloneA1_LoadLevel(void);	/* $0775AC */
void CloneA1_InstallHooks(void);	/* $077624 */
void CloneA1_EmptyStub(void);	/* $0776DA */
void CloneA1_RunScene(void);	/* $0776E2 */
long LightBeam_GetSize(void);	/* $0777B4 */
void LightBeam_NopStatic(void);	/* $0777C2 */
void LightBeam_Nop66(void *);	/* $0777CA */
void LightBeam_ClassInit(void);	/* $0777E0 */
void LightBeam_Activate(void *);	/* $077814 */
void LightBeam_Init(void *, long, long, void *);	/* $077848 */
void LightBeam_Draw(void *);	/* $077988 */
long LightBeam_StateIdle(void *);	/* $077CBC */
void CloneA2_EmptyStub1(void);	/* $081BBA */
void CloneA2_FlickerInit(void);	/* $081BC2 */
void CloneA2_FlickerUpdate(void);	/* $081BE6 */
void CloneA2_FlickerUpdate_Color15Case0(void);	/* $081C7E fragment */
void CloneA2_FlickerUpdate_SetColor15(void);	/* $081CAC fragment */
void CloneA2_StartQuake(void);	/* $081CE0 */
void CloneA2_FrameHook(void);	/* $081D08 */
void CloneA2_LoadResources(void);	/* $081DD2 */
void CloneA2_LoadLevel(void);	/* $081E02 */
void CloneA2_InstallHooks(void);	/* $081EA6 */
void CloneA2_EmptyStub2(void);	/* $081ED4 */
void CloneA2_CamHookNormal(void *);	/* $081EDC */
void CloneA2_CamHookAlt(void *);	/* $081F02 */
void CloneA2_InitCamera(void);	/* $081F28 */
void CloneA2_EmptyStub3(void);	/* $081F5A */
void CloneA2_BgPalTimer(void);	/* $081F62 */
void CloneA2_BgPalTimer_Case0(void);	/* $081FB4 fragment */
void CloneA2_BgPalTimer_Copy(void);	/* $081FDA fragment */
void CloneA2_StartBgPalTimer(void);	/* $081FFE */
void CloneA2_CamAltTimer(long);	/* $082038 */
void CloneA2_StartCamAltMode(void);	/* $08206A */
void CloneA2_SetCameraZone(short);	/* $0820AE */
void CloneA2_SetCameraLimitsToMap(void);	/* $082176 */
void CloneA2_CallEmptyStubs(void);	/* $0821D4 */
void CloneA2_StartCamera(void);	/* $0821E8 */
void CloneA2_RunScene(void);	/* $082244 */
long DropBomb_GetSize(void);	/* $08230C */
void DropBomb_Nop66(void *);	/* $08231A */
void DropBomb_Nop6A(void);	/* $082330 */
long DropBomb_ClassInit(void);	/* $082338 */
void DropBomb_Init(void *, long, long, void *);	/* $082340 */
long DropBomb_StateStart(void *);	/* $08243E */
long DropBomb_StateWaitInView(void *);	/* $082472 */
long DropBomb_StateDrop(void *);	/* $082508 */
long DropBomb_StateFall(void *);	/* $082542 */
void DropBomb_StateExplode(void *);	/* $0825CC */
long DropBomb_Spawn(long, long, long);	/* $08270E */
void SmallBrood_ScrStepKamikaze(void);	/* $0840CE fragment */
void SmallBrood_ScrStepApproach(void);	/* $0840D8 fragment */
void SmallBrood_ScrStepWalk(void);	/* $0840E6 fragment */
long SmallBrood_GetSize(void);	/* $084190 */
void SmallBrood_Nop66(SmallBrood *);	/* $08419E */
void SmallBrood_Nop6A(SmallBrood *);	/* $0841B4 */
void SmallBrood_ClassInit(void);	/* $0841BC */
void SmallBrood_Init(SmallBrood *, long, long, Placement *);	/* $0841C4 */
void SmallBrood_Init_CaseDefault(void);	/* $0843B8 fragment */
void SmallBrood_Init_Case1(void);	/* $0843CC fragment */
void SmallBrood_Init_SetYLimits(void);	/* $084420 fragment */
void SmallBrood_Update(SmallBrood *);	/* $084500 */
void SmallBrood_Move(SmallBrood *, short);	/* $084544 */
void SmallBrood_Shoot(SmallBrood *, short, short, char);	/* $0845EE */
short SmallBrood_StateStart(SmallBrood *);	/* $08468E */
short SmallBrood_StateLand(SmallBrood *);	/* $0846B2 */
void SmallBrood_ActIdle(SmallBrood *);	/* $084854 */
short SmallBrood_ActApproach(SmallBrood *);	/* $0848CA */
void SmallBrood_StateWaitCooldown(SmallBrood *);	/* $084988 */
short SmallBrood_ActWalk(SmallBrood *);	/* $0849F4 */
short SmallBrood_StateApproach(SmallBrood *);	/* $084A44 */
short SmallBrood_ActChase(SmallBrood *);	/* $084B28 */
short SmallBrood_StateChase(SmallBrood *);	/* $084B94 */
short SmallBrood_ActShoot(SmallBrood *);	/* $084C62 */
short SmallBrood_StateFire(SmallBrood *);	/* $084CC6 */
void SmallBrood_StateNextStep(SmallBrood *);	/* $084D14 */
short SmallBrood_ActJump(SmallBrood *);	/* $084D64 */
short SmallBrood_StateJumpFlip(SmallBrood *);	/* $084DCC */
short SmallBrood_StateJumpUntilSurface(SmallBrood *);	/* $084E2A */
void SmallBrood_StateLanded(SmallBrood *);	/* $084E8A */
void SmallBrood_ActWaitCooldown(SmallBrood *);	/* $084EEA */
void SmallBrood_StateWaitForPlayer(SmallBrood *);	/* $084F5A */
short SmallBrood_ActKamikaze(SmallBrood *);	/* $08503E */
short SmallBrood_StateArmed(SmallBrood *);	/* $085130 */
short SmallBrood_StateFlash(SmallBrood *);	/* $08515A */
void SmallBrood_StateExplode(SmallBrood *);	/* $0851BE */
void Teleporter_NoPause(void);	/* $0853FA */
long TeleportReceiver_GetSize(void);	/* $085402 */
void TeleportReceiver_InitSpawnRec(void *, long);	/* $085410 */
long Teleporter_GetSize(void);	/* $085444 */
void Teleporter_Nop66(void *);	/* $085452 */
void Teleporter_Nop6A(void);	/* $085468 */
long Teleporter_ClassInit(void);	/* $085470 */
void Teleporter_Init(void *, long, long, void *);	/* $085478 */
long Teleporter_StateStart(void *);	/* $08555E */
long Teleporter_StateWait(void *);	/* $085582 */
long Teleporter_StateTeleport(void *);	/* $085696 */
long Teleporter_StateHold(void *);	/* $0857DC */
long Teleporter_StateArrive(void *);	/* $085850 */
void Teleporter_StateWaitFade(void *);	/* $0858F6 */
void unk_CloneA5_085932(void);	/* $085932 */
long BroodBossParts_GetSize(void);	/* $08593E */
long BroodBossParts_Init(void *, long, long, void *);	/* $08594C */
long BroodBossParts_Draw(void *);	/* $0859B0 */
long BroodBossParts_Update(void *);	/* $085AA6 */
long LittleLeg_GetSize(void);	/* $085BDE */
void LittleLeg_SetWalking(void *, char);	/* $085BE8 */
long LittleLeg_New(long, short, short);	/* $085C5E */
void LittleLeg_Update(void *);	/* $085D4C */
void LittleLeg_Draw(void *);	/* $085F0A */
void LittleLeg_Destroy(void *);	/* $085F82 */
long BroodBossWall_GetSize(void);	/* $085FCE */
void BroodBossWall_Init(void *, long, long);	/* $085FD8 */
void BroodBossWall_Update(void *);	/* $086032 */
void BroodBossWall_Draw(void *);	/* $086136 */
long BroodBossArm_GetSize(void);	/* $0861F4 */
long BroodBossArm_Init(void *, long, long, long);	/* $086202 */
long BroodBossArm_Draw(void *);	/* $08627E */
long BroodBossDamage_GetSize(void);	/* $0862CA */
void BroodBossDamage_Init(void *, long, long, void *);	/* $0862D8 */
long BroodBossDamage_Draw(void);	/* $0864E2 */
long BroodBossDamage_Update(void *);	/* $0864EA */
long BroodBossDamage_StateIdle(void *);	/* $08669C */
long BroodBoss_GetSize(void);	/* $0866C0 */
void BroodBoss_Init(void *, long, long, void *);	/* $0866CE */
long BroodBoss_SpawnRecInView(long, void *);	/* $0868C6 */
void BroodBoss_Update(void *);	/* $0868F0 */
long BroodBoss_StateSetup(void *);	/* $086ABC */
long BroodBoss_StateRoar(void *);	/* $086C02 */
long BroodBoss_StateEnter(void *);	/* $086C3E */
long BroodBoss_StateWaitArrive(void *);	/* $086CC4 */
void BroodBoss_ActWait60(void *);	/* $086D5E */
void BroodBoss_ActWait30(void *);	/* $086DB2 */
void BroodBoss_ActClaw(void *);	/* $086E06 */
long BroodBoss_ActDropBomb(void *);	/* $086E86 */
long BroodBoss_StateDropBomb(void *);	/* $086ECE */
long BroodBoss_ActDropBomb2(void *);	/* $086F28 */
long BroodBoss_StateDropBomb2(void *);	/* $086F70 */
void BroodBoss_StateAfterBomb(void *);	/* $086FCA */
long BroodBoss_ActStomp(void *);	/* $08702A */
long BroodBoss_StateStompLand(void *);	/* $087072 */
long BroodBoss_StateShockOn(void *);	/* $0870AC */
void BroodBoss_StateShockOff(void *);	/* $0870EA */
void BroodBoss_ActStop(void *);	/* $08718A */
long BroodBoss_ActBringWall(void *);	/* $087208 */
long BroodBoss_StateWallStop(void *);	/* $087294 */
long BroodBoss_StateChargeStart(void *);	/* $087304 */
long BroodBoss_StateCharge(void *);	/* $087370 */
long BroodBoss_StateRetreat(void *);	/* $087426 */
void BroodBoss_SpawnDebris(void *);	/* $0874DC */
long BroodBoss_ActChargeWall(void *);	/* $087516 */
long BroodBoss_StateChargeWall(void *);	/* $087582 */
long BroodBoss_StateHitWall(void *);	/* $087600 */
long BroodBoss_StateRecoil(void *);	/* $08765E */
void BroodBoss_StateRecoilReturn(void *);	/* $0876BA */
long BroodBoss_ActAdvance(void *);	/* $087748 */
long BroodBoss_StateAdvance(void *);	/* $0877D2 */
void BroodBoss_StateMakeUnhittable(void *);	/* $087832 */
long BroodBoss_ActScream(void *);	/* $08789C */
long BroodBoss_StateScream(void *);	/* $0878E4 */
void BroodBoss_Defeat(void *);	/* $08797A */
long BroodBoss_StateHurt(void *);	/* $0879FE */
long BroodBoss_StateHurtEnd(void *);	/* $087B22 */
long BroodBoss_StateDeathStart(void *);	/* $087BAC */
long BroodBoss_StateDeathSlide(void *);	/* $087BFE */
long BroodBoss_StateDeathAnim(void *);	/* $087CB4 */
long BroodBoss_StateDeathFx(void *);	/* $087CEA */
long BroodBoss_StateDeathSink(void *);	/* $087DB0 */
long BroodBoss_StateExplode(void *);	/* $087E0C */
long BroodBoss_StateVanish(void *);	/* $087EBE */
long BroodBoss_StateClawOut(void *);	/* $087EFA */
long BroodBoss_StateRemoveParts(void *);	/* $087F3C */
void BroodBoss_StateFinish(void *);	/* $087FEA */
long CloneA5_SegHook16(void);	/* $08803A */
long CloneA5_SegHook15(short);	/* $088044 */
long CloneA5_SegHook17(short, void *, void *);	/* $088064 */
void CloneA5_CamApply(void *);	/* $088112 */
void CloneA5_CamUpdate(void *);	/* $08827A */
long CloneA5_InitTileHooks(void);	/* $0883AE */
void CloneA5_InitShockSprites(void);	/* $0883C2 */
long CloneA5_AnimShockSprites(void);	/* $088422 */
void CloneA5_ResetPalCycle(void);	/* $0884D2 */
void CloneA5_FrameHook(void);	/* $0884E6 */
void CloneA5_LoadResources(void);	/* $088588 */
void CloneA5_SpawnBoss(void);	/* $088600 */
void CloneA5_CamSetHooks(void *);	/* $08867C */
void CloneA5_InitCamera(void);	/* $08869E */
void CloneA5_RunScene(void);	/* $088730 */
void BroodBoss_ScrStepClaw(void);	/* $088A56 fragment */
void BroodBoss_ScrStepScream(void);	/* $088A9A fragment */
long EvilMaker_GetSize(void);	/* $0945C6 */
void EvilMaker_Nop6A(void);	/* $0945D4 */
void EvilMaker_Init(void *, long, long, long);	/* $0945E0 */
long EvilMaker_StateWaitKills(void *);	/* $0946BA */
long EvilMaker_StateIdle(void *);	/* $09472E */
void EvilMaker_ActWait60(void *);	/* $094752 */
void EvilMaker_ActDropBeast(void *);	/* $0947A6 */
void EvilMaker_ActKill(void *);	/* $094834 */
void EvilMaker_ActDropCyclops(void *);	/* $094850 */
void EvilMaker_ActDropGambit(void *);	/* $0948DE */
void EvilMaker_ActDropMagneto(void *);	/* $09496C */
void EvilMaker_ActDropNightcrawler(void *);	/* $0949FA */
void EvilMaker_ActDropPsylocke(void *);	/* $094A88 */
void EvilMaker_ActDropWolverine(void *);	/* $094B16 */
void EvilMaker_NopStatic(void);	/* $094BA4 */
void EvilMaker_ScrStepKill(void);	/* $094C26 fragment */
long EvilDrop_GetSize(void);	/* $094C82 */
long EvilDrop_Spawn(void *);	/* $094C90 */
void EvilDrop_Nop6A(void);	/* $094CDA */
long EvilDrop_ClassInit(void);	/* $094CE6 */
void EvilDrop_Init(void *, long, long, void *);	/* $094CF2 */
void EvilDrop_Move(void *, short);	/* $094E7A */
long EvilDrop_StateStart(void *);	/* $094EFC */
long EvilDrop_StateDrop(void *);	/* $094F20 */
long EvilDrop_StateFall(void *);	/* $094F50 */
long EvilDrop_StateBounce1(void *);	/* $094FB2 */
long EvilDrop_StateBounce2(void *);	/* $095014 */
void EvilDrop_ActOpen(void *);	/* $095094 */
void EvilDrop_ActRelease(void *);	/* $0950D6 */
void EvilDrop_NopStatic(void);	/* $09511E */
long XClone_GetSize(void);	/* $095208 */
void XClone_Nop66(XClone *self);	/* $095216 */
void XClone_Nop6A(void);	/* $09522C */
void XClone_ClassInit(void);	/* $095238 */
void XClone_Init(XClone *self, long x, long y, Placement *place);	/* $095250 */
void XClone_Deactivate(XClone *self);	/* $0954C8 */
void XClone_Kill(XClone *self);	/* $09550C */
void XClone_Nop82(void);	/* $095586 */
void XClone_Update(XClone *self);	/* $09558E */
void XClone_Move(XClone *self, short steps);	/* $095618 */
void XClone_Explode(XClone *self);	/* $0956D2 */
void XClone_TakeDamage(XClone *self, Actor *attacker, char type, char damage, short x, short y);	/* $095758 */
short XClone_StateLand(XClone *self);	/* $095876 */
void XClone_OnHurt(XClone *self);	/* $0958F6 */
short XClone_StateWaitChild(XClone *self);	/* $095968 */
void XClone_ActUnhittableScript(XClone *self);	/* $0959E0 */
void XClone_ActSoundEF(XClone *self);	/* $095A5A */
void XClone_ActSoundFA(XClone *self);	/* $095ABC */
short XClone_ActOpenBioBalls(XClone *self);	/* $095B1E */
void XClone_ActCloseBioBalls(XClone *self);	/* $095C22 */
short XClone_StateWaitPlayers(XClone *self);	/* $095CB2 */
short XClone_StateDropPod(XClone *self);	/* $095DDC */
short XClone_StatePodFall(XClone *self);	/* $095ECA */
short XClone_StatePodBounce1(XClone *self);	/* $095FA6 */
short XClone_StatePodBounce2(XClone *self);	/* $096082 */
short XClone_StatePodOpen(XClone *self);	/* $09615C */
short XClone_StatePodWaitOpen(XClone *self);	/* $0961F0 */
short XClone_StateEmerge(XClone *self);	/* $096270 */
void XClone_DieRestartMusic(XClone *self);	/* $096318 */
void XClone_ActSound8F(XClone *self);	/* $096330 */
void XClone_ActCheckLowHealth(XClone *self);	/* $096396 */
void XClone_ActStop(XClone *self);	/* $096434 */
void XClone_ActStandAnim(XClone *self);	/* $096490 */
void XClone_ActStand60(XClone *self);	/* $0964F4 */
void XClone_ActStand30(XClone *self);	/* $096568 */
void XClone_ActStand10(XClone *self);	/* $0965DC */
void XClone_ActStand60B(XClone *self);	/* $096650 */
void XClone_ActWait60(XClone *self);	/* $0966C4 */
void XClone_ActWait30(XClone *self);	/* $096718 */
void XClone_ActWait10(XClone *self);	/* $09676C */
void XClone_ActLeaveBalls(XClone *self);	/* $0967C0 */
short XClone_ActBecomeBalls(XClone *self);	/* $09687C */
short XClone_ActBallsToFloor(XClone *self);	/* $096998 */
void XClone_ActStopSoundFE(XClone *self);	/* $096A36 */
void XClone_ActSoundFE(XClone *self);	/* $096AA2 */
short XClone_ActWaitChildFlag(XClone *self);	/* $096B16 */
short XClone_ActChildAimPlayer(XClone *self);	/* $096B90 */
short XClone_StateChildAim(XClone *self);	/* $096C30 */
short XClone_StateChildHome(XClone *self);	/* $096C9A */
void XClone_StateChildHome_Step(void);	/* $096D36 fragment */
short XClone_ActChildShoot3(XClone *self);	/* $096D82 */
short XClone_StateChildShoot(XClone *self);	/* $096DF2 */
short XClone_ActChildLeap(XClone *self);	/* $096F3E */
void XClone_Impact(short x, short y);	/* $097016 */
short XClone_StateChildLeap(XClone *self);	/* $09708A */
void XClone_StateChildReturn(XClone *self);	/* $09715A */
void XClone_ActBallsMotionA(XClone *self);	/* $09722C */
void XClone_ActBallsMotionB(XClone *self);	/* $0972B2 */
void XClone_ActBallsMotionC(XClone *self);	/* $097338 */
short XClone_ActBallsMotionD(XClone *self);	/* $0973B4 */
short XClone_StateWaitChild2(XClone *self);	/* $097426 */
short XClone_StateBallsReturn(XClone *self);	/* $09749E */
short XClone_ActBallsRoll(XClone *self);	/* $097514 */
short XClone_ActBallsCollapse(XClone *self);	/* $0975B8 */
void XClone_ActTurn(XClone *self);	/* $097642 */
void XClone_ActFacePlayer(XClone *self);	/* $097698 */
void XClone_ActStopAnim46(XClone *self);	/* $097712 */
void XClone_ActAnim3C(XClone *self);	/* $097786 */
void XClone_ActAttack(XClone *self);	/* $097834 */
char XClone_WalkStep(XClone *self);	/* $097904 */
short XClone_ActApproachPlayer(XClone *self);	/* $097974 */
short XClone_ActKeepDistance(XClone *self);	/* $097AA2 */
short XClone_ActWaitStopped(XClone *self);	/* $097C7A */
void XClone_ActWalkStep(XClone *self);	/* $097D08 */
void XClone_ActMainScript(XClone *self);	/* $097D60 */
long EvilBeast_GetSize(void);	/* $097DB4 */
void EvilBeast_Init(void *, long, long, long);	/* $097DC2 */
void EvilBeast_UpdateThrows(void *);	/* $097E32 */
long EvilBeast_ActThrow(void *);	/* $097EE0 */
void EvilBeast_StateThrow(void *);	/* $097F60 */
long EvilBeam_GetSize(void);	/* $097FD0 */
long EvilBeam_Construct(void *, short, short, long, char);	/* $097FDA */
void EvilBeam_Update(void *);	/* $09806C */
void EvilBeam_Draw(void *);	/* $0981FC */
void EvilBeam_Destroy(void *);	/* $098236 */
long EvilCyclops_GetSize(void);	/* $098282 */
void EvilCyclops_Init(void *, long, long, long);	/* $098290 */
long EvilGambit_GetSize(void);	/* $0982FC */
void EvilGambit_Init(void *, long, long, long);	/* $09830A */
long EvilGambit_ActBallsPath(void *);	/* $098376 */
long EvilMagneto_GetSize(void);	/* $0983E8 */
void EvilMagneto_Init(void *, long, long, long);	/* $0983F6 */
long EvilMagneto_StateBlast(void *);	/* $098468 */
long EvilMagneto_ActBlast(void *);	/* $098568 */
long EvilNightcrawler_GetSize(void);	/* $098606 */
void EvilNightcrawler_Init(void *, long, long, long);	/* $098614 */
void EvilNightcrawler_StateLand(void *);	/* $098680 */
long EvilNightcrawler_StateFall(void *);	/* $0986E8 */
long EvilNightcrawler_StateAppear(void *);	/* $09874E */
long EvilNightcrawler_StateTeleportIn(void *);	/* $0987E2 */
long EvilNightcrawler_StateTeleportOut(void *);	/* $098830 */
long EvilNightcrawler_ActTeleport(void *);	/* $0988F6 */
long EvilPsylocke_GetSize(void);	/* $098990 */
void EvilPsylocke_Init(void *, long, long, long);	/* $09899E */
void EvilPsylocke_StateDashEnd(void *);	/* $098A0A */
long EvilPsylocke_ActDash(void *);	/* $098A66 */
long EvilPsylocke_ActBallsPath(void *);	/* $098AD2 */
long EvilWolverine_GetSize(void);	/* $098B44 */
void EvilWolverine_Init(void *, long, long, long);	/* $098B52 */
long EvilWolverine_StateLeap(void *);	/* $098BBE */
long EvilWolverine_ActLeap(void *);	/* $098C4E */
short Clone_CharTypeOf(void *);	/* $098CC8 */
XClone *XClone_SpawnByType(Placement *place, XPlayer *player);	/* $098DA8 */
void XClone_SpawnByType_Case1(void);	/* $098DF8 fragment */
void XClone_SpawnByType_Case7(void);	/* $098F50 fragment */
void XClone_SpawnByType_Done(void);	/* $098F86 fragment */
void XClone_KillAndCount(Actor *obj);	/* $098F92 */
void XClone_LoadResources(void);	/* $098FB6 */
void XClone_StartMakerMode(void);	/* $099016 */
void XClone_LockArena(XClone *self);	/* $09907A */
void XClone_LockArena_Case1(void);	/* $0991C2 fragment */
void XClone_LockArena_Case4(void);	/* $0991CA fragment */
void XClone_LockArena_Case3(void);	/* $0991CE fragment */
void XClone_LockArena_SetTheme(void);	/* $0991DC fragment */
void XClone_FrameHookCollapse(void);	/* $09921E */
void XClone_EndWave(void);	/* $0993C0 */
void XClone_UnlockArena(XClone *self);	/* $0993DC */
long XClone_TileHookReward(short col, short row, Actor *obj);	/* $099466 */
long XClone_TileHookOpenWall(short col, short row, Actor *obj);	/* $099526 */
long XClone_InstallTileHooks(void);	/* $09980C */
void XClone_FrameHook(void);	/* $09984A */
short XClone_MakerBufSize(void);	/* $09989E */
void XClone_CamArenaUpdate(Camera *cam);	/* $0998E6 */
void XClone_CamSetArenaMode(Camera *cam);	/* $099BBE */
void XClone_CamSetNormal(Camera *cam);	/* $099BF0 */
short XClone_SegHookArenaLocked(void);	/* $099C22 */
void XClone_SetBgPalette(short n);	/* $099C36 */
void XClone_NopStatic(void);	/* $099C88 */
void XClone_PodScript(void);	/* $09A1AA fragment */
void XClone_Script1(void);	/* $09A1B0 fragment */
void XClone_Script2(void);	/* $09A316 fragment */
void XClone_Script3(void);	/* $09A33C fragment */
void XClone_Script4(void);	/* $09A35A fragment */
void XClone_Script5(void);	/* $09A480 fragment */
void XClone_Script6(void);	/* $09A570 fragment */
void XClone_Script7(void);	/* $09A804 fragment */
void XClone_Script8(void);	/* $09A822 fragment */
void XClone_Script9(void);	/* $09A8C6 fragment */
void XClone_Script10(void);	/* $09A8D2 fragment */
void XClone_Script11(void);	/* $09A8F8 fragment */
void XClone_Script12(void);	/* $09A90A fragment */
void XClone_Script13(void);	/* $09AA8A fragment */
long BroodClaw_GetSize(void);	/* $09ACCE */
long BroodClaw_Init(void *, long, long, void *);	/* $09ACDC */
long BroodClaw_Spawn(void *, char);	/* $09AD96 */
long BroodClaw_Update(void *);	/* $09AE80 */
long BroodClaw_Draw(void *);	/* $09AF3E */
long BroodClaw_Kill(void *);	/* $09B168 */
long BroodClaw_Move(void *, short);	/* $09B1AC */
void BroodClaw_Swing(void *);	/* $09B224 */
long BroodClaw_StateWaitMusic(void *);	/* $09B45E */
long BroodClaw_StateScroll(void *);	/* $09B4BC */
long BroodClaw_StateStartScroll(void *);	/* $09B50C */
long BroodClaw_StateEnterView(void *);	/* $09B55C */
long BroodClaw_StateMoveToDrop(void *);	/* $09B60A */
long BroodClaw_StateDropClone(void *);	/* $09B6A2 */
void BroodClaw_CamApplyShake(void *);	/* $09B8A0 */
long BroodClaw_StateIdle(void *);	/* $09B968 */
long BubbleManager_GetSize(void);	/* $0AB038 */
void BubbleManager_NopStatic(void);	/* $0AB046 */
void BubbleManager_Nop6A(BubbleManager *self);	/* $0AB04E */
void BubbleManager_Nop66(BubbleManager *self);	/* $0AB05A */
void BubbleManager_ClassInit(BubbleManager *self);	/* $0AB070 */
void BubbleManager_ConfigCluster(BubbleManager *self, Placement *pl);	/* $0AB080 */
void BubbleManager_ConfigBurst(BubbleManager *self, Placement *pl);	/* $0AB0E6 */
void BubbleManager_Init(BubbleManager *self, long x, long y, Placement *pl);	/* $0AB128 */
short BubbleManager_StateEdgeStart(BubbleManager *self);	/* $0AB1CA */
void BubbleManager_SpawnAtSurface(BubbleManager *self);	/* $0AB1EE */
void BubbleManager_SpawnCluster(BubbleManager *self);	/* $0AB24E */
void BubbleManager_StateEdge(BubbleManager *self);	/* $0AB2CC */
short BubbleManager_StateStart(BubbleManager *self);	/* $0AB30C */
void BubbleManager_StateCluster(BubbleManager *self);	/* $0AB350 */
void BubbleManager_SetWaveParams(BubbleManager *self);	/* $0AB370 */
void BubbleManager_SetWaveParams_Case1(void);	/* $0AB3A4 fragment */
void BubbleManager_SetWaveParams_Case2(void);	/* $0AB3B6 fragment */
void BubbleManager_SetWaveParams_Done(void);	/* $0AB416 fragment */
short BubbleManager_StateBurst(BubbleManager *self);	/* $0AB41E */
short BubbleManager_StatePause(BubbleManager *self);	/* $0AB46C */
long FloatBubble_GetSize(void);	/* $0AB510 */
void FloatBubble_NopStatic(void);	/* $0AB51E */
void FloatBubble_SetDirection(FloatBubble *self);	/* $0AB526 */
void FloatBubble_SetDirection_Up(void);	/* $0AB55C fragment */
void FloatBubble_SetDirection_Left(void);	/* $0AB5DE fragment */
void FloatBubble_SetDirection_Down(void);	/* $0AB65E fragment */
void FloatBubble_SetDirection_DownRight(void);	/* $0AB68E fragment */
void FloatBubble_SetDirection_Right(void);	/* $0AB6D8 fragment */
void FloatBubble_SetDirection_UpRight(void);	/* $0AB706 fragment */
void FloatBubble_SetDirection_Done(void);	/* $0AB750 fragment */
FloatBubble *FloatBubble_Spawn(long x, long y, short dir, short speed, short flags);	/* $0AB75A */
short FloatBubble_StateGrowStart(FloatBubble *self);	/* $0AB7A8 */
short FloatBubble_StateGrow0(FloatBubble *self);	/* $0AB7EC */
short FloatBubble_StateGrow1(FloatBubble *self);	/* $0AB822 */
short FloatBubble_StateGrow2(FloatBubble *self);	/* $0AB858 */
short FloatBubble_StateGrow3(FloatBubble *self);	/* $0AB88E */
short FloatBubble_StateGrow4(FloatBubble *self);	/* $0AB8C4 */
short FloatBubble_StateGrow5(FloatBubble *self);	/* $0AB8FA */
short FloatBubble_StateRelease(FloatBubble *self);	/* $0AB930 */
short FloatBubble_StateStart(FloatBubble *self);	/* $0AB97C */
short FloatBubble_StateWobble1(FloatBubble *self);	/* $0AB9A8 */
short FloatBubble_StateWobble2(FloatBubble *self);	/* $0AB9DE */
short FloatBubble_StateWobble3(FloatBubble *self);	/* $0ABA14 */
short FloatBubble_StatePop(FloatBubble *self);	/* $0ABA4A */
short FloatBubble_StatePop2(FloatBubble *self);	/* $0ABA92 */
short FloatBubble_StatePop3(FloatBubble *self);	/* $0ABAC8 */
void FloatBubble_StatePopEnd(FloatBubble *self);	/* $0ABAFE */
void FloatBubble_StateBurst(FloatBubble *self);	/* $0ABB1A */
void FloatBubble_Nop6A(FloatBubble *self);	/* $0ABB42 */
void FloatBubble_Nop66(FloatBubble *self);	/* $0ABB4E */
void FloatBubble_ClassInit(FloatBubble *self);	/* $0ABB64 */
char FloatBubble_OutsideActiveWindow(FloatBubble *self);	/* $0ABBBE */
void FloatBubble_Deactivate(FloatBubble *self);	/* $0ABC26 */
void FloatBubble_Move(FloatBubble *self, short steps);	/* $0ABC56 */
void FloatBubble_TakeDamage(FloatBubble *self, Actor *attacker, char type, char damage, short hx, short hy);	/* $0ABE7A */
void FloatBubble_Attack(FloatBubble *self, Actor *target, char type, char damage, short hx, short hy);	/* $0ABEDA */
void FloatBubble_Draw(FloatBubble *self);	/* $0ABF3A */
void FloatBubble_Init(FloatBubble *self, long x, long y, Placement *pl);	/* $0AC01E */
void FloatBubble_Init_Speed1(void);	/* $0AC132 fragment */
void FloatBubble_Init_Speed2(void);	/* $0AC138 fragment */
void FloatBubble_Init_Speed5(void);	/* $0AC14A fragment */
void FloatBubble_Init_SetSpeed(void);	/* $0AC14E fragment */
long FloatBubbleExplode_GetSize(void);	/* $0AC23A */
void FloatBubbleExplode_NopStatic(void);	/* $0AC248 */
void FloatBubbleExplode_Spawn(long, long);	/* $0AC250 */
long FloatBubbleExplode_StateStart(void *);	/* $0AC27C */
void FloatBubbleExplode_StateEnd(void *);	/* $0AC2B2 */
void FloatBubbleExplode_Nop6A(void);	/* $0AC2CE */
void FloatBubbleExplode_Nop66(void *);	/* $0AC2DA */
void FloatBubbleExplode_ClassInit(long);	/* $0AC2F0 */
void FloatBubbleExplode_Init(void *, long, long, long);	/* $0AC308 */
long PerimeterDoor_GetSize(void);	/* $0AC432 */
void PerimeterDoor_NopStatic(void);	/* $0AC440 */
void PerimeterDoor_Nop66(void *);	/* $0AC448 */
long PerimeterDoor_ClassInit(void);	/* $0AC45E */
long PerimeterDoor_Init(void *, long, long, void *);	/* $0AC46E */
void PerimeterDoor_TileHit(void *, short, short, void *);	/* $0AC4E8 */
long PerimeterDoor_StateStart(void *);	/* $0AC91C */
long PerimeterDoor_StateWaitBroken(void *);	/* $0AC956 */
long PerimeterDoor_StateCrumble(void *);	/* $0AC982 */
long PerimeterDoor_StateWaitExit(void *);	/* $0AD060 */
void PerimeterDoor_StateDone(void);	/* $0AD166 */
void SavageA2_EmptyStub2(void);	/* $0AD16E */
long PerimeterDoor_TileHook(short, short, long, short, short);	/* $0AD176 */
long SavageA2_SetBlockTiles(short, short, short, char, short);	/* $0AD1BC */
void SavageA2_TileHookCrack(short, short);	/* $0AD342 */
void SavageA2_InstallTileHooks(void);	/* $0AD3E8 */
void SavageA2_InitLights(void);	/* $0AD420 */
void SavageA2_AnimLights(void);	/* $0AD486 */
void SavageA2_CamUpdateFlood(long);	/* $0AD550 */
void SavageA2_CamApply(void *);	/* $0AD674 */
void SavageA2_CamSetHooks(void *);	/* $0AD790 */
void SavageA2_InitCamera(void);	/* $0AD7BE */
void SavageA2_InitFlood(void);	/* $0AD7E4 */
void SavageA2_FrameHook(void);	/* $0AD824 */
void SavageA2_LoadResources(void);	/* $0AD84A */
void SavageA2_LoadLevel(void);	/* $0AD866 */
void SavageA2_PlacePlayers(void);	/* $0AD8EC */
void SavageA2_EmptyStub1(void);	/* $0AD902 */
void SavageA2_InstallFrameHook(void);	/* $0AD90A */
void SavageA2_RunScene(void);	/* $0AD91A */
long TrapDoor_GetSize(void);	/* $0AD9E6 */
void TrapDoor_NopStatic(void);	/* $0AD9F4 */
void TrapDoor_Nop66(void *);	/* $0AD9FC */
long TrapDoor_ClassInit(void);	/* $0ADA12 */
long TrapDoor_Init(void *, long, long, void *);	/* $0ADA1E */
long TrapDoor_SetTiles(void *, short);	/* $0ADB1A */
long TrapDoor_StateStart(void *);	/* $0ADB7E */
long TrapDoor_StateArm(void *);	/* $0ADBBE */
long TrapDoor_StateWaitPlayer(void *);	/* $0ADBE8 */
long TrapDoor_StateSag(void *);	/* $0ADC26 */
long TrapDoor_StateSpring(void *);	/* $0ADCD8 */
long TrapDoor_StateOpen(void *);	/* $0ADD8A */
long TrapDoor_StateOpenWide(void *);	/* $0ADE06 */
long TrapDoor_StateClosing(void *);	/* $0ADE40 */
long TrapDoor_StateClosed(void *);	/* $0ADE7A */
void SavageA3_EmptyStub2(void);	/* $0B02EC */
void SavageA3_FrameStub(void);	/* $0B02F4 */
void SavageA3_TileHookCrack(short, short);	/* $0B02FC */
void SavageA3_InstallTileHooks(void);	/* $0B03A2 */
void SavageA3_CamSetHooks(void *);	/* $0B03DA */
void SavageA3_InitCamera(void);	/* $0B0408 */
void SavageA3_FrameHook(void);	/* $0B042E */
void SavageA3_LoadResources(void);	/* $0B0468 */
void SavageA3_LoadLevel(void);	/* $0B0484 */
void SavageA3_PlacePlayers(void);	/* $0B050A */
void SavageA3_EmptyStub1(void);	/* $0B0520 */
void SavageA3_InstallFrameHook(void);	/* $0B0528 */
void SavageA3_RunScene(void);	/* $0B0538 */
void SavageA4_ResetPalCycles(void);	/* $0B600A */
void SavageA4_CyclePalettes(void);	/* $0B602A */
void SavageA4_InitEffectSprite(void);	/* $0B6086 */
void SavageA4_AnimEffectSprite(void);	/* $0B60BE */
void SavageA4_FrameHook(void);	/* $0B611E */
void SavageA4_LoadResources(void);	/* $0B6252 */
void SavageA4_LoadLevel(void);	/* $0B626E */
void SavageA4_InstallHooks(void);	/* $0B62F2 */
void SavageA4_CallEmptyStub(void);	/* $0B6322 */
long SavageA4_CamUpdate(void *);	/* $0B632E */
void SavageA4_CamSetHooks(void *);	/* $0B65A0 */
void SavageA4_RunScene(void);	/* $0B65C6 */
void SavageA4_EmptyStub(void);	/* $0B6632 */
void SavageA4_InitBossStages(void);	/* $0B66DC */
long BrainBabyBio_GetSize(void);	/* $0B6740 */
void BrainBabyBio_Init(void *, long, long, long);	/* $0B674E */
void BrainBabyBio_OnDeath(void *);	/* $0B6824 */
void BrainBabyBio_SetHome(void *, short, short);	/* $0B689A */
void BrainBabyBio_StateStart(void *);	/* $0B68B8 */
long BrainChild_GetSize(void);	/* $0B68FC */
long BrainChild_ClassInit(void);	/* $0B690A */
void BrainChild_Init(void *, long, long, long);	/* $0B6916 */
void BrainChild_Move(void *, short);	/* $0B6A00 */
void BrainChild_OnDeath(void *);	/* $0B6B2A */
long BrainChild_StateStart(void *);	/* $0B6BB2 */
long BrainChild_StateLeap(void *);	/* $0B6BD6 */
long BrainChild_StateRise1(void *);	/* $0B6C3E */
long BrainChild_StateRise2(void *);	/* $0B6C8E */
long BrainChild_StateApex(void *);	/* $0B6CDE */
long BrainChild_StateFall(void *);	/* $0B6D2E */
long BrainChild_StateFallHome(void *);	/* $0B6D7E */
long BrainChild_StateLand(void *);	/* $0B6DD4 */
void BrainChild_ActFacePlayer(void *);	/* $0B6EA6 */
long BrainChild_ActShoot(void *);	/* $0B6F28 */
void BrainChild_StateFire(void *);	/* $0B6F5E */
void BrainChild_ActIdle30(void *);	/* $0B703E */
long ChildShip_GetSize(void);	/* $0B70A2 */
long ChildShip_Init(void *, long, long, long);	/* $0B70B0 */
long ChildShip_Update(void *);	/* $0B71F4 */
long ChildShip_Move(void *, short);	/* $0B7298 */
long ChildShip_TakeDamage(void *, long, char, char);	/* $0B7394 */
void ChildShip_OnHurt(void *, long, char);	/* $0B7436 */
void ChildShip_OnHurt_Up(void);	/* $0B7472 fragment */
void ChildShip_OnHurt_Left(void);	/* $0B7482 fragment */
void ChildShip_OnHurt_Right(void);	/* $0B748A fragment */
void ChildShip_OnHurt_SetDecel(void);	/* $0B7490 fragment */
long ChildShip_StateKnockback(void *);	/* $0B74CC */
long ChildShip_StateKnockbackSlow(void *);	/* $0B74F6 */
void ChildShip_StateReturnY(void *);	/* $0B757C */
long ChildShip_StateCrash(void *);	/* $0B7622 */
long ChildShip_StateCrash1(void *);	/* $0B765C */
long ChildShip_StateCrash2(void *);	/* $0B76BC */
long ChildShip_StateCrash3(void *);	/* $0B771C */
long ChildShip_StateReleaseChild(void *);	/* $0B777C */
void ChildShip_StateDie(long);	/* $0B77E8 */
long ChildShip_StateEnter(void *);	/* $0B77FC */
long ChildShip_ActDropRocket(void *);	/* $0B78B8 */
void ChildShip_ActWait60(void *);	/* $0B7986 */
void ChildShip_ActWait20(void *);	/* $0B79DA */
long ChildShip_ActOverPlayer(void *);	/* $0B7A2E */
void SavageA4_NopStatic(void);	/* $0B7B14 */
void BrainBabyBio_Script(void);	/* $0B7CDA fragment */
void ChildShip_Script(void);	/* $0B7CF0 fragment */
void SavageBio_Script1(void);	/* $0B7E2E fragment */
void SavageBio_Script2(void);	/* $0B7E48 fragment */
void SavageBio_Script3(void);	/* $0B7E56 fragment */
void SavageBio_Script4(void);	/* $0B814C fragment */
void SavageBio_Script5(void);	/* $0B8178 fragment */
long SavageBioBase_GetSize(void);	/* $0BA19E */
long SavageBioBase_ClassInit(void);	/* $0BA1AC */
long SavageBioBase_Init(void *, long, long, void *);	/* $0BA1B8 */
long SavageBioBase_Move(void *, short);	/* $0BA258 */
long SavageBioBase_Update(void *);	/* $0BA35E */
long SavageBioBase_SetFrame(void *, short);	/* $0BA4A2 */
long SavageBigBio_GetSize(void);	/* $0BA562 */
long SavageBigBio_ClassInit(long);	/* $0BA570 */
long SavageBigBio_Init(void *, long, long, long);	/* $0BA58E */
long SavageBigBio_TakeDamage(void *, long, long, char);	/* $0BA6CE */
long SavageBigBio_Move(void *, short);	/* $0BA77C */
void SavageBigBio_TentacleHit(void *, short, void *, short, short);	/* $0BA7D8 */
long SavageBigBio_Update(long);	/* $0BA864 */
long SavageBigBio_Draw(void *);	/* $0BA8FA */
void SavageBio_ActScript2052(SavageBigBio *self);	/* $0BAEC4 */
short SavageBio_ActFacePlayer(SavageBigBio *self);	/* $0BAF14 */
short SavageBio_ActFireRocket(SavageBigBio *self);	/* $0BAF96 */
short SavageBio_StateFireMouth(SavageBigBio *self);	/* $0BAFF4 */
short SavageBio_StateFireTip(SavageBigBio *self);	/* $0BB0D6 */
short SavageBio_ActFireVertical(SavageBigBio *self);	/* $0BB268 */
short SavageBio_StateFireVertical(SavageBigBio *self);	/* $0BB2AA */
short SavageBio_ActReloadArt14(SavageBigBio *self);	/* $0BB38C */
short SavageBio_StateReloadArt14(SavageBigBio *self);	/* $0BB3CC */
short SavageBio_ActReloadArt10(SavageBigBio *self);	/* $0BB45C */
short SavageBio_StateReloadArt10(SavageBigBio *self);	/* $0BB49C */
short SavageBio_ActExtendTentacle(SavageBigBio *self);	/* $0BB52E */
short SavageBio_ActExtendTentacle2(SavageBigBio *self);	/* $0BB590 */
short SavageBio_StateExtend(SavageBigBio *self);	/* $0BB5E0 */
short SavageBio_StateRetract(SavageBigBio *self);	/* $0BB634 */
short SavageBio_StateRetractStart(SavageBigBio *self);	/* $0BB698 */
short SavageBio_StateNextStep(SavageBigBio *self);	/* $0BB6CE */
short SavageBio_ActExtendShort(SavageBigBio *self);	/* $0BB71E */
short SavageBio_StateExtend10(SavageBigBio *self);	/* $0BB780 */
void SavageBio_ActAnimA(SavageBigBio *self);	/* $0BB7F0 */
void SavageBio_ActAnimB(SavageBigBio *self);	/* $0BB850 */
void SavageBio_ActAnimC(SavageBigBio *self);	/* $0BB8B0 */
short SavageBio_ActCharge(SavageBigBio *self);	/* $0BB910 */
short SavageBio_StateChargeToEdge(SavageBigBio *self);	/* $0BBA0E */
void SavageBio_ActChargeGo(SavageBigBio *self);	/* $0BBA94 */
short SavageBio_ActChargePlayer(SavageBigBio *self);	/* $0BBBBE */
short SavageBio_StateChargeToPlayer(SavageBigBio *self);	/* $0BBCBC */
short SavageBio_ActDashPast(SavageBigBio *self);	/* $0BBE0C */
short SavageBio_ActDashTo(SavageBigBio *self);	/* $0BBF4E */
short SavageBio_StateDash(SavageBigBio *self);	/* $0BC0C4 */
short SavageBio_StateDashBrake(SavageBigBio *self);	/* $0BC1A8 */
void SavageBio_ActFrame40(SavageBigBio *self);	/* $0BC2D2 */
void SavageBio_ActStop60(SavageBigBio *self);	/* $0BC338 */
void SavageBio_ActStop30(SavageBigBio *self);	/* $0BC398 */
void SavageBio_ActWait30(SavageBigBio *self);	/* $0BC3F8 */
void SavageBio_ActWait5(SavageBigBio *self);	/* $0BC44C */
void SavageBio_ActFrame4F(SavageBigBio *self);	/* $0BC4A0 */
void SavageBio_ActFrame4FB(SavageBigBio *self);	/* $0BC516 */
short SavageBio_StateWaitCrossing(SavageBigBio *self);	/* $0BC58C */
short SavageBio_StateCrossSurface(SavageBigBio *self);	/* $0BC64A */
long SavageBioBalls_ActLeap(void *);	/* $0BC71A */
void SavageBioBalls_StateWait(void *);	/* $0BC7A2 */
long SavageBioBalls_ActMotion(void *);	/* $0BC80C */
short SavageBio_StateDying(SavageBigBio *self);	/* $0BC85C */
short SavageBio_ActCrossIfUp(SavageBigBio *self);	/* $0BC8AC */
short SavageBio_ActToCentre(SavageBigBio *self);	/* $0BC920 */
short SavageBio_StateToCentre(SavageBigBio *self);	/* $0BC954 */
short SavageBio_StateDeathWait(SavageBigBio *self);	/* $0BCA2C */
short SavageBio_ActDie(SavageBigBio *self);	/* $0BCA7E */
short SavageBio_StateDeathExplode(SavageBigBio *self);	/* $0BCB92 */
long SavageMediumBio_GetSize(void);	/* $0BCC6A */
void SavageMediumBio_ClassInit(long);	/* $0BCC78 */
void SavageMediumBio_Init(void *, long, long, long);	/* $0BCC96 */
void SavageMediumBio_TakeDamage(long, long, char, char, short, short);	/* $0BCD4E */
void SavageMediumBio_OnDeath(void *);	/* $0BCD90 */
void SavageMediumBio_SetHome(void *, short, short);	/* $0BCDF0 */
long SavageMediumBio_StateAppear(void *);	/* $0BCE0E */
void SavageMediumBio_StateReady(void *);	/* $0BCE56 */
long SavageMediumBio_ActWalk(void *);	/* $0BCED6 */
void SavageMediumBio_StateWalk(void *);	/* $0BCF2A */
void SavageMediumBio_ActFacePlayer(void *);	/* $0BCF94 */
long SavageMediumBio_ActShoot(void *);	/* $0BD00A */
void SavageMediumBio_StateFire(void *);	/* $0BD04C */
void SavageMediumBio_StateBurst(void *);	/* $0BD128 */
long SavageBioBalls_ActOpen(void *);	/* $0BD1D8 */
void SavageBioBalls_ActBecomeBigBio(void *);	/* $0BD216 */
void SavageBio_NopStatic(void);	/* $0BD274 */
long Barbarian_GetSize(void);	/* $0BD51E */
short Barbarian_SegHook1A(short mode);	/* $0BD52C */
void Barbarian_FindPlatforms(Barbarian *self, Placement *pl);	/* $0BD54A */
void Barbarian_StandOn(Barbarian *self, Seg *seg);	/* $0BD6E2 */
char Barbarian_TargetClose(Barbarian *self);	/* $0BD75C */
void Barbarian_StepTargetX(Barbarian *self, Seg *seg, short dx);	/* $0BD79A */
char Barbarian_TargetPlayer(Barbarian *self, XPlayer *p);	/* $0BD7EA */
char Barbarian_SetupLeap(Barbarian *self);	/* $0BD894 */
void Barbarian_NotifySegmentGone(Seg *seg);	/* $0BD9F0 */
short Barbarian_StateStart(Barbarian *self);	/* $0BDA66 */
short Barbarian_StateIdle(Barbarian *self);	/* $0BDAC4 */
short Barbarian_StateWatchP1(Barbarian *self);	/* $0BDB04 */
short Barbarian_StateWatchP2(Barbarian *self);	/* $0BDB4C */
short Barbarian_StateIdleTimer(Barbarian *self);	/* $0BDB94 */
short Barbarian_StateTaunt(Barbarian *self);	/* $0BDBE2 */
void Barbarian_StateTaunt_Reset(void);	/* $0BDC34 fragment */
void Barbarian_StateTaunt_Play(void);	/* $0BDC4A fragment */
void Barbarian_StateTaunt_Wait(void);	/* $0BDC7E fragment */
short Barbarian_StateEngage(Barbarian *self);	/* $0BDC9A */
short Barbarian_StateApproach(Barbarian *self);	/* $0BDCFE */
short Barbarian_StateWalkStart(Barbarian *self);	/* $0BDD52 */
short Barbarian_StateWalkRight(Barbarian *self);	/* $0BDDC2 */
short Barbarian_StateWalkLeft(Barbarian *self);	/* $0BDE22 */
short Barbarian_StateSegmentEnd(Barbarian *self);	/* $0BDE82 */
short Barbarian_StateReachedTarget(Barbarian *self);	/* $0BDEF4 */
short Barbarian_StateTryLeap(Barbarian *self);	/* $0BDF50 */
short Barbarian_StateLeap(Barbarian *self);	/* $0BDF9C */
short Barbarian_StateLeapRise(Barbarian *self);	/* $0BDFD8 */
short Barbarian_StateLeapFall(Barbarian *self);	/* $0BE014 */
short Barbarian_StateLeapDrop(Barbarian *self);	/* $0BE04E */
short Barbarian_StateLeapNear(Barbarian *self);	/* $0BE08A */
short Barbarian_StateLeapLand(Barbarian *self);	/* $0BE0C2 */
short Barbarian_StateLandSlide(Barbarian *self);	/* $0BE10C */
short Barbarian_StateLanded(Barbarian *self);	/* $0BE15C */
short Barbarian_StateLeapFailed(Barbarian *self);	/* $0BE190 */
short Barbarian_StateTryLeap2(Barbarian *self);	/* $0BE1BA */
short Barbarian_StateToLeapEdge(Barbarian *self);	/* $0BE206 */
short Barbarian_StateLeapEdge(Barbarian *self);	/* $0BE22A */
short Barbarian_StateGiveUp(Barbarian *self);	/* $0BE29A */
short Barbarian_StateAttack(Barbarian *self);	/* $0BE2CE */
void Barbarian_StateAttack_Reset(void);	/* $0BE310 fragment */
void Barbarian_StateAttack_Case1(void);	/* $0BE328 fragment */
void Barbarian_StateAttack_Case4(void);	/* $0BE364 fragment */
void Barbarian_StateAttack_Done(void);	/* $0BE376 fragment */
short Barbarian_StateAttack0(Barbarian *self);	/* $0BE37E */
short Barbarian_StateAttack1(Barbarian *self);	/* $0BE3B4 */
short Barbarian_StateAttack2(Barbarian *self);	/* $0BE3EA */
short Barbarian_StateAttack3(Barbarian *self);	/* $0BE420 */
short Barbarian_StateAttack4(Barbarian *self);	/* $0BE456 */
short Barbarian_StateAttackEnd(Barbarian *self);	/* $0BE48C */
short Barbarian_StateScrollTiles(Barbarian *self);	/* $0BE4C0 */
void Barbarian_Nop6A(Barbarian *self);	/* $0BE55C */
void Barbarian_ClassInit(Barbarian *self);	/* $0BE564 */
void Barbarian_Nop66(Barbarian *self);	/* $0BE57E */
void Barbarian_Move(Barbarian *self, short n);	/* $0BE594 */
void Barbarian_Draw(Barbarian *self);	/* $0BE5D6 */
void Barbarian_TakeDamage(Barbarian *self, Actor *attacker, char type, char damage, short hx, short hy);	/* $0BE602 */
void Barbarian_EndHurt(Barbarian *self);	/* $0BE656 */
void Barbarian_Init(Barbarian *self, long x, long y, Placement *pl);	/* $0BE6A4 */
long Launcher_GetSize(void);	/* $0BE7DA */
void Launcher_ClearShots(void *);	/* $0BE7E8 */
void Launcher_AddShot(void *, short);	/* $0BE814 */
long Launcher_StateStart(void *);	/* $0BE872 */
long Launcher_StateFrame0(void *);	/* $0BE896 */
long Launcher_StateFrame1(void *);	/* $0BE8EE */
long Launcher_StateFire(void *);	/* $0BE946 */
void Launcher_Nop6A(void);	/* $0BE99C */
long Launcher_ClassInit(long);	/* $0BE9A4 */
void Launcher_Nop66(void *);	/* $0BE9E2 */
long Launcher_ShotHitsPlayer(void *, short, short, long);	/* $0BE9F8 */
long Launcher_Draw(void *);	/* $0BEA74 */
long Launcher_Init(void *, long, long, void *);	/* $0BEC6A */
long SporeMine_GetSize(void);	/* $0BED84 */
void SporeMine_Nop66(void *);	/* $0BED92 */
void SporeMine_Nop6A(void);	/* $0BEDA8 */
long SporeMine_ClassInit(void);	/* $0BEDB0 */
void SporeMine_Init(void *, long, long, long);	/* $0BEDD2 */
void SporeMine_InitPool(void);	/* $0BEE38 */
long SporeMine_RemoveSpore(long);	/* $0BEED0 */
void SporeMine_UpdatePool(void);	/* $0BEF12 */
void SporeMine_DrawPool(void);	/* $0BF050 */
long SporeMine_AddSpore(void *);	/* $0BF106 */
long SporeMine_SpawnFromRec(long, void *);	/* $0BF186 */
void SporeMine_RecIdle(void *);	/* $0BF1C2 */
long SporeMine_RecFindTarget(void *);	/* $0BF1FA */
void SporeMine_RecWatch(void *);	/* $0BF234 */
void SporeMine_RecArm1(void *);	/* $0BF2B2 */
void SporeMine_RecArm2(void *);	/* $0BF2FC */
void SporeMine_RecArm3(void *);	/* $0BF346 */
void SporeMine_RecArm4(void *);	/* $0BF390 */
long SporeMine_RecExplode(void *);	/* $0BF3DA */
long SpikeBall_GetSize(void);	/* $0BF5F8 */
void SpikeBall_StateCycle(void *);	/* $0BF606 */
void SpikeBall_Nop6A(void);	/* $0BF654 */
long SpikeBall_ClassInit(long);	/* $0BF65C */
long SpikeBall_Draw(void *);	/* $0BF680 */
void SpikeBall_Nop66(void *);	/* $0BF76E */
long SpikeBall_Init(void *, long, long, void *);	/* $0BF784 */
void SavageJungle_InitPalPhases(void);	/* $0BF898 */
void SavageJungle_PalPhaseUpdate(void);	/* $0BF8DC */
void SavageJungle_PalPhaseUpdate_Case0(void);	/* $0BF914 fragment */
void SavageJungle_PalPhaseUpdate_Case1(void);	/* $0BF93A fragment */
void SavageJungle_PalPhaseUpdate_Done(void);	/* $0BF9C8 fragment */
long SwampFly_SpawnRecInView(void *, void *);	/* $0BF9EA */
long SwampBubbles_GetSize(void);	/* $0BFA7C */
long SwampBubbles_StateIdle(void *);	/* $0BFA8A */
void SwampBubbles_Nop6A(void);	/* $0BFAC0 */
void SwampBubbles_ClassInit(long);	/* $0BFAC8 */
void SwampBubbles_Init(void *, long, long, long);	/* $0BFADA */
long SwampBubbles_Draw(void *);	/* $0BFB44 */
void SwampBubbles_Draw_Multi(void);	/* $0BFB9A fragment */
void SwampBubbles_Draw_Done(void);	/* $0BFC94 fragment */
void SwampBubbles_Nop66(void *);	/* $0BFC9E */
long SwampBubbles_Deactivate(void *);	/* $0BFCB4 */
long SwampPlatform_GetSize(void);	/* $0BFD60 */
long SwampPlatform_StateIdle(void *);	/* $0BFD6E */
void SwampPlatform_Nop6A(void);	/* $0BFDB0 */
void SwampPlatform_ClassInit(void);	/* $0BFDB8 */
void SwampPlatform_Nop66(void *);	/* $0BFDD6 */
void SwampPlatform_Move(void *);	/* $0BFDEC */
void SwampPlatform_Draw(void *);	/* $0BFE6C */
void SwampPlatform_Init(void *, long, long, void *);	/* $0BFF8C */
long SwampFly_GetSize(void);	/* $0C095A */
long SwampFly_StateStart(void *);	/* $0C0968 */
long SwampFly_StatePickPath(void *);	/* $0C099C */
void SwampFly_StatePickPath_Default(void);	/* $0C09D6 fragment */
void SwampFly_StatePickPath_Case1(void);	/* $0C09E2 fragment */
void SwampFly_StatePickPath_Case5(void);	/* $0C0A12 fragment */
void SwampFly_StatePickPath_Start(void);	/* $0C0A1C fragment */
long SwampFly_StateFly(void *);	/* $0C0A52 */
void SwampFly_Nop6A(void);	/* $0C0B7C */
void SwampFly_ClassInit(long);	/* $0C0B84 */
void SwampFly_Nop66(void *);	/* $0C0B96 */
void SwampFly_Draw(void *);	/* $0C0BAC */
void SwampFly_Init(void *, long, long, void *);	/* $0C0C0C */
void SavageJungle_EmptyStub(void);	/* $0C0DBA */
void Water_Init(short, short, short);	/* $0C0DC2 */
void SavageC1_SetParallax(void);	/* $0C0E16 */
long SavageC1_FrameHook(void);	/* $0C0E2A */
void SavageC1_LoadLevel(void);	/* $0C0E54 */
void SavageJungle_ClearColor0(void);	/* $0C0ED4 */
void SavageJungle_PlacePlayers(void);	/* $0C0EE0 */
void Water_DrawSplash(long, short, short, short);	/* $0C0F88 */
void Water_DrawHook(void);	/* $0C0FD2 */
void SavageC1_InstallHooks(void);	/* $0C11B6 */
void SavageC1_RunScene(void);	/* $0C11CE */
void SavageB1_FrameHook(void);	/* $0C122A */
void SavageB1_LoadLevel(void);	/* $0C1258 */
void SavageB1_InstallHooks(void);	/* $0C12DE */
void SavageB1_SetParallax(void);	/* $0C12F6 */
void SavageB1_RunScene(void);	/* $0C130A */
long Player_MusicVoice(void *);	/* $0C1372 */
void Music_SetPlayerParams(void);	/* $0C1412 */
void Music_PlayStage(short);	/* $0C1478 */
void Music_StopStage(void);	/* $0C14B4 */
void SentinelsD1_InitFlagBits(void);	/* $0DC294 */
void SentinelsD1_EmptyStub(void);	/* $0DC2C6 */
void SentinelsD1_AnimLight(void);	/* $0DC2CE */
void SentinelsD1_InitLight(void);	/* $0DC316 */
long SentinelsD1_FrameHook(void);	/* $0DC350 */
void SentinelsB2_EmptyStub2(void);	/* $0DC38C */
void SentinelsD1_LoadResources(void);	/* $0DC394 */
long SentinelsD1_LoadLevel(void);	/* $0DC3DE */
void SentinelsD1_InstallHooks(void);	/* $0DC47C */
void SentinelsD1_RunScene(void);	/* $0DC4AA */
long ClosedBarrier_GetSize(void);	/* $0DC4DE */
void ClosedBarrier_Init(void *, long, long, void *);	/* $0DC4EC */
void ClosedBarrier_Update(void *);	/* $0DC59C */
long Barrier_GetSize(void);	/* $0DC5FE */
void Barrier_Nop6A(void);	/* $0DC60C */
void Barrier_ClassInit(void);	/* $0DC618 */
void Barrier_Init(void *, long, long, void *);	/* $0DC638 */
void Barrier_Despawn(void *, void *);	/* $0DC776 */
long Barrier_StateStart(void *);	/* $0DC7D0 */
long Barrier_StateWatch(void *);	/* $0DC816 */
long Barrier_StateIdle(void *);	/* $0DC9A0 */
void Barrier_Close(void *);	/* $0DC9C4 */
void Barrier_NopStatic(void);	/* $0DCA26 */
long Blast_GetSize(void);	/* $0DCB20 */
void Blast_Spawn(void *);	/* $0DCB2E */
void Blast_Nop66(void *);	/* $0DCB7A */
void Blast_Nop6A(void);	/* $0DCB90 */
void Blast_ClassInit(void);	/* $0DCB98 */
void Blast_Init(void *, long, long, void *);	/* $0DCBC8 */
long Blast_OutsideActiveWindow(void *);	/* $0DCD18 */
void Blast_Attack(void *, void *, char, char, short, short);	/* $0DCD4E */
void Blast_Draw(void *);	/* $0DCDD8 */
void Blast_SetFlags(void *, short);	/* $0DCE4E */
void Blast_ClearFlags(void *, short);	/* $0DCE6A */
long Blast_StateStart(void *);	/* $0DCE88 */
long Blast_StateLaunch(void *);	/* $0DCEBE */
long Blast_StateFly(void *);	/* $0DCF0A */
long Blast_StateStick(void *);	/* $0DCF58 */
long Blast_StateUnstick(void *);	/* $0DCFBC */
void Blast_StateKill(void *);	/* $0DCFF0 */
long Blast_Latch(void *);	/* $0DD00C */
void Blast_StateZap(void *);	/* $0DD060 */
long ControlBox_GetSize(void);	/* $0DD5F6 */
void ControlBox_Nop66(void *);	/* $0DD604 */
void ControlBox_Nop6A(void);	/* $0DD61A */
long ControlBox_ClassInit(void);	/* $0DD622 */
void ControlBox_Init(void *, long, long, void *);	/* $0DD62A */
void ControlBox_Draw(void *);	/* $0DD75A */
void ControlBox_StartPattern(void *);	/* $0DD804 */
void ControlBox_FireBlast(void *, long, long);	/* $0DD856 */
long ControlBox_StateWaitSwitch(void *);	/* $0DD8AE */
long ControlBox_StateActivate(void *);	/* $0DD8E2 */
long ControlBox_StateArm(void *);	/* $0DD956 */
void ControlBox_StateFire(void *);	/* $0DD980 */
long ControlPanel_GetSize(void);	/* $0DDEB8 */
void ControlPanel_Nop66(void *);	/* $0DDEC6 */
void ControlPanel_Nop6A(void);	/* $0DDEDC */
long ControlPanel_ClassInit(void);	/* $0DDEE4 */
void ControlPanel_Init(void *, long, long, void *);	/* $0DDEEC */
void ControlPanel_Draw(void *);	/* $0DE0B4 */
void ControlPanel_SetFlags(void *, short);	/* $0DE15E */
void ControlPanel_StartPattern(void *);	/* $0DE17A */
void ControlPanel_FireBlast(void *, long, long);	/* $0DE1CC */
void ControlPanel_StateIntact(void *);	/* $0DE21A */
void ControlPanel_StateDamaged1(void *);	/* $0DE240 */
void ControlPanel_StateDamaged2(void *);	/* $0DE274 */
long ControlPanel_StateDestroyed(void *);	/* $0DE2A8 */
long ControlPanel_StateArm(void *);	/* $0DE33C */
void ControlPanel_StateFire(void *);	/* $0DE366 */
long ControlPanel_StateHit(void *);	/* $0DE3E8 */
long SentinelDoor_GetSize(void);	/* $0DE516 */
void SentinelDoor_Nop66(void *);	/* $0DE524 */
void SentinelDoor_Nop6A(void);	/* $0DE53A */
long SentinelDoor_ClassInit(void);	/* $0DE542 */
void SentinelDoor_Init(void *, long, long, void *);	/* $0DE54A */
void SentinelDoor_Despawn(void *, long);	/* $0DE66E */
long SentinelDoor_StateStart(void *);	/* $0DE6A6 */
long SentinelDoor_StateClosed(void *);	/* $0DE6CA */
long SentinelDoor_StateOpen(void *);	/* $0DE71A */
long SentinelDoor_StateOpen1(void *);	/* $0DE76A */
long SentinelDoor_StateOpen2(void *);	/* $0DE7B2 */
long SentinelDoor_StateOpen3(void *);	/* $0DE7E8 */
long SentinelDoor_StateOpen4(void *);	/* $0DE81E */
long SentinelDoor_StateOpen5(void *);	/* $0DE854 */
long SentinelDoor_StateClose1(void *);	/* $0DE88E */
long SentinelDoor_StateClose2(void *);	/* $0DE8C4 */
long SentinelDoor_StateClose3(void *);	/* $0DE8FA */
long SentinelDoor_StateClose4(void *);	/* $0DE930 */
long SentinelDoor_StateClose5(void *);	/* $0DE966 */
long SentinelElevator_GetSize(void);	/* $0DF1FC */
void SentinelElevator_Nop66(void *);	/* $0DF20A */
void SentinelElevator_Nop6A(void);	/* $0DF220 */
long SentinelElevator_ClassInit(void);	/* $0DF228 */
void SentinelElevator_Init(void *, long, long, void *);	/* $0DF230 */
long SentinelElevator_StateAuto(void *);	/* $0DF34C */
long SentinelElevator_StateStart(void *);	/* $0DF38E */
long SentinelElevator_StateWaitSwitch(void *);	/* $0DF3C2 */
long SentinelElevator_StateGoUp(void *);	/* $0DF3F6 */
long SentinelElevator_StateRising(void *);	/* $0DF426 */
long SentinelElevator_StateGoDown(void *);	/* $0DF4A6 */
long SentinelElevator_StateSinking(void *);	/* $0DF4D6 */
void SentinelsA1_ResetPalCycles(void);	/* $0E72B6 */
void SentinelsA1_CyclePalettes(void);	/* $0E7306 */
void SentinelsA1_InitZapLights(void);	/* $0E7582 */
void SentinelsA1_StepZapLights(void);	/* $0E76DA */
void SentinelsA1_FrameHook(void);	/* $0E785E */
void SentinelsA1_LoadResources(void);	/* $0E789A */
void SentinelsA1_LoadLevel(void);	/* $0E78F2 */
void SentinelsA1_InstallHooks(void);	/* $0E79A0 */
void SentinelsA1_CallEmptyStub(void);	/* $0E7A5E */
void SentinelsA1_RunScene(void);	/* $0E7A6A */
void SentinelsA1_EmptyStub(void);	/* $0E7A8C */
long HoloBolt_GetSize(void);	/* $0E808A */
long HoloBolt_Spawn(long, long, short, short);	/* $0E8098 */
long HoloBolt_StateAppear(void *);	/* $0E80DE */
long HoloBolt_StateActivate(void *);	/* $0E8114 */
long HoloBolt_StateEnd(void *);	/* $0E8162 */
void HoloBolt_StateDone(void);	/* $0E8196 */
long HoloBolt_Update(void *);	/* $0E819E */
void HoloBolt_Init(void *, long, long, long);	/* $0E82E4 */
long Hologram_GetSize(void);	/* $0E99D6 */
void Hologram_EmptyStub(void);	/* $0E99E4 */
void Hologram_SetXPowerSpawns(char);	/* $0E99EC */
void Hologram_SetPose(short);	/* $0E9A5C */
long Hologram_FindLifts(void *);	/* $0E9A8C */
void Hologram_FindGateSeg(void *);	/* $0E9B2E */
char Hologram_PlayerInArena(void *, void *);	/* $0E9B72 */
void Hologram_LockArena(void *);	/* $0E9BCE */
void Hologram_BeginDying(void);	/* $0E9C04 */
void Hologram_Explode(void *);	/* $0E9C5E */
void Hologram_OpenExit(void *);	/* $0E9D3A */
long Hologram_StateStart(void *);	/* $0E9D88 */
long Hologram_StateSetup(void *);	/* $0E9DAE */
long Hologram_WaitLowerLiftBoarded(void *);	/* $0E9DEA */
long Hologram_WaitLowerLiftDown(void *);	/* $0E9E36 */
void Hologram_WaitPlayersInArena(void *);	/* $0E9E68 */
void Hologram_StartDyingScript(void *);	/* $0E9ED4 */
void Hologram_ExplodeWhenReady(void *);	/* $0E9F24 */
long Hologram_StateOpenExit(void *);	/* $0E9F58 */
long Hologram_WaitLowerLiftBoarded2(void *);	/* $0E9F84 */
long Hologram_WaitLowerLiftUp(void *);	/* $0E9FC0 */
long Hologram_WaitUpperLiftBoarded(void *);	/* $0E9FF2 */
long Hologram_RideUpperLift(void *);	/* $0EA02E */
void Hologram_StateDone(void);	/* $0EA098 */
void Hologram_StartAttackScript(void *);	/* $0EA0A0 */
void Hologram_ActPose0(void *);	/* $0EA100 */
long Hologram_SeqA_Pose1(void *);	/* $0EA15E */
long Hologram_SeqA_Pose2(void *);	/* $0EA18E */
long Hologram_SeqA_Pose1b(void *);	/* $0EA1BE */
void Hologram_SeqA_Pose0(void *);	/* $0EA1EE */
long Hologram_SeqB_Pose3(void *);	/* $0EA24C */
long Hologram_SeqB_Pose4(void *);	/* $0EA27C */
long Hologram_SeqB_Pose3b(void *);	/* $0EA2AC */
void Hologram_SeqB_Pose0(void *);	/* $0EA2DC */
long Hologram_Raise_Pose3(void *);	/* $0EA33A */
void Hologram_Raise_Pose4(void *);	/* $0EA36A */
long Hologram_Lower_Pose3(void *);	/* $0EA3C8 */
void Hologram_Lower_Pose0(void *);	/* $0EA3F8 */
long Hologram_Fire1_Pose6(void *);	/* $0EA456 */
void Hologram_Fire1_Bolts(void *);	/* $0EA486 */
long Hologram_Fire2_Pose6(void *);	/* $0EA512 */
void Hologram_Fire2_Bolts(void *);	/* $0EA542 */
long Hologram_Fire3_Pose5(void *);	/* $0EA5CE */
void Hologram_Fire3_Bolts(void *);	/* $0EA5FE */
long Hologram_Fire4_Pose5(void *);	/* $0EA68A */
void Hologram_Fire4_Bolts(void *);	/* $0EA6BA */
void Hologram_ActPose0WaitA(void *);	/* $0EA746 */
void Hologram_ActWaitB(void *);	/* $0EA7A4 */
void Hologram_ActWait30(void *);	/* $0EA7F8 */
void Hologram_Reset(void);	/* $0EA84C */
void Hologram_InitSpawnRec(void *, long);	/* $0EA858 */
void Hologram_ClassInit(long);	/* $0EA888 */
void Hologram_Nop66(void *);	/* $0EA898 */
void Hologram_Draw(void *);	/* $0EA8AE */
void Hologram_Init(void *, long, long, void *);	/* $0EAD14 */
void Hologram_VBlankFlicker(void);	/* $0EAE8E asm */
void Hologram_ScriptIntro(void);	/* $0EAEFE fragment */
void Hologram_ScriptDying(void);	/* $0EAF04 fragment */
void Hologram_ScriptAttack(void);	/* $0EAF12 fragment */
void CoreShields_EmptyStub(void);	/* $0EB65A */
long CoreShields_GetSize(void);	/* $0EB662 */
void SentinelsA1_OpenExitTiles(void);	/* $0EB670 */
void CoreShields_StateAllDown(long);	/* $0EB6CE */
long CoreShields_StateActive(void *);	/* $0EB702 */
long CoreShields_StateStart(void *);	/* $0EB796 */
void CoreShields_Reset(void);	/* $0EB7BC */
void CoreShields_ClassInit(long);	/* $0EB7C8 */
void CoreShields_Update(void *);	/* $0EB7D8 */
long CoreShields_Draw(void *);	/* $0EB818 */
void CoreShields_Move(void *);	/* $0EBA56 */
void CoreShields_TakeDamage(void *);	/* $0EBA94 */
void CoreShields_Init(void *, long, long, void *);	/* $0EBB40 */
long CoreZap_GetSize(void);	/* $0EBC6E */
long CoreShields_FireZap(void *, short);	/* $0EBC7C */
void CoreZap_HitActor(void *, void *);	/* $0EBCEE */
void CoreZap_Init(void *, long, long, long);	/* $0EBD78 */
void CoreZap_Update(void *);	/* $0EBDBC */
void CoreElevator_EmptyStub(void);	/* $0EC51E */
long CoreElevator_GetSize(void);	/* $0EC526 */
char CoreElevator_PlayerBelow(void *, void *);	/* $0EC534 */
long CoreElevator_StateIdle(void *);	/* $0EC572 */
long CoreElevator_StartDown(void *);	/* $0EC5B0 */
long CoreElevator_MoveDown(void *);	/* $0EC5E0 */
long CoreElevator_WaitAtBottom(void *);	/* $0EC622 */
long CoreElevator_StartUp(void *);	/* $0EC668 */
long CoreElevator_MoveUp(void *);	/* $0EC69E */
long CoreElevator_WaitAtTop(void *);	/* $0EC6E0 */
long CoreElevator_CarriesAllPlayers(void *);	/* $0EC740 */
void CoreElevator_SendDown(void *);	/* $0EC7B0 */
void CoreElevator_SendUp(void *);	/* $0EC7D6 */
void CoreElevator_Reset(void);	/* $0EC7F8 */
void CoreElevator_InitSpawnRec(void *, long);	/* $0EC804 */
void CoreElevator_ClassInit(void);	/* $0EC834 */
void CoreElevator_Nop66(void *);	/* $0EC840 */
void CoreElevator_Init(void *, long, long, void *);	/* $0EC856 */
void SentinelsB1_InstallSegHooks(void);	/* $0F42FC */
long SentinelsB1_FrameHook(void);	/* $0F4314 */
void SentinelsB1_EmptyStub(void);	/* $0F4350 */
void SentinelsB1_LoadLevel(void);	/* $0F4358 */
void SentinelsB1_InstallHooks(void);	/* $0F43F6 */
void SentinelsB1_RunScene(void);	/* $0F4428 */
long SentinelsB_HazardSeg(short, void *, void *);	/* $0F4440 */
long SentinelsB_ExitSeg(short);	/* $0F44E8 */
char SentinelsB_SegIsMode3(short);	/* $0F4524 */
void SentinelsB2_InstallSegHooks(void);	/* $0F4542 */
void SentinelsB2_ResetAlarm(void);	/* $0F4562 */
void SentinelsB2_AlarmPalette(void);	/* $0F4576 */
long SentinelsB2_FrameHook(void);	/* $0F47AE */
void SentinelsB2_LoadDigits(void);	/* $0F4AEE */
void SentinelsB2_LoadLevel(void);	/* $0F4B2C */
void SentinelsB2_InstallHooks(void);	/* $0F4C72 */
void SentinelsB2_CallStubs(void);	/* $0F4CBC */
void SentinelsB2_EmptyStub(void);	/* $0F4CD0 */
void SentinelsB2_RunScene(void);	/* $0F4CD8 */
void SiberiaB1_ResetPalCycles(void);	/* $0FC71A */
void SiberiaB1_CyclePalettes(void);	/* $0FC72E */
void SiberiaB1_CamApply(void *);	/* $0FC78A */
void SiberiaB1_InitCamera(void *);	/* $0FC890 */
void SiberiaB1_InitWind(void);	/* $0FC8A4 */
void SiberiaB1_StepWind(void);	/* $0FC954 */
void SiberiaB1_InitWindSprite(void);	/* $0FCB8E */
void SiberiaB1_UpdateWindSprite(void);	/* $0FCBDA */
void SiberiaB1_EmptyStub(void);	/* $0FCC62 */
void SiberiaB1_PreLoad(void);	/* $0FCC6A */
void SiberiaB1_PostLoad(void);	/* $0FCC86 */
void SiberiaB1_FrameHook(void);	/* $0FCCAE */
void SiberiaB1_LoadResources(void);	/* $0FCDEE */
long SiberiaB1_DebrisInit(void *);	/* $0FCE32 */
long SiberiaB1_DebrisAlloc(void);	/* $0FCE74 */
void SiberiaB1_DebrisFree(long);	/* $0FCEAA */
void SiberiaB1_DebrisUpdate(void);	/* $0FCEDE */
long SiberiaB1_SpawnDebris(short, short);	/* $0FD0E0 */
long SiberiaB1_IceTileHit(short, short);	/* $0FD24C */
void SiberiaB1_InstallTileHooks(void);	/* $0FD50A */
void SiberiaB1_LoadLevel(void);	/* $0FD576 */
void SiberiaB1_InstallHooks(void);	/* $0FD614 */
void SiberiaB1_CallEmptyStub(void);	/* $0FD69C */
void SiberiaB1_RunScene(void);	/* $0FD6A8 */
void Platform1_EmptyStub(void);	/* $106B60 */
long Platform1_GetSize(void);	/* $106B68 */
void Platform1_Nop66(void *);	/* $106B76 */
void Platform1_ClassInit(void);	/* $106B8C */
void Platform1_LevelStart(void);	/* $106BCE */
long Platform1_Init(void *, long, long, void *);	/* $106BF0 */
void Platform1_Init_SetFrame2(void);	/* $106CC8 fragment */
void Platform1_Init_SetFrame1(void);	/* $106CEC fragment */
void Platform1_Init_Common(void);	/* $106CFE fragment */
void Platform1_Move(void *, short);	/* $106DC0 */
char Platform1_MemberInWindow(void *);	/* $106E14 */
char Platform1_MemberOutsideWindow(void *);	/* $106EA8 */
long Platform1_SteerByPad(void *, void *);	/* $106F42 */
void Platform1_SteerByPad_LeftRight(void);	/* $106F9C fragment */
void Platform1_SteerByPad_UpDown(void);	/* $107092 fragment */
void Platform1_StateRide(void *);	/* $1070E8 */
long Platform1_StateShuttle(void *);	/* $1071AA */
long Platform1_StateSwitched(void *);	/* $1071DE */
long Platform1_StateStart(void *);	/* $107254 */
void SpaceB1_EmptyStub(void);	/* $107370 */
void SpaceB1_ResetPalCycles(void);	/* $107378 */
void SpaceB1_CyclePalettes(void);	/* $1073A8 */
void SpaceB1_CyclePalettes_SetA(void);	/* $107400 fragment */
void SpaceB1_CyclePalettes_NextA(void);	/* $10746C fragment */
void SpaceB1_CyclePalettes_SetB(void);	/* $1074B8 fragment */
void SpaceB1_CyclePalettes_NextB(void);	/* $1074F4 fragment */
void SpaceB1_InstallTileHooks(void);	/* $10750C */
long SpaceB1_Seg28Handler(long, long, long);	/* $107540 */
long SpaceB1_FrameHook(void);	/* $107560 */
void SpaceB1_LoadResources(void);	/* $107592 */
void SpaceB1_LoadLevel(void);	/* $1075B4 */
void SpaceB1_InitCamera(void *);	/* $107658 */
void SpaceB1_InstallHooks(void);	/* $10769A */
void SpaceB1_CallEmptyStub(void);	/* $107780 */
void SpaceB1_RunScene(void);	/* $10778C */
long Switch_GetSize(void);	/* $1077AC */
void Switch_ClassInit(void);	/* $1077BA */
void Switch_InitSpawnRec(void *, long);	/* $107800 */
void Switch_DebounceWait(void *);	/* $107834 */
void Switch_FindAndToggle(void *);	/* $107860 */
long Switch_TileHit(short, short);	/* $1079D8 */
short VDP_SetShadowHighlight(char);	/* $10B552 */
void SpaceA1_EmptyStub1(void);	/* $10B57C */
long Elevator_GetSize(void);	/* $10B584 */
void Elevator_Init(void *, long, short, short);	/* $10B58E */
void Elevator_Update(void *);	/* $10B5D0 */
void Elevator_Toggle(void *);	/* $10B672 */
long SpaceA1_SwitchTileHit(short, short, void *);	/* $10B6BE */
void SpaceA1_InstallTileHooks(void);	/* $10B76C */
void SpaceA1_CamApply(void *);	/* $10B7A0 */
void SpaceA1_CamUpdate(void *);	/* $10B840 */
void SpaceA1_FrameHook(void);	/* $10B98A */
void SpaceA1_EmptyStub2(void);	/* $10B9C8 */
void SpaceA1_LoadLevel(void);	/* $10B9D0 */
void SpaceA1_InitCamera(void *);	/* $10BA48 */
void SpaceA1_InstallHooks(void);	/* $10BA6A */
void SpaceA1_CallEmptyStub(void);	/* $10BB62 */
void SpaceA1_RunScene(void);	/* $10BB6E */
void Deathbird_EmptyStub1(void);	/* $1111E2 */
void Deathbird_MovePolar(void *, void *, void *, void *, void *, void *, long, long, void *, void *, void *, void *, short);	/* $1111EA */
void Deathbird_MovePolar_Case0(void);	/* $1112FE fragment */
void Deathbird_MovePolar_Case3(void);	/* $111336 fragment */
void Deathbird_MovePolar_Case4(void);	/* $111352 fragment */
void Deathbird_MovePolar_Case16(void);	/* $1114FC fragment */
long Deathbird_GetSize(void);	/* $11157A */
void Deathbird_EmptyStub2(void);	/* $111588 */
void Deathbird_Nop66(void *);	/* $111590 */
long Deathbird_LaunchBurst(void *);	/* $1115A6 */
void Deathbird_EmptyStub3(void);	/* $111790 */
void Deathbird_EmptyStub4(void);	/* $111798 */
void Deathbird_SetMoveMode(void *, short);	/* $1117A0 */
void Deathbird_SetMoveMode_Case0(void);	/* $1117DC fragment */
void Deathbird_SetMoveMode_Case1(void);	/* $1117F0 fragment */
void Deathbird_SetMoveMode_Case2(void);	/* $111842 fragment */
void Deathbird_SetMoveMode_Case4(void);	/* $1118D8 fragment */
void Deathbird_SetMoveMode_Case5(void);	/* $111920 fragment */
void Deathbird_SetMoveMode_Case7(void);	/* $111A00 fragment */
void Deathbird_SetMoveMode_Case9(void);	/* $111ABA fragment */
void Deathbird_SetMoveMode_Case10(void);	/* $111B50 fragment */
void Deathbird_SetMoveMode_Default(void);	/* $111B9A fragment */
void Deathbird_UpdateBurst(void *);	/* $111BAA */
void Deathbird_ClassInit(void);	/* $111DEA */
void Deathbird_Init(void *, long, long, long);	/* $111E44 */
long Deathbird_Update(void *);	/* $112016 */
void Deathbird_Move(void *, short);	/* $11214E */
void Deathbird_Move_Polar(void);	/* $112186 fragment */
void Deathbird_Move_Wobble(void);	/* $1121C6 fragment */
void Deathbird_Move_Done(void);	/* $112330 fragment */
long Deathbird_SetFrame(void *, short);	/* $11233A */
void Deathbird_Draw(void *);	/* $1123FA */
char Deathbird_OutsideActiveWindow(void);	/* $112596 */
void Deathbird_Reset(void);	/* $1125A0 */
long Deathbird_StateHurt(void *);	/* $1125B8 */
long Deathbird_StateHurtFlash(void *);	/* $112664 */
long Deathbird_StateFalling(void *);	/* $112754 */
long Deathbird_StateDeadFlicker(void *);	/* $11286E */
long Deathbird_StateStart(void *);	/* $1128AE */
long Deathbird_StateSendHornets(void *);	/* $1128E4 */
long Deathbird_StateChooseAttack(void *);	/* $112A2E */
long Deathbird_StateSwoopDown(void *);	/* $112B2E */
long Deathbird_StateHoverPause(void *);	/* $112B90 */
long Deathbird_StateThrowBurst(void *);	/* $112BDE */
long Deathbird_StateWaitThenLeave(void *);	/* $112C50 */
long Deathbird_StateFlyOff(void *);	/* $112C9E */
long Deathbird_StateDiveLow(void *);	/* $112D1E */
long Deathbird_StateGoToSwitch(void *);	/* $112D9E */
long Deathbird_StateLandOnSwitch(void *);	/* $112EA4 */
long Deathbird_StatePeck(void *);	/* $112F58 */
long Deathbird_StateHitSwitch(void *);	/* $112F8E */
long XNpc_GetSize(void);	/* $113B9A */
void XNpc_Nop66(void *);	/* $113BA8 */
void XNpc_Init(void *, long, long, long);	/* $113BBE */
void XNpc_Explode(void *);	/* $113C14 */
void XNpc_Move(long, short);	/* $113C80 */
void XNpc_Attack(void *, void *, char, char, short, short);	/* $113CA6 */
long XNpc_GetMoveLimit(long, long, void *);	/* $113D0E */
void XNpc_TakeDamage(void *, long, char, char);	/* $113D20 */
void XNpc_TakeDamage_Type4(void);	/* $113D60 fragment */
void XNpc_TakeDamage_Default(void);	/* $113D64 fragment */
void XNpc_TakeDamage_Hurt(void);	/* $113DA0 fragment */
void XNpc_TakeDamage_Done(void);	/* $113E76 fragment */
long XNpc_KnockbackDist(void *, char);	/* $113E80 */
void XNpc_OnHurt(long, char, char);	/* $113EDA */
void XNpc_EndHurt(void *);	/* $113EFE */
void XNpc_OnDeath(long, char, char);	/* $113F38 */
void XNpc_HurtReact(void *, char, char);	/* $113F5C */
void XNpc_HurtReact_Stun(void);	/* $113FFC fragment */
void XNpc_HurtReact_Done(void);	/* $114100 fragment */
void XNpc_DeathReact(void *, char, char);	/* $11410E */
void XNpc_DeathReact_Explode(void);	/* $1141C6 fragment */
void XNpc_DeathReact_Done(void);	/* $1142D2 fragment */
void XNpc_IgnoreHurt(void);	/* $1142E0 */
void XNpc_KillOrExplode(void *, long, char);	/* $1142E8 */
void XNpc_StateExplode(void *);	/* $11432E */
void XNpc_ShakeStep(void *);	/* $11434A */
void XNpc_CalcKnockback(long, short, void *, void *);	/* $11438A */
long XNpc_StateStunStart(void *);	/* $1143BC */
long XNpc_StateStun(void *);	/* $1143EC */
long XNpc_StateKnockStart(void *);	/* $11446C */
long XNpc_StateKnockShake(void *);	/* $11449C */
long XNpc_StateKnockSlide(void *);	/* $114550 */
long XNpc_StateKnockBStart(void *);	/* $1146C6 */
long XNpc_StateKnockBShake(void *);	/* $1146F6 */
long XNpc_StateKnockBSlide(void *);	/* $1147B6 */
long XNpc_StateKnockBExpire(void *);	/* $114860 */
void XNpc_StartDeathFling(void *);	/* $1148B2 */
void XNpc_StateDeathFlingHigh(void *);	/* $114914 */
void XNpc_StateDeathFling(void *);	/* $11494A */
long XNpc_StateDeathFlying(void *);	/* $11497C */
long BioNpc_GetSize(void);	/* $114ABA */
void BioNpc_Nop66(void *);	/* $114AC8 */
void BioNpc_Init(void *, long, long, long);	/* $114ADE */
void BioNpc_DrawStunned(void *);	/* $114B2E */
void BioNpc_StunShake(void *, short);	/* $114B90 */
void BioNpc_UpdateStunned(void *);	/* $114C4C */
void BioNpc_TakeDamage(void *, long, char, char, short, short);	/* $114D0A */
long PowerUp_GetSize(void);	/* $11753C */
void PowerUp_Init(void *, long, long, void *);	/* $11754A */
void PowerUp_Update(void *);	/* $117608 */
void PowerUp_Collect(void);	/* $1177AA */
long SegmentPlusPowerUp_GetSize(void);	/* $1177B8 */
void SegmentPlusPowerUp_Init(void *, long, long, long);	/* $1177C6 */
void SegmentPlusPowerUp_Spawn(long, long, short);	/* $117802 */
void SegmentPlusPowerUp_Collect(long, void *);	/* $117860 */
long SegmentMinusPowerUp_GetSize(void);	/* $11789C */
void SegmentMinusPowerUp_Init(void *, long, long, long);	/* $1178AA */
void SegmentMinusPowerUp_Collect(long, void *);	/* $1178FC */
long XPowerPlusPowerUp_GetSize(void);	/* $117938 */
void XPowerPlusPowerUp_Init(void *, long, long, long);	/* $117946 */
void XPowerPlusPowerUp_Collect(long, void *);	/* $11798A */
long XPowerMinusPowerUp_GetSize(void);	/* $1179C6 */
void XPowerMinusPowerUp_Init(void *, long, long, long);	/* $1179D4 */
void XPowerMinusPowerUp_Collect(long, void *);	/* $117A26 */
long MondoPlusPowerUp_GetSize(void);	/* $117A62 */
void MondoPlusPowerUp_Init(void *, long, long, long);	/* $117A70 */
void MondoPlusPowerUp_Collect(long, void *);	/* $117AB4 */
long MondoMinusPowerUp_GetSize(void);	/* $117AE4 */
void MondoMinusPowerUp_Init(void *, long, long, long);	/* $117AF2 */
void MondoMinusPowerUp_Collect(long, void *);	/* $117B44 */
long MysteryPowerUp_GetSize(void);	/* $117B82 */
void MysteryPowerUp_Init(void *, long, long, long);	/* $117B90 */
void MysteryPowerUp_Collect(long, void *);	/* $117BD4 */
long Explosion_ResetAll(void);	/* $118042 */
long Explosion_SpawnEx(long, long, char, short);	/* $118074 */
char Explosion_Spawn(long, long, char);	/* $118136 */
long Explosion_SpawnSetTimerWord(long, long, char, short);	/* $118166 */
long Explosion_SpawnHurtingSetTimerWord(long, long, char, short, short);	/* $1181AC */
void Explosion_Free(void *);	/* $1181F6 */
void Explosion_HurtActor(void *, short, short);	/* $11821E */
char ObjList_ForEachInBox(short, short, short, short, void *, void *, char);	/* $118254 */
void Explosion_Update(void *);	/* $1182D4 */
long ICandy_GetSize(void);	/* $118426 */
long ICandy_New(long, long, long, long);	/* $118430 */
void ICandy_Init(void *, long, long);	/* $1184AC */
void ICandy_Update(void *);	/* $118550 */
void ICandy_Destroy(void *);	/* $1186A8 */
void ICandy_Draw(void *);	/* $11870A */
void ICandy_Nop22(void);	/* $118750 */
void ICandy_Nop1E(void);	/* $118758 */
void ICandy_PoolInit(long, short, short);	/* $118760 */
long ICandy_PoolAlloc(void);	/* $1187C2 */
void ICandy_PoolFree(void *);	/* $118814 */
void ICandy_SetSpriteDef(void *, char);	/* $118826 */
void ICandy_SetAnim(void *, long);	/* $118874 */
long RThetaObject_GetSize(void);	/* $1188DA */
void RThetaObject_Init(void *, long, long, long);	/* $1188E8 */
void RThetaObject_Move(void *, short);	/* $11893C */
void RThetaObject_Move_Case0(void);	/* $118A74 fragment */
void RThetaObject_Move_Case2(void);	/* $118A88 fragment */
void RThetaObject_Move_Case4(void);	/* $118AB4 fragment */
void RThetaObject_Move_Case16(void);	/* $118C2E fragment */
long GaussBullet_GetSize(void);	/* $118D4C */
void GaussBullet_Nop0A(void *);	/* $118D56 */
void GaussBullet_Update(void *, short);	/* $118D6C */
void GaussBullet_UpdateAll(void);	/* $119086 */
long GaussBullet_InitPool(void);	/* $1190D6 */
long GaussBullet_Fire(long, long, void *, short, short, long);	/* $119190 */
long BBoard_GetSize(void);	/* $11972E */
long BBoard_New(void *, short);	/* $119738 */
long BBoard_CalcSize(long, short);	/* $119784 */
void BBoard_Init(void *, short);	/* $119796 */
long Synchronizer_GetSize(void);	/* $1197E8 */
long Synchronizer_New(void *, long, char);	/* $1197F2 */
long Synchronizer_Init(void *, long, char);	/* $119894 */
void Synchronizer_SetFlag(void *);	/* $119914 */
void Synchronizer_ClearFlag(void *);	/* $11993A */
void Synchronizer_Run(void *);	/* $119960 */
void Synchronizer_Free(void *);	/* $11997A */
void Synchronizer_Destroy(void *);	/* $11998A */
void Synchronizer_DespawnMembers(void *);	/* $1199B8 */
void Synchronizer_ActivateMembers(void *);	/* $119A46 */
void Synchronizer_WaitMembers(void *);	/* $119A8C */
void Synchronizer_DeactivateMembers(void *);	/* $119B20 */
void Synchronizer_RunMembers(void *);	/* $119B66 */
void Synchronizer_GatherMembers(void *);	/* $119BCA */
void Synchronizer_Nop26(void *);	/* $119CEC */
long SyncActor_GetSize(void);	/* $119D36 */
void SyncActor_ClassInit(void);	/* $119D44 */
void SyncActor_Init(void *, long, long, void *);	/* $119D72 */
char SyncActor_InActivateWindow(void *);	/* $119DC4 */
char SyncActor_MemberInWindow(long);	/* $119E20 */
char SyncActor_OutsideActiveWindow(void);	/* $119E3E */
char SyncActor_MemberOutsideWindow(long);	/* $119E48 */
char SyncActor_SpawnRecOutOfView(void);	/* $119E66 */
char SyncActor_MemberSpawnRecOutOfView(long, long);	/* $119E70 */
short SyncActor_IsTriggered(void *);	/* $119E92 */
void SyncActor_Nop66(void *);	/* $119EB4 */
void BabySentinelA_Script(void);	/* $11B4E6 fragment */
void BabySentinelB_Script2(void);	/* $11B508 fragment */
void BabySentinelB_Script2_Part2(void);	/* $11B524 fragment */
void BabySentinelB_Script1(void);	/* $11B53E fragment */
void BabySentinelB_Script1_Part2(void);	/* $11B552 fragment */
void BabySentinelC_Script2(void);	/* $11B560 fragment */
void BabyMoleMan_ScriptDefault(void);	/* $11B5C0 fragment */
void BabyMoleMan_Script1(void);	/* $11B5FC fragment */
void FloatBio_Script0(void);	/* $11B632 fragment */
void FloatBio_Script3(void);	/* $11B65C fragment */
void FloatBio_Script1(void);	/* $11B66E fragment */
void FloatBio_Script1_Part2(void);	/* $11B67E fragment */
void FloatBio_Script2(void);	/* $11B688 fragment */
void WallCrawlBio_Script1(void);	/* $11B6A2 fragment */
void WallCrawlBio_Script2(void);	/* $11B6AC fragment */
long BioFlame_GetSize(void);	/* $11BB30 */
void BioFlame_Nop66(void *);	/* $11BB3E */
void BioFlame_Init(void *, long, long, void *);	/* $11BB54 */
void BioFlame_Update(void *);	/* $11BC88 */
void BioFlame_Draw(void *);	/* $11BE72 */
long BabyBio_GetSize(void);	/* $11C146 */
void BabyBio_Nop66(void *);	/* $11C154 */
void BabyBio_Nop6A(void);	/* $11C16A */
void BabyBio_ClassInit(long);	/* $11C172 */
void BabyBio_LevelStart(void);	/* $11C184 */
void BabyBio_Init(void *, long, long, void *);	/* $11C190 */
void BabyBio_Move(void *, short);	/* $11C3C2 */
void BabyBio_Deactivate(void *);	/* $11C576 */
long BabyBio_TakeDamage(void *, long, char, char, short, short);	/* $11C5BA */
void BabyBio_EndHurt(void *);	/* $11C630 */
long BabyBio_GetMoveLimit(void *, char, void *);	/* $11C662 */
void BabyBio_GetMoveLimit_Horiz(void);	/* $11C694 fragment */
void BabyBio_GetMoveLimit_Vert(void);	/* $11C6BC fragment */
void BabyBio_GetMoveLimit_Done(void);	/* $11C6C4 fragment */
void BabyBio_StateStart(void *);	/* $11C6CE */
void BabyBio_ActNext(void *);	/* $11C71C */
void BabyBio_StateExplode(void *);	/* $11C782 */
long BabyBio_StateFallOff(void *);	/* $11C79E */
long BabyBio_ActFacePlayer(void *);	/* $11C7E6 */
void BabyBio_ActPlayAnimA(void *);	/* $11C88C */
void BabyBio_ActWaitParam(void *);	/* $11C918 */
void BabyBio_ActStopWaitParam(void *);	/* $11C974 */
void BabyBio_ActStopWait60(void *);	/* $11C9DC */
void BabyBio_ActStopWait30(void *);	/* $11CA3C */
void BabyBio_ActPlayAnimB(void *);	/* $11CA9C */
void BabyBio_ActPlayAnimC(void *);	/* $11CAFC */
void BabyBio_ActPlayAnimD(void *);	/* $11CB5C */
long BabyBio_ActWaitPlayerNear192(void *);	/* $11CBBC */
long BabyBio_ActWaitPlayerNear128(void *);	/* $11CC7E */
void BabyBio_WaitReachVLimit(void *);	/* $11CD42 */
long BabyBio_ActMoveVertical(void *);	/* $11CDA8 */
void BabyBio_WaitReachHLimit(void *);	/* $11CE34 */
long BabyBio_ActMoveHorizontal(void *);	/* $11CE9A */
long BabyBio_ActWalkUntilPlayerAhead(void *);	/* $11CFE8 */
long BabyBio_ActStartWalk(void *);	/* $11D0D4 */
void BabyBio_WaitStopOrLimit(void *);	/* $11D184 */
void BabyBio_ActWalkForward(void *);	/* $11D210 */
long BabyBio_ActFireLoop(void *);	/* $11D2C6 */
long BabyBio_ActStartFiring(void *);	/* $11D3F2 */
long BabyBio_ActAttachFlame(void *);	/* $11D422 */
void BabyBio_ActDropFlame(void *);	/* $11D4F0 */
long BabyBio_ActCrawl(void *);	/* $11D55C */
void BabyBio_WaitCrawlLimit(void *);	/* $11D646 */
long BabySentinelA_GetSize(void);	/* $11D6C0 */
void BabySentinelA_Nop66(void *);	/* $11D6CE */
void BabySentinelA_Init(void *, long, long, long);	/* $11D6E4 */
long BabySentinelB_GetSize(void);	/* $11D74A */
void BabySentinelB_Nop66(void *);	/* $11D758 */
void BabySentinelB_Init(void *, long, long, long);	/* $11D76E */
long BabySentinelC_GetSize(void);	/* $11D836 */
void BabySentinelC_Nop66(void *);	/* $11D844 */
void BabySentinelC_Init(void *, long, long, long);	/* $11D85A */
long BabyMoleMan_GetSize(void);	/* $11D922 */
void BabyMoleMan_Nop66(void *);	/* $11D930 */
void BabyMoleMan_Init(void *, long, long, long);	/* $11D946 */
void BabyMoleMan_Init_Case0(void);	/* $11D9BC fragment */
void BabyMoleMan_Init_Case1(void);	/* $11D9D0 fragment */
void BabyMoleMan_Init_Case2(void);	/* $11D9E4 fragment */
void BabyMoleMan_Init_Default(void);	/* $11DA0C fragment */
long FloatBio_GetSize(void);	/* $11DA54 */
void FloatBio_ActWaitParam(void *);	/* $11DA62 */
void FloatBio_Nop66(void *);	/* $11DAB6 */
void FloatBio_Init(void *, long, long, void *);	/* $11DACC */
void FloatBio_Init_Case0(void);	/* $11DB4E fragment */
void FloatBio_Init_Case1(void);	/* $11DB64 fragment */
void FloatBio_Init_Tail(void);	/* $11DBFA fragment */
long WallCrawlBio_GetSize(void);	/* $11DC32 */
void WallCrawlBio_ActStopFrame0(void *);	/* $11DC40 */
void WallCrawlBio_ActStopFrame11(void *);	/* $11DCB6 */
void WallCrawlBio_WaitReachVLimit(void *);	/* $11DD2C */
long WallCrawlBio_ActCrawlVertical(void *);	/* $11DD92 */
void WallCrawlBio_WaitReachHLimit(void *);	/* $11DE38 */
long WallCrawlBio_ActCrawlHorizontal(void *);	/* $11DE9E */
void WallCrawlBio_Nop66(void *);	/* $11DF44 */
long WallCrawlBio_Init(void *, long, long, void *);	/* $11DF5A */
void WallCrawlBio_Move(void *);	/* $11E176 */
long BioBullet_GetSize(void);	/* $11E360 */
void BioBullet_Init(void *, long, long, long);	/* $11E36E */
long BioBullet_HitActor(void *, short, short);	/* $11E3E6 */
char BioBullet_OutsideActiveWindow(void *);	/* $11E420 */
void BioBullet_Update(void *);	/* $11E480 */
void BioBullet_SetVelocity(void *, short, short, char);	/* $11E5E6 */
void BioBullet_Draw(void *);	/* $11E618 */
void BioBalls_ScriptIdle(void);	/* $11E77E fragment */
long BioBalls_GetSize(void);	/* $11E7AA */
void BioBalls_Reset(void);	/* $11E7B8 */
void BioBalls_Init(void *, long, long, long);	/* $11E7C4 */
long BioBalls_Update(void *);	/* $11E8F2 */
void BioBalls_Draw(void *);	/* $11E9CC */
void BioBalls_Move(void *, short);	/* $11ED04 */
void BioBalls_ActCollapse(void *);	/* $11EEA8 */
long BioBalls_StateIdle(void *);	/* $11EF1E */
void BioBalls_ActClearDone(void *);	/* $11EF42 */
void BioBalls_ActSetDone(void *);	/* $11EF96 */
void BioBalls_WaitPathDone(void *);	/* $11EFF2 */
void BioBalls_WaitMotionDone(void *);	/* $11F05C */
void BioBalls_ActClearAnim(void *);	/* $11F0C6 */
long BioBalls_StateAnimA(void *);	/* $11F11C */
long BioBalls_StateAnimB(void *);	/* $11F152 */
char BioBalls_IsDone(void *);	/* $11F188 */
long BioBalls_AllocBallList(short);	/* $11F1A6 */
void BioBalls_UpdateBox(void *);	/* $11F22C */
void BioBalls_SetFormation(void *, void *);	/* $11F2F2 */
void BioBalls_StartPath(void *balls, void *path, char flags, short dx, short dy);	/* $11F3E2 */
void BioBalls_SetPathCollapsed(void *, void *);	/* $11F42C */
void BioBalls_FillFromFirst(void *);	/* $11F4C0 */
void BioBalls_ResetToPos(void *);	/* $11F538 */
char BioBalls_MotionGather(void *, short);	/* $11F588 */
long BioBalls_MotionOrbit(void *, short);	/* $11F67C */
long BioBalls_MotionRing(void *, short);	/* $11FAA2 */
long BioBalls_MotionHelix(void *, short);	/* $11FCB8 */
char BioBalls_MotionPath(void *, short);	/* $11FEE4 */
long BioBalls_SpawnPuff(short, short, char);	/* $1200AE */
long BioBalls_MotionRoll(void *, short);	/* $120124 */
void BioBalls_EmptyStub(void);	/* $1203A4 */
void BigBio_Script(void);	/* $120828 fragment */
long BasicBioSentinel_GetSize(void);	/* $120868 */
void BasicBioSentinel_SetFrame(void *, short);	/* $120876 */
long BioSentinel_GetSize(void);	/* $120936 */
void BioSentinel_Deactivate(void *);	/* $120944 */
long BioSentinel_Init(void *, long, long, void *);	/* $1209D8 */
void BioSentinel_StateStart(void *);	/* $120B4A */
long BioSentinel_StateCeilingCrawl(void *);	/* $120C02 */
long BioSentinel_StateDropFlip(void *);	/* $120D32 */
long BioSentinel_StateFalling(void *);	/* $120DA2 */
long BioSentinel_FireBullet(void *, char);	/* $120E2C */
long BioSentinel_StateFireUp(void *);	/* $120F00 */
long BioSentinel_StateFireDown(void *);	/* $120F64 */
long BioSentinel_StateFireLevel(void *);	/* $120FC8 */
long BioSentinel_StateAimLevel(void *);	/* $12102C */
long BioSentinel_StateAimUp(void *);	/* $121062 */
long BioSentinel_StateAimDown(void *);	/* $121098 */
long BioSentinel_StateAfterSwipe(void *);	/* $1210CE */
long BioSentinel_StateWalking(void *);	/* $12110E */
long BioSentinel_StateThink(void *);	/* $12123E */
long BioSentinel_StateHurt(void *);	/* $121408 */
void BioSentinel_TakeDamage(void *, long, char, char, short, short);	/* $1214B2 */
void BioSentinel_EmptyStub(void);	/* $121566 */
void BioSentinel_ClassInit(void);	/* $12156E */
void BioSentinel_Nop6A(void);	/* $12157A */
void BioSentinel_Nop66(void *);	/* $121582 */
long BigBio_GetSize(void);	/* $121598 */
void BigBio_SpitSpawn(BigBio *self, char kind);	/* $1215A6 */
void BigBio_SpitFree(BigBioSpit *spit);	/* $121634 */
void BigBio_SpitUpdateAll(void);	/* $121662 */
void BigBio_Init(BigBio *self, long x, long y, Placement *place);	/* $1218F0 */
char BigBio_InActivateWindow(BigBio *self);	/* $121992 */
void BigBio_Draw(BigBio *self);	/* $1219FE */
void BigBio_PlaceOrbs(BigBio *self, BigBioShapePt *shape);	/* $121CD6 */
void BigBio_SetOrbShape(BigBio *self, BigBioShapePt *shape);	/* $121D94 */
char BigBio_UpdateOrbRing(BigBio *self);	/* $121DB6 */
void BigBio_ActIdleAnim90(BigBio *self);	/* $122024 */
short BigBio_StateMorphToShape(BigBio *self);	/* $122088 */
short BigBio_StateStartMorph(BigBio *self);	/* $1221D0 */
short BigBio_ActRoarThenMorph(BigBio *self);	/* $122244 */
short BigBio_StateSpitFire(BigBio *self);	/* $1222A0 */
short BigBio_StateSpitBetween(BigBio *self);	/* $12232C */
short BigBio_StateSpitStart(BigBio *self);	/* $122362 */
short BigBio_ActSpit3(BigBio *self);	/* $122398 */
void BigBio_ActFaceSide(BigBio *self);	/* $1223D4 */
void BigBio_ActFaceIdle90(BigBio *self);	/* $122438 */
short BigBio_StateGatherOrbs(BigBio *self);	/* $1224B0 */
short BigBio_ActJumpToSide(BigBio *self);	/* $122660 */
short BigBio_StateFlyToTarget(BigBio *self);	/* $1226C4 */
short BigBio_StateArcSwoop(BigBio *self);	/* $1227B4 */
short BigBio_ActStartArcSwoop(BigBio *self);	/* $12287A */
short BigBio_StateWaitThenFlyBack(BigBio *self);	/* $1228F2 */
short BigBio_StateBrake(BigBio *self);	/* $1229AC */
short BigBio_ActChargePlayer(BigBio *self);	/* $122A14 */
short BigBio_StateSpinUp(BigBio *self);	/* $122AFA */
short BigBio_StateFormRing(BigBio *self);	/* $122B96 */
void BigBio_Launch(BigBio *self, BigBioShapePt *shape);	/* $122DBE */
short BigBio_StateLaunch(BigBio *self);	/* $122E18 */
short BigBio_ActRoarThenLaunch(BigBio *self);	/* $122E92 */
short BigBio_StateStart(BigBio *self);	/* $122EDA */
short BigBio_StateWaitPlayers(BigBio *self);	/* $122F64 */
short BigBio_StateWalkIn(BigBio *self);	/* $12314C */
void BigBio_StartScript(BigBio *self);	/* $1231E0 */
short BigBio_ResumeAfterHit(BigBio *self);	/* $123242 */
short BigBio_StateDying(BigBio *self);	/* $123272 */
short BigBio_StateDeathOrbs(BigBio *self);	/* $1232F0 */
short BigBio_StateDefeated(BigBio *self);	/* $123346 */
void BigBio_TakeDamage(BigBio *self, Actor *attacker, char type, char damage, short x, short y);	/* $12340E */
void BigBio_EmptyStub(void);	/* $1234F4 */
void BigBio_ClassInit(void);	/* $1234FC */
long BrainRocket_GetSize(void);	/* $133630 */
void BrainRocket_ClassInit(void);	/* $13363E */
void BrainRocket_Init(void *, long, long, long);	/* $13364A */
long BrainRocket_Update(void *);	/* $1336DC */
void BrainRocket_Move(void *, short);	/* $1337BA */
char BrainRocket_OutsideActiveWindow(void *);	/* $1337FC */
void BrainRocket_Deactivate(long);	/* $133898 */
void BrainRocket_Attack(void *, void *, char, char, short, short);	/* $1338A8 */
long Rocket_SetAngleOwner(void *, void *, short);	/* $13395E */
void Rocket_SetAngleA(void *rocket, void *owner, short angle);	/* $1339A6 */
void Rocket_SetAngleB(long, long, short, short, short);	/* $1339C8 */
long BrainRocket_StateLaunch(void *);	/* $1339F4 */
long BrainRocket_StateSetDirection(void *);	/* $133A40 */
long BrainRocket_StateFly(void *);	/* $133B42 */
long DropRocket_GetSize(void);	/* $133B66 */
void DropRocket_Init(void *, long, long, long);	/* $133B74 */
long DropRocket_StateLaunch(void *);	/* $133BB2 */
long DropRocket_StateDrop(void *);	/* $133C1A */
long DropRocket_StateFly(void *);	/* $133CD0 */
void BrainRocket_EmptyStub(void);	/* $133CF4 */
void BigAcBullet_Attack(void *, void *, long, char, short, short);	/* $133E4A */
long FireWall_GetSize(void);	/* $133F3C */
void FireWall_Reset(void);	/* $133F4A */
void FireWall_ClassInit(void);	/* $133F56 */
void FireWall_Init(void *, long, long, void *);	/* $133F62 */
long FireWall_StateWaitFlame(void *);	/* $134080 */
long FireWall_StateWaitOnScreen(void *);	/* $1340D0 */
long FireWall_StateSpawnFlame(void *);	/* $13412A */
void FireWall_EmptyStub(void);	/* $13420C */
void FlyBrood_ScrStepHoverForever(void);	/* $1344DA fragment */
void FlyBrood_ScrStepsWaitPassed(void);	/* $1344E0 fragment */
void FlyBrood_ScrStepsWaitPassedLow(void);	/* $1344EA fragment */
void FlyBrood_ScrStepsWaitPassedSine(void);	/* $1344F4 fragment */
long FlyBrood_GetSize(void);	/* $134590 */
void FlyBrood_EmptyStub(void);	/* $13459E */
void FlyBrood_EnterFromSide(void *, char);	/* $1345A6 */
long FlyBrood_MoveHoming(void *, short);	/* $13463C */
void FlyBrood_MovePath(void *, short);	/* $13478C */
void FlyBrood_MoveBallistic(void *, short);	/* $1347F8 */
void FlyBrood_MoveSine(void *, short);	/* $13486A */
void FlyBrood_Nop6A(void);	/* $1348E8 */
void FlyBrood_Nop66(void *);	/* $1348F0 */
void FlyBrood_ClassInit(void);	/* $134906 */
void FlyBrood_Init(void *, long, long, void *);	/* $134934 */
void FlyBrood_Init_Case0(void);	/* $134AE4 fragment */
void FlyBrood_Init_Case1(void);	/* $134AFA fragment */
void FlyBrood_Init_Case4(void);	/* $134B42 fragment */
void FlyBrood_Init_Tail(void);	/* $134BE6 fragment */
long FlyBrood_Deactivate(long);	/* $134C3A */
void FlyBrood_Draw(void *);	/* $134C60 */
long FlyBrood_Update(void *);	/* $134D64 */
void FlyBrood_ActRestartScript(void *);	/* $134E2A */
void FlyBrood_WaitPathDone(void *);	/* $134E6A */
long FlyBrood_ActHover90(void *);	/* $134EC4 */
void FlyBrood_StateHoverFacePlayer(void *);	/* $134F20 */
long FlyBrood_ActHoverUntilNear(void *);	/* $134F9C */
void FlyBrood_StateWaitPlayerInRange(void *);	/* $134FF2 */
void FlyBrood_StateFacePlayer(void *);	/* $1350FE */
long FlyBrood_ActHoverForever(void *);	/* $135134 */
void FlyBrood_StateFlyOffKill(void *);	/* $13518A */
long FlyBrood_ActDiveAtPlayer(void *);	/* $1351EA */
long FlyBrood_StateWaitPlayerPassed(void *);	/* $1352D4 */
long FlyBrood_ActDiveAtPlayerLow(void *);	/* $13536A */
long FlyBrood_StateWaitPlayerPassedLow(void *);	/* $135452 */
long FlyBrood_ActSineFlyBy(void *);	/* $1354E8 */
long FlyBrood_StateWaitPlayerPassedSine(void *);	/* $13557A */
long FlyBrood_StateSwoopPatrol(void *);	/* $135610 */
long FlyBrood_WaitSwoopDone(void *);	/* $13570A */
void FlyBrood_StateHit(long);	/* $135794 */
long Hornet_GetSize(void);	/* $1364B4 */
void Hornet_NopStatic(void);	/* $1364C2 */
void Hornet_MoveSwing(void *);	/* $1364CA */
void Hornet_MoveStartSwing(void *);	/* $136584 */
void Hornet_StepCirclePath(void *);	/* $136598 */
void Hornet_MoveVelocity(void *);	/* $136642 */
void Hornet_MoveBounce(void *);	/* $1366A2 */
void Hornet_MoveFlyOff(void *);	/* $13679A */
void Hornet_MoveWaitThenFlyOff(void *);	/* $136816 */
void Hornet_MoveRiseThenLeave(void *);	/* $136880 */
void Hornet_MoveWaitThenRiseLeave(void *);	/* $1368F4 */
void Hornet_MoveDiveOnce(void *);	/* $13695C */
void Hornet_MoveDiveAgain(void *);	/* $136992 */
void Hornet_MoveWaitThenDive(void *);	/* $1369C8 */
void Hornet_MoveRiseThenWait(void *);	/* $136A32 */
void Hornet_MoveWaitThenRise(void *);	/* $136AA6 */
void Hornet_MoveDiveRepeat(void *);	/* $136B0E */
long Hornet_StateAttackDelay(void *);	/* $136B44 */
long Hornet_StateFacePlayer(void *);	/* $136B84 */
long Hornet_StateShotLoop(void *);	/* $136BD4 */
long Hornet_StateFire(void *);	/* $136C28 */
long Hornet_StateShotRecover(void *);	/* $136CCA */
long Hornet_StateDropIn(void *);	/* $136D1A */
void Hornet_StateDropping(void *);	/* $136D44 */
long Hornet_StatePickPattern(void *);	/* $136D86 */
void Hornet_StatePickPattern_CaseDefault(void);	/* $136DAE fragment */
void Hornet_StatePickPattern_Case1(void);	/* $136DC4 fragment */
void Hornet_StatePickPattern_StartAttack(void);	/* $136E04 fragment */
void Hornet_Nop66(void *);	/* $136E3A */
void Hornet_Reset(void);	/* $136E50 */
void Hornet_ClassInit(void);	/* $136E5C */
void Hornet_Init(void *, long, long, void *);	/* $136E7C */
long Hornet_Move(void *);	/* $13707A */
long Hornet_Update(void *);	/* $13709E */
void Hornet_Explode(void *);	/* $1370E8 */
long Hornet_SpawnForDeathbird(long, long, long, short);	/* $137142 */
void Hornet_SpawnForDeathbird_CaseDefault(void);	/* $1371C8 fragment */
void Hornet_SpawnForDeathbird_Case2(void);	/* $1371D8 fragment */
void Hornet_SpawnForDeathbird_Finish(void);	/* $13722C fragment */
long HornetBullet_GetSize(void);	/* $137340 */
void HornetBullet_NopStatic(void);	/* $13734E */
void HornetBullet_Reset(void);	/* $137356 */
void HornetBullet_ClassInit(void);	/* $137362 */
long HornetBullet_Init(void *, long, long, long);	/* $137382 */
void HornetBullet_Update(void *);	/* $13745E */
void HornetBullet_Draw(void *);	/* $1375DA */
long LaserCannon_GetSize(void);	/* $13814A */
void LaserCannon_Nop66(void *);	/* $138158 */
void LaserCannon_Nop6A(void);	/* $13816E */
void LaserCannon_ClassInit(void);	/* $138176 */
void LaserCannon_Init(void *, long, long, void *);	/* $13817E */
void LaserCannon_OnDeath(void *);	/* $138284 */
long LaserCannon_StateStart(void *);	/* $1382BA */
long LaserCannon_StateIdle(void *);	/* $1382E4 */
long LaserCannon_StatePickTarget(void *);	/* $138318 */
long LaserCannon_StateWatchTarget(void *);	/* $138358 */
long LaserCannon_StateExtend(void *);	/* $13846E */
long LaserCannon_StateReload(void *);	/* $1384A4 */
long LaserCannon_StateRetract(void *);	/* $1384F4 */
long LaserCannon_StateLoadBurst(void *);	/* $13852A */
long LaserCannon_StateWaitExtended(void *);	/* $138556 */
long LaserCannon_StateAim(void *);	/* $138582 */
long LaserCannon_StateFire(void *);	/* $1385B8 */
void LineSegKiller_RemoveSegsInBox(short, short, short, short, short);	/* $138766 */
long LineSegKiller_GetSize(void);	/* $13884A */
void LineSegKiller_ClassInit(void);	/* $138858 */
void LineSegKiller_Init(void *, long, long, void *);	/* $138864 */
void LineSegKiller_Update(void);	/* $1388F8 */
void LineSegKiller_Draw(void);	/* $138900 */
void LineSegKiller_Deactivate(void *);	/* $138908 */
void LineSegKiller_Kill(void *);	/* $13891A */
void LineSegKiller_KillAll(void);	/* $138968 */
void LineSegKiller_NopStatic(void);	/* $1389C2 */
long MaintRobotFire_GetSize(void);	/* $138A46 */
void MaintRobotFire_Init(void *, long, long, long);	/* $138A54 */
void MaintRobotFire_Update(void *);	/* $138AD4 */
long MaintRobot_GetSize(void);	/* $138B42 */
void MaintRobot_ClassInit(void);	/* $138B50 */
void MaintRobot_Init(void *, long, long, void *);	/* $138B5C */
long MaintRobot_StateFire(void *);	/* $138CD6 */
char MaintRobot_TargetInRange(void *, void *);	/* $138DA8 */
long MaintRobot_StateStartVolley(void *);	/* $138EA2 */
long MaintRobot_StateStop(void *);	/* $138EDC */
long MaintRobot_StateChase(void *);	/* $138F1E */
void MaintRobot_StateChase_Case0(void);	/* $138FBC fragment */
void MaintRobot_StateChase_Case1(void);	/* $139038 fragment */
void MaintRobot_StateChase_Case3(void);	/* $13906A fragment */
void MaintRobot_StateChase_End(void);	/* $1390B0 fragment */
long MaintRobot_StatePatrol(void *);	/* $1390BA */
long MaintRobot_StateResume(void *);	/* $139108 */
void MaintRobot_Nop66(void *);	/* $139162 */
void MaintRobot_NopStatic(void);	/* $139178 */
long Mine_GetSize(void);	/* $13A736 */
void Mine_Nop66(void *);	/* $13A744 */
void Mine_Nop6A(void);	/* $13A75A */
void Mine_ClassInit(void);	/* $13A762 */
void Mine_Init(void *, long, long, long);	/* $13A784 */
void Mine_InitPool(void);	/* $13A7EA */
long Mine_PoolRemove(long);	/* $13A882 */
void Mine_PoolUpdate(void);	/* $13A8C4 */
void Mine_PoolDraw(void);	/* $13AA02 */
long Mine_PoolAdd(void *);	/* $13AB12 */
long Mine_SpawnFromRec(long, void *);	/* $13AB92 */
void Mine_StateStart(void *);	/* $13ABCE */
long Mine_StatePickTarget(void *);	/* $13AC06 */
void Mine_StateWatch(void *);	/* $13AC40 */
void Mine_StateArm1(void *);	/* $13ACBE */
void Mine_StateArm2(void *);	/* $13ACF8 */
void Mine_StateArm3(void *);	/* $13AD32 */
void Mine_StateArm4(void *);	/* $13AD6C */
long Mine_Explode(void *);	/* $13ADA6 */
long Ninja_GetSize(void);	/* $13CC14 */
void Ninja_Reset(void);	/* $13CC22 */
void Ninja_ClassInit(void);	/* $13CC2E */
long Ninja_CalcPatrolBounds(void *, short, void *, void *);	/* $13CC3A */
void Ninja_Init(void *, long, long, void *);	/* $13CC90 */
long Ninja_FindSegInBox(short, short, short, long, short, short, short, short);	/* $13CE68 */
long Ninja_ClipToSegs(void *, short, short);	/* $13CF6A */
void Ninja_Move(void *, short);	/* $13D1CE */
void Ninja_Deactivate(void *);	/* $13D260 */
void Ninja_StateNextStep(void *);	/* $13D2A0 */
void Ninja_ActStartCombat(void *);	/* $13D2F4 */
long Ninja_ActLeap(void *);	/* $13D344 */
long Ninja_StateLeapAir(void *);	/* $13D424 */
long Ninja_StateWaitLanding(void *);	/* $13D46A */
void Ninja_ActAttack(void *);	/* $13D4FE */
void Ninja_ActCheckLeave(void *);	/* $13D574 */
void Ninja_ActFacePlayer(void *);	/* $13D60C */
void Ninja_ActIdleAnim(void *);	/* $13D6B6 */
void Ninja_ActWait(void *);	/* $13D716 */
void Ninja_ActPause30(void *);	/* $13D786 */
void Ninja_NopStatic(void);	/* $13D7F6 */
void Ninja_ScrStepStartCombat(void);	/* $13D8C2 fragment */
void Ninja_ScrStepFacePlayer(void);	/* $13D8C8 fragment */
void Ninja_ScrStepFacePlayer2(void);	/* $13D8D0 fragment */
void ExplodeFx_Free(void *);	/* $13D8F6 */
void ExplodeFx_BurstStart(void *, char, long, char, char);	/* $13D954 */
void ExplodeFx_BurstPuffAim(void *, long, long);	/* $13DA0C */
void ExplodeFx_BurstPuffInit(void *, long, long);	/* $13DA50 */
void ExplodeFx_BurstUpdate(void *);	/* $13DA80 */
void ExplodeFx_BurstDraw(void *);	/* $13DBD8 */
long ExplodeFx_Burst(void *, short);	/* $13DCB8 */
void ExplodeFx_BurstMedium(void *);	/* $13DE36 */
void ExplodeFx_BurstSmall(void *);	/* $13DE4A */
void ExplodeFx_TrailStart(void *, char, long, char, char);	/* $13DE5E */
void ExplodeFx_TrailSetHead(void *, long, long, short, short, short, short);	/* $13DF16 */
void ExplodeFx_TrailPuffInit(void *, long, long);	/* $13DF44 */
void ExplodeFx_TrailUpdate(void *);	/* $13DF74 */
void ExplodeFx_TrailDraw(void *);	/* $13E14A */
long ExplodeFx_Trail(void *, short, short, short, short);	/* $13E23E */
void ExplodeFx_TrailArc(void *);	/* $13E30E */
void ExplodeFx_BounceStart(void *, char, long, char, char);	/* $13E36E */
long ExplodeFx_BounceGroupAim(void *, long, long);	/* $13E426 */
long ExplodeFx_BounceGroupInit(void *, long, long);	/* $13E490 */
void ExplodeFx_BounceUpdate(void *);	/* $13E4EA */
void ExplodeFx_BounceDraw(void *);	/* $13E6EE */
long ExplodeFx_Bounce(void *);	/* $13E808 */
void ExplodeFx_ChainStart(void *, char, long, char, char, short, short, char, long, short);	/* $13E928 */
void ExplodeFx_ChainStep(void *);	/* $13EA46 */
void ExplodeFx_ChainHit(void *, short, short);	/* $13EBB6 */
void ExplodeFx_ChainUpdate(void *);	/* $13EC26 */
void ExplodeFx_ChainDraw(void *);	/* $13EE20 */
void ExplodeFx_Chain(void *obj, void *path, char damaging, short dx, short dy);	/* $13EEFA */
void ExplodeFx_ChainRepeat(void *obj, void *path, char damaging, short dx, short dy, short repeats);	/* $13EFA0 */
long Pop_GetSize(void);	/* $13F04A */
void Pop_ClassInit(void);	/* $13F058 */
void Pop_Reset(void);	/* $13F064 */
void Pop_Init(void *, long, long, void *);	/* $13F070 */
long Pop_StateStart(void *);	/* $13F0F0 */
long Pop_StateWait(void *);	/* $13F114 */
void Pop_StateWait_Case5(void);	/* $13F14A fragment */
void Pop_StateWait_CaseDefault(void);	/* $13F1AE fragment */
void Pop_FxBurstMedium(long);	/* $13F1DC */
void Pop_FxBurstSmall(long);	/* $13F1EC */
void Pop_FxBounce(long);	/* $13F1FC */
void Pop_FxTrailArc(long);	/* $13F20C */
void Pop_FxChainPath(void *);	/* $13F21C */
void Pop_FxChainSpiral(void *);	/* $13F24C */
void Pop_NopStatic(void);	/* $13F282 */
void Debris_SetFloorY(void *, short);	/* $13F474 */
void Debris_SetSprDef(void *, short);	/* $13F486 */
void Debris_SetBounceLimit(void *, short);	/* $13F498 */
DebrisPool *Debris_CreatePool(short n, void *def, char frame);	/* $13F4D4 */
void Debris_Spawn(short, short, void *, short);	/* $13F58A */
void Debris_Update(void *);	/* $13F728 */
long Spark_GetSize(void);	/* $13FA88 */
void Spark_ClassInit(void);	/* $13FA96 */
void Spark_Init(void *, long, long, void *);	/* $13FAA2 */
long Spark_Update(void *);	/* $13FBA4 */
void Spark_Draw(void *);	/* $13FCC2 */
long Spark_Move(void *, short);	/* $13FDA2 */
void Spark_Attack(void *, void *, char, char, short, short);	/* $13FE36 */
long Spark_StateStart(void *);	/* $13FEB6 */
long Spark_StateStop(void *);	/* $13FF2C */
long Spark_StateDart(void *);	/* $13FF5E */
long Spark_StateFlyStraight(void *);	/* $14004A */
void Spark_StateHit(void *);	/* $1400AA */
void Spark_NopStatic(void);	/* $1400D6 */
long Tank_GetSize(void);	/* $1416C0 */
void Tank_ClassInit(void);	/* $1416CE */
void Tank_Init(void *, long, long, void *);	/* $1416EA */
void Tank_SetTrackLimits(void *, short, void *, void *);	/* $1418A4 */
void Tank_Move(void *, short);	/* $1418FA */
void Tank_Deactivate(void *);	/* $14199C */
long Tank_ActFireRocket(void *);	/* $1419E0 */
void Tank_ActPause(void *);	/* $141ADC */
long Tank_StateWreck(void *);	/* $141B3C */
long Tank_OnDeath(void *);	/* $141B72 */
long TankLeft_GetSize(void);	/* $141CBC */
void TankLeft_Init(void *, long, long, long);	/* $141CCA */
void TankLeft_Move(void *, short);	/* $141D46 */
void TankLeft_Deactivate(void *);	/* $141DA8 */
long TankLeft_StateWaitDrive(void *);	/* $141DE8 */
long TankLeft_ActWaitDrive(void *);	/* $141EEE */
long TankLeft_StateDriveIn(void *);	/* $141F18 */
long TankLeft_ActDriveIn(void *);	/* $142008 */
void TankLeft_ActDrive(void *);	/* $14205E */
void Tank_ScriptWaitDrive(void);	/* $14221E fragment */
void Tank_ScriptDriveIn(void);	/* $14222C fragment */
void Tank_ScriptDrive(void);	/* $14223A fragment */
long TechWallGun_GetSize(void);	/* $14224E */
long TechWallGun_FindFreeBullet(void);	/* $14225C */
void TechWallGun_NopStatic(void);	/* $1422A0 */
void TechWallGun_Explode(void *);	/* $1422A8 */
long TechWallGun_StateHurt(void *);	/* $142322 */
void TechWallGun_StateBurst(void *);	/* $1423D6 */
void TechWallGun_SetupBurst(void *, long, long, short);	/* $14241C */
long TechWallGun_BurstDiagUp(void *);	/* $142446 */
long TechWallGun_BurstLevel(void *);	/* $142490 */
long TechWallGun_BurstDiagDown(void *);	/* $1424DA */
long TechWallGun_BurstCeilLeft(void *);	/* $142524 */
long TechWallGun_BurstCeilDown(void *);	/* $142562 */
long TechWallGun_BurstCeilRight(void *);	/* $1425A0 */
long TechWallGun_AimAngle(short, short);	/* $1425DE */
long TechWallGun_AimWall(void *, void *);	/* $1426FE */
long TechWallGun_AimCeiling(void *, void *);	/* $14278E */
long TechWallGun_StateAimUp(void *);	/* $14280A */
long TechWallGun_StateAimLevel(void *);	/* $14286A */
long TechWallGun_StateAimDown(void *);	/* $1428E6 */
long TechWallGun_StateCeilAimLeft(void *);	/* $142946 */
long TechWallGun_StateCeilAimDown(void *);	/* $1429A6 */
long TechWallGun_StateCeilAimRight(void *);	/* $142A22 */
long TechWallGun_StartWall(void *);	/* $142A82 */
long TechWallGun_StartCeiling(void *);	/* $142AA6 */
void TechWallGun_Nop66(void *);	/* $142ACA */
void TechWallGun_ClassInit(void);	/* $142AE0 */
void TechWallGun_Init(void *, long, long, void *);	/* $142B54 */
long TechWallGun_UpdateBullet(void *);	/* $142CD6 */
void TechWallGun_UpdateBullet_Case1(void);	/* $142F0C fragment */
void TechWallGun_UpdateBullet_Case4(void);	/* $142F2A fragment */
void TechWallGun_UpdateBullet_Case5(void);	/* $142F38 fragment */
void TechWallGun_UpdateBullet_Case8(void);	/* $142F5C fragment */
void TechWallGun_UpdateBullet_Case9(void);	/* $142F64 fragment */
void TechWallGun_UpdateBullet_Draw(void);	/* $142F70 fragment */
void TechWallGun_UpdateBullets(void *);	/* $14304E */
long TechWallGun_Fire(void *, short);	/* $143082 */
void TechWallGun_Fire_Case1(void);	/* $1430E4 fragment */
void TechWallGun_Fire_Case4(void);	/* $143136 fragment */
void TechWallGun_Fire_Case5(void);	/* $143152 fragment */
void TechWallGun_Fire_Case8(void);	/* $14319E fragment */
void TechWallGun_Fire_Case9(void);	/* $1431B6 fragment */
void TechWallGun_Fire_Default(void);	/* $1431D0 fragment */
void TechWallGun_Fire_Spawn(void);	/* $1431D6 fragment */
void TechWallGun_Fire_Ret(void);	/* $14321A fragment */
void ToxicCan_DrawLeak(short, short, void *, void *);	/* $144774 */
long ToxicCan_GetSize(void);	/* $1449CC */
void ToxicCan_ClassInit(void);	/* $1449DA */
void ToxicCan_Init(void *, long, long, void *);	/* $1449E6 */
void ToxicCan_Update(void *);	/* $144AE2 */
void ToxicCan_Activate(void *);	/* $144C76 */
void ToxicCan_Deactivate(void *);	/* $144CAA */
long ToxicCanDamaged_GetSize(void);	/* $144CE2 */
void ToxicCan_Despawn(void *, void *);	/* $144CF0 */
void ToxicCanDamaged_Init(void *, long, long, long);	/* $144D50 */
void ToxicCan_NopStatic(void);	/* $144D88 */
long FluxBomb_GetSize(void);	/* $157BA6 */
long MagneticGlob_GetSize(void);	/* $157BB4 */
void MagnetoShots_Reset(void);	/* $157BC2 */
void MagneticGlob_SetFrame(void *, short);	/* $157BD2 */
void MagneticGlob_Draw(void *);	/* $157C22 */
long MagneticGlob_New(long, long, long, long, long);	/* $157C5C */
long MagneticGlob_HitCb(void *, short, short, void *);	/* $157CB2 */
long MagneticGlob_OutsideActiveWindow(void *);	/* $157CFE */
void MagneticGlob_Activate(void *);	/* $157D5E */
void MagneticGlob_Deactivate(void *);	/* $157E64 */
void MagneticGlob_Update(void *);	/* $157ED6 */
void MagneticGlob_SetAnimForAngle(void *);	/* $1580B2 */
void MagneticGlob_Init(void *, long, long, void *);	/* $1581CA */
long FluxBomb_New(long, long, long, long, long);	/* $158260 */
void FluxBomb_Init(void *, long, long, void *);	/* $1582B6 */
void FluxBomb_StepBurst(void *);	/* $1583D6 */
long FluxBomb_StepImplode(void *);	/* $158474 */
long FluxBomb_StartImplode(void *);	/* $158536 */
void FluxBomb_HitTilesInArea(void *, short, short, short, short);	/* $158610 */
void FluxBomb_DamageArea(void *);	/* $158876 */
long FluxBomb_StartBurst(void *);	/* $158992 */
void FluxBomb_Update(void *);	/* $158A60 */
void FluxBomb_SetFrame(long, short);	/* $158AC0 */
long FluxBomb_DrawImplode(void *);	/* $158AF8 */
long FluxBomb_DrawBurst(void *);	/* $158C0E */
void FluxBomb_Draw(void *);	/* $158DD4 */
long FluxBomb_StartThrown(void *);	/* $158E32 */
long FluxBomb_StateThrown(void *);	/* $158E78 */
long FluxBomb_StateImplode(void *);	/* $158EEE */
long FluxBomb_Detonate(void *);	/* $158F32 */
void FluxBomb_StateBurst(void *);	/* $158F68 */
long FluxBomb_Explode(void *);	/* $158F92 */
long FluxBomb_StartHeld(void *);	/* $158FE4 */
long FluxBomb_StateHeld(void *);	/* $159030 */
long Magneto_AirShotAnim(short);	/* $159092 */
void Magneto_AirShotAnim_Case0(void);	/* $1590C6 fragment */
void Magneto_AirShotAnim_Case2(void);	/* $1590DA fragment */
void Magneto_AirShotAnim_Case3(void);	/* $1590E4 fragment */
void Magneto_AirShotAnim_Case7(void);	/* $15910C fragment */
void Magneto_AirShotAnim_Case9(void);	/* $159120 fragment */
void Magneto_AirShotAnim_Default(void);	/* $15912A fragment */
void Magneto_AirShotAnim_Result(void);	/* $15912E fragment */
void Magneto_AirShotAnim_Ret(void);	/* $159130 fragment */
long Magneto_AirShotEndAnim(short);	/* $159138 */
void Magneto_AirShotEndAnim_Case0(void);	/* $15916C fragment */
void Magneto_AirShotEndAnim_Case2(void);	/* $159180 fragment */
void Magneto_AirShotEndAnim_Case3(void);	/* $15918A fragment */
void Magneto_AirShotEndAnim_Case7(void);	/* $1591B2 fragment */
void Magneto_AirShotEndAnim_Case9(void);	/* $1591C6 fragment */
void Magneto_AirShotEndAnim_Default(void);	/* $1591D0 fragment */
void Magneto_AirShotEndAnim_Result(void);	/* $1591D4 fragment */
void Magneto_AirShotEndAnim_Ret(void);	/* $1591D6 fragment */
long Magneto_ShotAnim(short);	/* $1591DE */
void Magneto_ShotAnim_Case1(void);	/* $15920C fragment */
void Magneto_ShotAnim_Case2(void);	/* $159216 fragment */
void Magneto_ShotAnim_Case4(void);	/* $15922A fragment */
void Magneto_ShotAnim_Case7(void);	/* $159248 fragment */
void Magneto_ShotAnim_Case8(void);	/* $159252 fragment */
void Magneto_ShotAnim_Default(void);	/* $15925C fragment */
void Magneto_ShotAnim_Result(void);	/* $159260 fragment */
void Magneto_ShotAnim_Ret(void);	/* $159262 fragment */
long Magneto_ShotEndAnim(short);	/* $15926A */
void Magneto_ShotEndAnim_Case1(void);	/* $159298 fragment */
void Magneto_ShotEndAnim_Case2(void);	/* $1592A2 fragment */
void Magneto_ShotEndAnim_Case4(void);	/* $1592B6 fragment */
void Magneto_ShotEndAnim_Case7(void);	/* $1592D4 fragment */
void Magneto_ShotEndAnim_Case8(void);	/* $1592DE fragment */
void Magneto_ShotEndAnim_Default(void);	/* $1592E8 fragment */
void Magneto_ShotEndAnim_Result(void);	/* $1592EC fragment */
void Magneto_ShotEndAnim_Ret(void);	/* $1592EE fragment */
long Magneto_ShotOriginCrouch(char, void *, void *, void *, void *);	/* $1592F6 */
void Magneto_ShotOriginAir(char, void *, void *);	/* $159354 */
void Magneto_ShotOriginAir_Case0(void);	/* $159398 fragment */
void Magneto_ShotOriginAir_Case9(void);	/* $1593B6 fragment */
void Magneto_ShotOriginAir_Case1(void);	/* $1593D4 fragment */
void Magneto_ShotOriginAir_Case2(void);	/* $1593F2 fragment */
void Magneto_ShotOriginAir_Case3(void);	/* $159410 fragment */
void Magneto_ShotOriginAir_Case7(void);	/* $159482 fragment */
void Magneto_ShotOriginAir_Ret(void);	/* $1594B6 fragment */
void Magneto_ShotOrigin(char, void *, void *);	/* $1594C0 */
void Magneto_ShotOrigin_Case1(void);	/* $159502 fragment */
void Magneto_ShotOrigin_Case7(void);	/* $1595B0 fragment */
void Magneto_ShotOrigin_Case8(void);	/* $1595CA fragment */
void Magneto_ShotOrigin_Ret(void);	/* $1595E4 fragment */
long Magneto_SpawnShot(long, long, char, char, char, char, char, short);	/* $1595EE */
long Magneto_BombOriginCrouch(char, void *, void *, void *);	/* $1597BE */
void Magneto_BombOrigin(char, void *, void *);	/* $15980C */
void Magneto_BombOrigin_Case1(void);	/* $15984E fragment */
void Magneto_BombOrigin_Case2(void);	/* $159864 fragment */
void Magneto_BombOrigin_Case4(void);	/* $15988C fragment */
void Magneto_BombOrigin_Case7(void);	/* $1598C8 fragment */
void Magneto_BombOrigin_Case8(void);	/* $1598DA fragment */
void Magneto_BombOrigin_Ret(void);	/* $1598EC fragment */
long Magneto_SpawnFluxBomb(long, long, char, char, char, char);	/* $1598F6 */
long Magneto_FrameBank(short);	/* $1599CC */
long Magneto_FrameInBank(short);	/* $159A06 */
void Magneto_SetFrame(void *, short);	/* $159A46 */
void Cam_ClampLookToPlayers(void *);	/* $159ECC */
void Cam_CommitLook(void *);	/* $159FB8 */
void Pal_CycleStep(void *, void *, void *, void *, char);	/* $159FD6 */
long Seg_MoveAlong(void *, short);	/* $15A0AC */
void Seg_MoveAlongSteps(void *, short);	/* $15A168 */
long Obj_RideRail(void *, short);	/* $15A19C */
void AiScript_Set(AiCursor *cursor, void *table);	/* $15A6DE */
long Obj_InScreenWindow(void *);	/* $15A6FC */
long Obj_OutsideActivateWindow(void *);	/* $15A79C */
short Seg_GridCell(long, long);	/* $15A7C8 */
long Seg_FindAt(short, short, short, short);	/* $15A82A */
Seg *Seg_FindBelow(short, short, short);	/* $15A8FE */
Seg *Seg_FindAbove(short, short, short);	/* $15AA20 */
Seg *Seg_FindLeft(short x, short y, short type);	/* $15AB44 */
Seg *Seg_FindRight(short x, short y, short type);	/* $15AC68 */
long SegHook_HurtPlayerStop(short, long, void *, void *);	/* $15AD8A */
long SegHook_HurtPlayer(short, long, void *, void *);	/* $15ADEC */
void Spr_EmitFlicker(void *, void *, void *, char, char, char, char, char);	/* $15AE48 */
void Timer_SetVBlankHookCb(long, long);	/* $15B0D0 */
void Spr_ReloadDefs(void (*load)(void), char repack);	/* $15B0E6 */
void Spr_SetFromDef(void *, char);	/* $15B18A */
long Math_LMul(void);	/* $15C1FC asm */
long Math_LDivS(void);	/* $15C222 asm */
long Math_LDivS_Sel(void);	/* $15C22E asm */
void Math_LDivS_68000(void);	/* $15C236 fragment */
long Math_LModS(void);	/* $15C246 asm */
long Math_LModS_68000(void);	/* $15C252 asm */
long Math_DivModS(void);	/* $15C25C asm */
long Math_DivModU(void);	/* $15C2AC asm */
void Pause_LoadSprite(void);	/* $15C306 */
void Pause_ShowText(void);	/* $15C32E */
void Pause_LinkSprites(void);	/* $15C39A */
void Pause_RestoreLink(void);	/* $15C41C */
long Pause_HandleInput(void);	/* $15C42A */
void Pause_CheatUnlock(void);	/* $15C49E fragment */
void Pause_HandleInput_EndCode(void);	/* $15C5D6 fragment */
void Pause_Loop(void);	/* $15CA6C */
long Pause_Check(void);	/* $15CAA4 */
char Tile_RunHooksAtPoints(void *, void *, short, short);	/* $15CB34 */
long Tile_TestAt(long, short, short, char, short, char);	/* $15CECC */
void Map_RedrawBlock(short, short, char);	/* $15D00E */
void Map_RefreshBlock(short, short);	/* $15D7D8 */
void Map_SetBlockFG(short, short, short, char);	/* $15D7F8 */
void Map_SetBlockFGFlip(short, short, short, char, char, char);	/* $15D858 */
void Map_SetBlockBG(short, short, short, char);	/* $15D8CC */
long Actor_GetSize(void);	/* $15D92C */
void Actor_InitSpawnRec(void *, void *);	/* $15D93A */
void Actor_Nop5A(void);	/* $15D970 */
void Actor_ClassInit(void);	/* $15D978 */
void Actor_Nop62(void);	/* $15D980 */
void Obj_AllocPool(short);	/* $15D988 */
long Actor_New(long, void *, long, long, long);	/* $15DA38 */
long Actor_StateFallDead(void *);	/* $15DA90 */
void Actor_StateHurt(void *);	/* $15DAD8 */
long Actor_SpawnRecInView(long, void *);	/* $15DB26 */
long Actor_SpawnFromRec(void *, void *);	/* $15DB8E */
long Actor_SpawnRecOutOfView(void *, void *);	/* $15DBF2 */
void Actor_Despawn(void *, void *);	/* $15DC78 */
void Actor_Init(void *, long, long);	/* $15DCC4 */
long Actor_InActivateWindow(void *);	/* $15DE1E */
void HitList_Init(void);	/* $15DEAE */
long HitList_AllocNode(void);	/* $15DEEE */
ListNode *HitList_Add(void *obj, PTR(ListNode) *head);	/* $15DF2A */
void List_Unlink(ListNode *node, PTR(ListNode) *head);	/* $15DF66 */
void Actor_Activate(void *);	/* $15DFA8 */
long Actor_OutsideActiveWindow(void *);	/* $15E18C */
void Actor_Deactivate(void *);	/* $15E222 */
long Actor_ContainsPoint(void *, short, short);	/* $15E3D4 */
void Actor_SetFrame(void *, short);	/* $15E410 */
void Actor_DrawSprite(void *);	/* $15E452 */
void Actor_Draw(void *);	/* $15E55A */
void Actor_Attack(void *, void *, char, char, short, short);	/* $15E630 */
void Actor_TakeDamage(void *, long, long, char);	/* $15E680 */
void Actor_Kill(void *);	/* $15E6B2 */
void Obj_CarryChild(void *, void *);	/* $15E76C */
void Actor_FreeSegCb(long, long);	/* $15E7EA */
void Actor_Update(void *);	/* $15E7FA */
short Obj_CountOfClass(void *);	/* $15EA78 */
void Actor_Nop6A(void);	/* $15EAC8 */
void Actor_Nop66(void *);	/* $15EAD0 */
long Map_PutRowBlockA(void);	/* $15EAE2 asm */
long Map_PutRowBlockB(void);	/* $15EB78 asm */
void Map_PutRowPartA(void);	/* $15EC0E asm */
void Map_PutRowPartB(void);	/* $15ECB2 asm */
void Map_DrawRowA(void);	/* $15ED56 asm */
void Map_DrawRowB(void);	/* $15EDCC asm */
void Map_StreamRows(void);	/* $15EE48 asm */
void Map_PutColBlockA(void);	/* $15EF90 asm */
void Map_PutColBlockB(void);	/* $15F036 asm */
void Map_DrawColumnA(void);	/* $15F0DC asm */
void Map_DrawColumnB(void);	/* $15F16A asm */
void Map_StreamColumns(void);	/* $15F1F8 asm */
void Nem_Decode(void *, void *, long, char, void *);	/* $15F37C */
long Nem_DecodeLoop(void);	/* $15F3F0 asm */
void Nem_DecodeLoop_Row(void);	/* $15F4BA fragment */
void Nem_DecodeLoop_Inline(void);	/* $15F540 fragment */
void Nem_PutRowVdp(void);	/* $15F566 asm */
void Nem_PutRowRam(void);	/* $15F572 asm */
void Nem_PutRowMasked(void);	/* $15F57E asm */
long Nem_BuildCodeTable(void *, void *);	/* $15F5CA asm */
void Eni_Decode(void *, void *, void *);	/* $15F5FC */
void Eni_Ops(void);	/* $15F65C asm */
long Eni_ReadInline(void);	/* $15F6F0 asm */
void Anim_Advance(void *);	/* $15F740 */
char Anim_ApplyStep(void *, char);	/* $15F78E */
long Anim_AdvanceByDistance(void *, short, long, long);	/* $15F8B6 */
void Spawn_Nop(void);	/* $15F954 */
void Spawn_InitRecords(void);	/* $15F95C */
void VDP_WriteTileRect(void *, void *, long, long, long);	/* $15FA2C */
void Intro_SegaLogoFlash(long, short);	/* $15FA7A */
void Intro_WaitFrame(void);	/* $15FBAE asm */
void Bitmap_Set(void *, short);	/* $15FC70 asm */
char Bitmap_Test(void *, short);	/* $15FC84 asm */
void Bitmap_FromTileWords(void *, void *, long, short);	/* $15FC9A asm */
void Tile_RemapWords(void *, void *, short, long);	/* $15FCD8 asm */
void Scroll_ComputeParallax(void);	/* $15FD0E asm */
long Sys_GetStackPtr(void);	/* $1600A0 asm */
short Rand_Next(void);	/* $1600AA asm */
void Rand_Next_Mix2(void);	/* $1602AC fragment */
void Rand_Next_Mix3(void);	/* $1603E6 fragment */
void Rand_Next_Finish(void);	/* $160500 fragment */
void Rand_Seed(short);	/* $160512 */
void Mem_ClearAboveStack(void);	/* $160550 asm */
void VDP_InitRegs(void);	/* $160594 */
void Sys_InitHardware(void);	/* $160642 */
long Obj_GetLiftCarrier(void *);	/* $1606F4 */
long Cam_GetSize(void);	/* $1607A0 */
long Cam_New(void *, long);	/* $1607AA */
long Cam_Init(void *);	/* $1607E8 */
void Cam_Nop12(void *);	/* $160996 */
long Cam_Update(void *);	/* $1609A8 */
void Cam_FollowBoth(void *, short, short, short, short);	/* $1609CC */
void Cam_FollowPlayer(void *, short, short, short, short);	/* $160A18 */
void Cam_FollowObject(void *, long, short, short, short, short);	/* $160ACA */
void Cam_EaseOffset(void *);	/* $160B0E */
void Cam_ShakeStep(void *);	/* $160BB0 */
void Cam_StepLookDown(void *);	/* $160C3E */
void Cam_StepLookUp(void *);	/* $160D06 */
long Cam_LookUp(void *);	/* $160DCE */
long Cam_LookDown(void *);	/* $160E0E */
void Cam_EndLookUp(void *);	/* $160E4E */
void Cam_EndLookDown(void *);	/* $160E60 */
void Cam_PanStep(void *);	/* $160E72 */
void Cam_PanTo(void *, short, short, short, short, char, char);	/* $160F36 */
void Cam_UnlockAxes(void *);	/* $160FCC */
long Cam_IsPanning(void *);	/* $160FDE */
void Cam_ApplyPos(void *);	/* $160FFE */
void Cam_TrackBoth(void *);	/* $16103C */
long Cam_EaseAxis(short, short, short, short, short);	/* $16118E */
void Cam_SpringAxis(short, short, short, short, short, short, void *, void *, void *, void *);	/* $161222 */
void Cam_TrackPlayer(void *);	/* $16136E */
void Cam_TrackObject(void *);	/* $161744 */
void Cam_DefaultUpdate(void *);	/* $161816 */
long Beast_GetSize(void);	/* $182AE6 */
void Beast_Jump(void *);	/* $182AF4 */
void Beast_SetAirFrame(void *);	/* $182B3C */
long Beast_WallJump(void *);	/* $182BDE */
long Beast_StateGrabWall(void *);	/* $182C74 */
long Beast_StateCling(void *);	/* $182DC2 */
void Beast_DiveImpact(void *);	/* $182F0C */
long Beast_StateDive(void *);	/* $182FFA */
long Beast_StateDiveHold(void *);	/* $1830BE */
long Beast_AirAttackDown(void *);	/* $183136 */
long Beast_AirSpecial(void *);	/* $1831D0 */
void Beast_ShockwaveInit(short, void *, void *, long, char);	/* $18323E */
long Beast_ShockwaveHitCb(void *, short, short, short);	/* $1832B6 */
long Beast_ShockwaveStep(void *, void *, void *, char, char, char);	/* $183320 */
long Beast_UpdateShockwaves(void *);	/* $183512 */
void Beast_StateStompEnd(void *);	/* $1835C2 */
long Beast_StateStomp(void *);	/* $183630 */
long Beast_GroundSpecial(void *);	/* $1836C0 */
void Beast_SetFrameKeepCenter(void *, short);	/* $183724 */
void Beast_SetFrameKeepCenterX(void *, short);	/* $1837B0 */
void Beast_RollLand(void *);	/* $183810 */
long Beast_StateRollRecover(void *);	/* $183870 */
long Beast_StateRollEnd(void *);	/* $1838B8 */
long Beast_StateRoll(void *);	/* $18393C */
void Beast_RollProbeCb(long, void *, void *);	/* $183A24 */
long Beast_RollPathClear(void *);	/* $183A3E */
long Beast_StartRoll(void *);	/* $183AB6 */
long Beast_StateCrouch(void *);	/* $183BAE */
long Beast_Crouch(void *);	/* $183E1A */
void Beast_StateUpAttack(void *);	/* $183EB2 */
long Beast_GroundAttackUp(void *);	/* $183F70 */
void Beast_Init(void *, long, long, long);	/* $183FFE */
long Beast_Attack(void *, long, char, char, short, short);	/* $1840EC */
void Beast_SetFrame(void *, short);	/* $184158 */
long Beast_SelectSpriteDef(void);	/* $1841E4 */
long Beast_SelectFrame(void);	/* $1841F4 */
long Wolverine_GetSize(void);	/* $196552 */
void Wolverine_SetAirFrame(void *);	/* $196560 */
long Wolverine_WallToCeiling(void *);	/* $1965D2 */
long Wolverine_CeilingToWall(void *);	/* $196686 */
long Wolverine_StateWallClimb(void *);	/* $19673A */
long Wolverine_StartCeilingCrawl(void *);	/* $1968D2 */
long Wolverine_StateWallCling(void *);	/* $1969C2 */
void Wolverine_StateCeilingCrawl(void *);	/* $196C82 */
long Wolverine_StartWallClimb(void *);	/* $196E64 */
long Wolverine_StateCeilingHang(void *);	/* $196F70 */
long Wolverine_StateGrab(void *);	/* $197116 */
long Wolverine_StateDive(void *);	/* $1972B8 */
long Wolverine_StateDiveHold(void *);	/* $197348 */
long Wolverine_AirAttackDown(void *);	/* $197412 */
void Wolverine_StateLungeEnd(void *);	/* $197498 */
long Wolverine_StateLunge(void *);	/* $1974EE */
long Wolverine_AirSpecial(void *);	/* $1975E6 */
void Wolverine_LeapSlashEnd(void *);	/* $19767C */
long Wolverine_GroundSpecial(void *);	/* $1976D2 */
long Wolverine_StateCrouch(void *);	/* $19777E */
long Wolverine_Crouch(void *);	/* $1979A4 */
void Wolverine_Update(void *);	/* $197A3C */
void Wolverine_Init(void *, long, long, long);	/* $197ACA */
void Wolverine_SetFrame(void *, short);	/* $197B96 */
long Wolverine_SelectSpriteDef(void);	/* $197C22 */
long Wolverine_SelectFrame(void);	/* $197C32 */
void Wolverine_Reset(void);	/* $197C3C */
long Nightcrawler_FlashPalette(short);	/* $1A60B6 */
long Nightcrawler_GetSize(void);	/* $1A61DE */
void Nightcrawler_Jump(void *);	/* $1A61EC */
void Nightcrawler_DropThrough(void *);	/* $1A621C */
void Nightcrawler_SetAirFrame(void *);	/* $1A6248 */
long Nightcrawler_WallAttackEnd(void *);	/* $1A62A2 */
long Nightcrawler_StateWallClimb(void *);	/* $1A62D6 */
long Nightcrawler_StateWaitUpRelease(void *);	/* $1A6410 */
long Nightcrawler_StateWaitLeftRelease(void *);	/* $1A646A */
long Nightcrawler_StateWaitRightRelease(void *);	/* $1A64C4 */
long Nightcrawler_WallToCeiling(void *);	/* $1A651E */
long Nightcrawler_StateWallCling(void *);	/* $1A65EC */
void Nightcrawler_StateWallCling_Ret(void);	/* $1A6A52 fragment */
long Nightcrawler_StateCeilingCrawl(void *);	/* $1A6A5C */
long Nightcrawler_CeilingToWall(void *);	/* $1A6B36 */
long Nightcrawler_StateCeilingHang(void *);	/* $1A6C48 */
long Nightcrawler_StateGrab(void *);	/* $1A6FDC */
long Nightcrawler_StateDive(void *);	/* $1A714C */
long Nightcrawler_StateDiveHold(void *);	/* $1A71EE */
long Nightcrawler_AirAttackDown(void *);	/* $1A72C6 */
long Nightcrawler_AirAttack(void *);	/* $1A73B2 */
long Nightcrawler_SetTeleportVelocity(void *);	/* $1A74A2 */
long Nightcrawler_StartTeleport(void *);	/* $1A757E */
long Nightcrawler_TeleportVanish(void *);	/* $1A76E6 */
void Nightcrawler_TeleportDown(void *);	/* $1A7730 */
void Nightcrawler_StateTeleport(void *);	/* $1A775E */
void Nightcrawler_TeleportDone(void *);	/* $1A77F0 */
void Nightcrawler_StateTeleportFlash(void *);	/* $1A7848 */
long Nightcrawler_TeleportAppear(void *);	/* $1A78BC */
void Nightcrawler_GroundSpecial(void *);	/* $1A7962 */
void Nightcrawler_AirSpecial(void *);	/* $1A7984 */
long Nightcrawler_Attack(void *, long, char, char, short, short);	/* $1A79A6 */
long Nightcrawler_UpdateCharge(void *);	/* $1A7A86 */
void Nightcrawler_Init(void *, long, long, long);	/* $1A7B10 */
void Nightcrawler_SetFrame(void *, short);	/* $1A7BFE */
void Nightcrawler_Reset(void);	/* $1A7CA4 */
long Nightcrawler_SelectSpriteDef(void);	/* $1A7CAC */
long Nightcrawler_SelectFrame(void);	/* $1A7CBC */
long Psylocke_GetSize(void);	/* $1B9060 */
void Psylocke_SetAirFrame(void *);	/* $1B906E */
void Psylocke_CrouchCombo2(void *);	/* $1B90E0 */
void Psylocke_StateCrouchCombo(void *);	/* $1B9198 */
long Psylocke_GroundAttackDown(void *);	/* $1B9248 */
long Psylocke_StateSlide(void *);	/* $1B92C0 */
long Psylocke_StateCrouch(void *);	/* $1B947C */
long Psylocke_Crouch(void *);	/* $1B976C */
long Psylocke_WallJump(void *);	/* $1B9800 */
long Psylocke_StateGrabWall(void *);	/* $1B9896 */
long Psylocke_StateCling(void *);	/* $1B99C2 */
long Psylocke_StateAirDash(void *);	/* $1B9B02 */
long Psylocke_AirSpecial(void *);	/* $1B9B6E */
void Psylocke_GroundSpecial(void *);	/* $1B9C40 */
void Psylocke_GroundSpecialDown(void *);	/* $1B9CC2 */
void Psylocke_GroundSpecialUp(void *);	/* $1B9D44 */
void Psylocke_Init(void *, long, long, long);	/* $1B9D68 */
void Psylocke_SetFrame(void *, short);	/* $1B9E78 */
long Psylocke_Attack(void *, void *, char, long, short, short);	/* $1B9F1E */
void Psylocke_AfterState(long);	/* $1B9FE8 */
long Psylocke_SelectSpriteDef(void);	/* $1BA006 */
long Psylocke_SelectFrame(void);	/* $1BA016 */
long Magneto_GetSize(void);	/* $1CDA3A */
long Magneto_HoverDamp(void *, char, char);	/* $1CDA48 */
void Magneto_AirSteer(void *);	/* $1CDB32 */
void Magneto_SetAirFrame(void *);	/* $1CDCE8 */
long Magneto_StateAirShotEnd(void *);	/* $1CDDA4 */
long Magneto_StateAirShot(void *);	/* $1CDEB2 */
void Magneto_StartAirShot(void *);	/* $1CDFD2 */
void Magneto_AirAttackUp(void *);	/* $1CE05E */
void Magneto_AirAttack(void *);	/* $1CE0F2 */
void Magneto_AirAttackDown(void *);	/* $1CE19A */
void Magneto_FireShot(void *);	/* $1CE22C */
long Magneto_StartShot(void *);	/* $1CE2CE */
long Magneto_StateAim(void *);	/* $1CE34E */
void Magneto_GroundAttack(void *);	/* $1CE618 */
void Magneto_GroundAttackUp(void *);	/* $1CE646 */
long Magneto_StateCrouchAim(void *);	/* $1CE674 */
void Magneto_GroundAttackDown(void *);	/* $1CE73E */
long Magneto_StateHoldBomb(void *);	/* $1CE778 */
long Magneto_SpawnBomb(void *);	/* $1CE826 */
long Magneto_StartBomb(void *);	/* $1CE902 */
void Magneto_GroundSpecial(void *);	/* $1CE982 */
void Magneto_GroundSpecialUp(void *);	/* $1CEA2E */
void Magneto_GroundSpecialDown(void *);	/* $1CEADA */
void Magneto_Init(void *, long, long, long);	/* $1CEB18 */
void Magneto_TakeDamage(void *, long, char, char, short, short);	/* $1CEBCC */
long Magneto_SelectSpriteDef(void);	/* $1CEC28 */
long Magneto_SelectFrame(void);	/* $1CEC38 */
void Magneto_Reset(void);	/* $1CEC42 */
long Gambit_GetSize(void);	/* $1CEF6E */
long Gambit_SetAirFrame(void *);	/* $1CEF7C */
long Gambit_CardHitEnemy(void *, short, short, void *);	/* $1CF070 */
long Gambit_CardCollide(void *);	/* $1CF0E2 */
char Gambit_FindFreeCard(void *);	/* $1CF1B6 */
long Gambit_SpawnCard(void *, char, char, short, short, short, short, char);	/* $1CF1EC */
long Gambit_PointNearBody(void *, void *, short, short);	/* $1CF36E */
void Gambit_PointNearBody_Type0(void);	/* $1CF3AE fragment */
void Gambit_PointNearBody_Type1(void);	/* $1CF3C0 fragment */
void Gambit_PointNearBody_Test(void);	/* $1CF3E2 fragment */
void Gambit_UpdateCard(void *, void *);	/* $1CF432 */
void Gambit_UpdateCard_TrailLoop(void);	/* $1CF74E fragment */
void Gambit_UpdateCard_TrailNext(void);	/* $1CF7E6 fragment */
void Gambit_AfterState(void *);	/* $1CF7FA */
void Gambit_ThrowCards(void *);	/* $1CF84A */
long Gambit_StateAirThrowEnd(void *);	/* $1CFA48 */
long Gambit_AirThrow(void *);	/* $1CFA90 */
void Gambit_GroundThrow(void *);	/* $1CFAD8 */
long Gambit_StateThrow(void *);	/* $1CFB14 */
long Gambit_StateGroundAim(void *);	/* $1CFB56 */
void Gambit_AirThrowAim(void *);	/* $1CFE26 */
long Gambit_StateAirThrowBrake(void *);	/* $1CFF7E */
long Gambit_StateAirCharge(void *);	/* $1CFFEA */
long Gambit_StateCrouchAim(void *);	/* $1D0058 */
void Gambit_GroundAim(void *);	/* $1D011E */
void Gambit_CrouchAim(void *);	/* $1D0148 */
void Gambit_AirCharge(void *);	/* $1D0182 */
void Gambit_GroundSpecial(void *);	/* $1D01A4 */
void Gambit_GroundSpecialUp(void *);	/* $1D01C6 */
void Gambit_GroundSpecialDown(void *);	/* $1D01E8 */
void Gambit_AirSpecial(void *);	/* $1D020A */
void Gambit_AirSpecialUp(void *);	/* $1D022C */
void Gambit_AirSpecialDown(void *);	/* $1D024E */
void Gambit_ClearCards(void *);	/* $1D0270 */
void Gambit_Init(void *, long, long, long);	/* $1D029E */
void Gambit_SetFrame(void *, short);	/* $1D041E */
long Gambit_StateHardLanding(void *);	/* $1D04C4 */
void Gambit_HardLanding(void *);	/* $1D052A */
long Gambit_StateAirKick(void *);	/* $1D0560 */
long Gambit_AirKick(void *);	/* $1D059C */
long Gambit_AirAttack(void *);	/* $1D06AE */
void Gambit_Nop6A(void);	/* $1D073C */
long Gambit_SelectSpriteDef(void);	/* $1D0744 */
long Gambit_SelectFrame(void);	/* $1D0754 */
void Cyclops_ResetBeamFlicker(void);	/* $1E15F4 asm */
void Cyclops_DrawBeam(void *, void *);	/* $1E1604 */
void Cyclops_DrawBeam_StepLevel(void);	/* $1E16BC fragment */
void Cyclops_DrawBeam_StepDown(void);	/* $1E16CC fragment */
void Cyclops_DrawBeam_StepDiag(void);	/* $1E16D4 fragment */
void Cyclops_DrawBeam_CutVertical(void);	/* $1E1768 fragment */
void Cyclops_DrawBeam_CutBoth(void);	/* $1E176E fragment */
void Cyclops_DrawBeam_Emit(void);	/* $1E177A fragment */
void Cyclops_DrawBeam_None(void);	/* $1E1916 fragment */
void Cyclops_BeamVBlankFlicker(void);	/* $1E1928 asm */
long Cyclops_GetSize(void);	/* $1E2AB0 */
void Cyclops_SetAirFrame(void *);	/* $1E2ABE */
long Cyclops_AirAttack(void *);	/* $1E2B48 */
void Cyclops_AirAttackDown(void *);	/* $1E2C1A */
long Cyclops_ResetBeams(void);	/* $1E2C3E */
void Cyclops_EndBeam(void *);	/* $1E2CBC */
void Cyclops_StartBeam(void *, void *, char);	/* $1E2CFC */
long Cyclops_BeamTimeUp(void *);	/* $1E2F24 */
void Cyclops_BeamHitEnemies(void *);	/* $1E2F46 */
long Cyclops_StateAirBeam(void *);	/* $1E31BA */
void Cyclops_StateGroundBeam(void *);	/* $1E3214 */
void Cyclops_StateCrouchBeam(void *);	/* $1E3246 */
long Cyclops_StateHardLanding(void *);	/* $1E3278 */
void Cyclops_HardLanding(void *);	/* $1E32EC */
void Cyclops_AirBeamAim(void *);	/* $1E3322 */
long Cyclops_StateAirBeamBrake(void *);	/* $1E34BA */
long Cyclops_StateAirCharge(void *);	/* $1E3526 */
long Cyclops_StateGroundAim(void *);	/* $1E3594 */
long Cyclops_StateCrouchAim(void *);	/* $1E3892 */
void Cyclops_AirCharge(void *);	/* $1E3966 */
void Cyclops_GroundAim(void *);	/* $1E3988 */
void Cyclops_CrouchAim(void *);	/* $1E39B8 */
void Cyclops_GroundSpecial(void *);	/* $1E39F2 */
void Cyclops_GroundSpecialUp(void *);	/* $1E3A14 */
void Cyclops_GroundSpecialDown(void *);	/* $1E3A36 */
void Cyclops_AirSpecial(void *);	/* $1E3A58 */
void Cyclops_AirSpecialUp(void *);	/* $1E3A7A */
void Cyclops_AirSpecialDown(void *);	/* $1E3A9C */
void Cyclops_Init(void *, long, long, long);	/* $1E3ABE */
void Cyclops_Draw(void *);	/* $1E3BB0 */
void Cyclops_TakeDamage(void *, long, char, char, short, short);	/* $1E3BF2 */
void Cyclops_Nop6A(void);	/* $1E3C52 */
long Cyclops_SelectSpriteDef(void);	/* $1E3C5A */
long Cyclops_SelectFrame(void);	/* $1E3C6A */
char XPlayer_ActionPressed(void *);	/* $1F005C */
long XPlayer_GetSize(void);	/* $1F00E8 */
void XPlayer_EnterAirFrame(void *);	/* $1F00F6 */
void XPlayer_LeaveGround(void *);	/* $1F0188 */
long XPlayer_CanDoubleJumpNo(void);	/* $1F01F4 */
long XPlayer_DoubleJumpNone(void *);	/* $1F01FE */
long XPlayer_CheckGrabNo(void);	/* $1F0222 */
long XPlayer_StateClimbNone(void *);	/* $1F022C */
long XPlayer_AirSpecial(void *);	/* $1F0250 */
void XPlayer_AirSpecialUp(void *);	/* $1F0280 */
void XPlayer_AirSpecialDown(void *);	/* $1F02A4 */
long XPlayer_GroundSpecial(void *);	/* $1F02C8 */
void XPlayer_GroundSpecialUp(void *);	/* $1F02EC */
void XPlayer_GroundSpecialDown(void *);	/* $1F0310 */
long XPlayer_StartDeath(void *);	/* $1F0334 */
long XPlayer_StateDeathBlink(void *);	/* $1F039A */
void XPlayer_DeathDone(void *);	/* $1F0430 */
long XPlayer_PushingTetherEdge(void *);	/* $1F049E */
void XPlayer_TetherClamp(void *, void *, long, long);	/* $1F0542 */
long XPlayer_TetherToPartner(void *);	/* $1F0582 */
long XPlayer_PlaceAtSafePos(void *);	/* $1F074E */
char XPlayer_CanRejoinAt(void *, void *);	/* $1F07CA */
void XPlayer_StateRejoin(void *);	/* $1F0834 */
long XPlayer_StateFallOffScreen(void *);	/* $1F0930 */
long XPlayer_StartFallOffScreen(void *);	/* $1F0998 */
void XPlayer_StateLookUp(void *);	/* $1F09D6 */
long XPlayer_StartLookUp(void *);	/* $1F0A48 */
long XPlayer_StateControl(void *);	/* $1F0A94 */
void XPlayer_SteerFacing(void *, short, short, char);	/* $1F1372 */
char Player_TopAboveSeg(void *, short, void *);	/* $1F13D0 */
char Player_BottomBelowSeg(void *, short, void *);	/* $1F1424 */
char Player_FrontPastSeg(void *, short, void *);	/* $1F1478 */
long XPlayer_SaveSafePos(void *);	/* $1F1504 */
long XPlayer_Move(void *, short);	/* $1F153A */
long XPlayer_Update(void *);	/* $1F1644 */
long Spr_UploadFrame18(void *);	/* $1F196E */
long XPlayer_Draw(void *);	/* $1F1A2E */
void XPlayer_Deactivate(void *);	/* $1F1C06 */
void XPlayer_Activate(void *);	/* $1F1CD4 */
void XPlayer_AddHealth(void *, long, short);	/* $1F1D88 */
long XPlayer_OutsideActiveWindow(void);	/* $1F1DE2 */
long XPlayer_TakeDamage(void *, void *, char, char);	/* $1F1DEC */
void XPlayer_Attack(void *, void *, char, short, short, short);	/* $1F1FEE */
void XPlayer_Nop72(void);	/* $1F20D0 */
void XPlayer_StateFrozen(void *);	/* $1F20D8 */
void XPlayer_FreezeStanding(void *);	/* $1F20EA */
long XPlayer_UpdateCharge(void *);	/* $1F2126 */
long XPlayer_InitSpawnRec(void *, void *);	/* $1F21C0 */
void XPlayer_ClassInit(void);	/* $1F220E */
void XPlayer_LevelStart(void);	/* $1F2216 */
void XPlayer_Nop66(void *);	/* $1F221E */
void XPlayer_Init(void *, long, long, long);	/* $1F2234 */
void XPlayer_Jump(void *);	/* $1F2306 */
void XPlayer_DropThrough(void *);	/* $1F2376 */
void XPlayer_AirSteer(void *);	/* $1F241C */
void XPlayer_Steer(void *, short, short, char);	/* $1F2520 */
long XPlayer_StateFloatFall(void *);	/* $1F2572 */
void XPlayer_StartFloatFall(void *);	/* $1F26F2 */
void XPlayer_SetAirFrameNop(void);	/* $1F272E */
long XPlayer_StateAirAttack(void *);	/* $1F2736 */
long XPlayer_AirAttackUp(void *);	/* $1F27EA */
long XPlayer_AirAttack(void *);	/* $1F285A */
long XPlayer_AirAttackDown(void *);	/* $1F28CA */
long XPlayer_CanDoubleJump(void *);	/* $1F293A */
long XPlayer_StateDoubleJump(void *);	/* $1F2972 */
long XPlayer_DoubleJump(void *);	/* $1F2AEA */
void XPlayer_AttackRecover(void *);	/* $1F2B8E */
void XPlayer_CrouchAttackRecover(void *);	/* $1F2C6E */
long XPlayer_AttackCombo2(void *);	/* $1F2D74 */
long XPlayer_StateAttackCombo(void *);	/* $1F2E56 */
long XPlayer_GroundAttack(void *);	/* $1F2F0C */
long XPlayer_GroundAttackUp(void *);	/* $1F2FAC */
long XPlayer_GroundAttackDown(void *);	/* $1F3044 */
char XPlayer_Land(void *);	/* $1F30DC */
void XPlayer_StateHardLanding(void *);	/* $1F31F0 */
long XPlayer_HardLanding(void *);	/* $1F3226 */
void XPlayer_WalkAccel(void *, short, short, char);	/* $1F3282 */
void XPlayer_SetWalkAnim(void *, long, long);	/* $1F32D0 */
void XPlayer_Walk(void *);	/* $1F3342 */
long XPlayer_StateWalk(void *);	/* $1F33D0 */
void XPlayer_StateCrouchLook(void *);	/* $1F35DC */
long XPlayer_StateCrouch(void *);	/* $1F364E */
void XPlayer_Crouch(void *);	/* $1F37F2 */
long XPlayer_StateStand(void *);	/* $1F383A */
long XPlayer_Stand(void *);	/* $1F39C4 */
long XPlayer_StateHurtRecover(void *);	/* $1F3ADE */
long XPlayer_StateHurt(void *);	/* $1F3C7C */
long XClimber_GetSize(void);	/* $1F3CD6 */
void XClimber_Init(void *, long, long, long);	/* $1F3CE4 */
void XClimber_Nop66(void *);	/* $1F3D20 */
long XClimber_SaveSafePos(void *);	/* $1F3D36 */
long XClimber_ClassifySeg(void *, void *);	/* $1F3DB4 */
long XClimber_CheckGrab(void *);	/* $1F3EC0 */
void XClimber_CheckGrab_Case0(void);	/* $1F3FCC fragment */
void XClimber_CheckGrab_Default(void);	/* $1F401C fragment */
void XClimber_StateClimbNop(void);	/* $1F4136 */
void XClimber_PlaceAtSafePos(void *);	/* $1F413E */
char Player_IsClimber(void *);	/* $1F41C2 */
char Player_IsNcOrWolverine(void *);	/* $1F4208 */
char Player_IsBeastOrPsylocke(void *);	/* $1F423A */
char Player_IsNcOrWolverine2(void *);	/* $1F426C */
long Player_ClimbState(void *);	/* $1F429E */
long Player_ClimbStateNcWolv(void *);	/* $1F42D0 */
long Player_ClimbStateBeastPsy(void *);	/* $1F4302 */
char Player_CeilingEndInView(void *);	/* $1F4334 */
void EntryPoint(void);	/* $1F43AE fragment */
void Boot_RegionCheck(void);	/* $1F44A8 fragment */
void Boot_DrawText(void);	/* $1F45AE asm */
void Boot_StartApp(void);	/* $1F483E fragment */
void Irq_Enable(void);	/* $1F4854 asm */
void Irq_Disable(void);	/* $1F485A asm */
void Irq_VBlankVector(void);	/* $1F4860 fragment */
void Irq_HBlankVector(void);	/* $1F4870 fragment */
long unk_Sys_1F4890(void);	/* $1F4890 asm */
void Mem_Copy(void *dst, void *src, long n);	/* $1F48AA asm */
void *Mem_Set(void *dst, long c, long n);	/* $1F4900 asm */
long Obj_FindFreeSlot(void);	/* $1F4A1A asm */
void Obj_UpdateBoxes(void *);	/* $1F4A40 asm */
void Obj_UpdateBox(void *);	/* $1F4BA2 asm */
void Actor_Move(long, long, long, void *, long, long, void *);	/* $1F4C56 asm */
void VDP_UpdateScroll(void);	/* $1F4EFC */
void Scroll_RequestVsram(void);	/* $1F5228 */
void Scroll_RequestColumnVs(void);	/* $1F523E */
void Scroll_Init(void);	/* $1F5254 */
long Level_FillBlockSlot(void *, short, short, short, char);	/* $1F5340 */
short Level_AssignBlockSlots(void *, long, long, short);	/* $1F53CC */
void Map_SetRowPriority(void *, short, short);	/* $1F5500 */
void Map_SetRowPalette(void *, short, short, short);	/* $1F553C */
long Level_CompactUsedItems(long, long, void *, char, long, short);	/* $1F5594 */
short Level_LoadUsedTiles(void *, void *, short, long, long, char);	/* $1F5612 */
void Level_LoadMaps(void *, void *, void *, void *, short, short, char, char, char, char);	/* $1F5788 */
void Cam_SetMinXBlocks(short);	/* $1F5F40 */
void Cam_SetMaxXBlocks(short);	/* $1F5F5A */
void Cam_SetPos(short, short);	/* $1F5F78 */
void Cam_SetPosScaled(short, short);	/* $1F60E4 */
long Math_MulFix8(short, short);	/* $1F6258 */
void Obj_SetAnim(void *obj, void *anim);	/* $1F626A */
void Obj_ChangeAnim(void *, void *);	/* $1F6296 */
short SprDef_Add(void *def, short frame);	/* $1F62C0 */
void Obj_SetSpriteDef(Actor *self, unsigned char n);	/* $1F6334 */
char Seg_WouldLeaveEnd(void *, short);	/* $1F638A */
long Seg_AtOpenEndAhead(void *, short);	/* $1F6420 */
long Obj_AttachChild(void *, void *);	/* $1F64B4 */
long Obj_Detach(void *);	/* $1F6530 */
void Obj_ForEachChild(void *, void *);	/* $1F669A */
void Actor_DeactivateFree(void *);	/* $1F66F0 */
void Obj_ForEachChildCall(void *, void *);	/* $1F6774 */
void Pal_StartFadeDelay(short, short, short, long);	/* $1F67C8 */
void Pal_SaveBase(void);	/* $1F6816 */
void Pal_RestoreBase(void);	/* $1F684E */
void Pal_ClearBase(void);	/* $1F6886 */
void Pal_SaveCycle(void);	/* $1F68B2 */
void Pal_RestoreCycle(void);	/* $1F68C0 */
void Pal_FadeOutWait(long);	/* $1F68CE */
void Pal_FadeInWait(long);	/* $1F690E */
XPlayer *Player_Nearest(Actor *self);	/* $1F694A */
long Player_NearestLive(void *);	/* $1F6A32 */
long Obj_StandOnSeg(void *, void *);	/* $1F6B56 */
void Obj_StandOnSegHere(void *);	/* $1F6B9C */
long Seg_ClearAll(void);	/* $1F6BD2 */
void Seg_LoadList(void *, char);	/* $1F6C04 */
long Seg_ApplyObjPos(void *);	/* $1F6D76 */
void Seg_ApplyObjPos_Cases(void);	/* $1F6E42 fragment */
void Seg_ApplyObjPos_Store(void);	/* $1F6EA6 fragment */
long Seg_AlignObjVelocity(void *);	/* $1F6EC4 */
void Seg_AlignObjVelocity_Slope(void);	/* $1F6F80 fragment */
void Seg_AlignObjVelocity_Reverse(void);	/* $1F6F92 fragment */
void Seg_AlignObjVelocity_Flat(void);	/* $1F6FD6 fragment */
void Seg_AlignObjVelocity_Store(void);	/* $1F6FDA fragment */
void Seg_ComputeObjPos(void *, void *);	/* $1F6FF4 */
void Seg_ComputeObjPos_Cases(void);	/* $1F70A4 fragment */
void Seg_ComputeObjPos_Done(void);	/* $1F71E8 fragment */
void Seg_ObjPosUnclamped(void *, void *);	/* $1F71F2 */
void Seg_ObjPosUnclamped_Horiz(void);	/* $1F72A0 fragment */
void Seg_ObjPosUnclamped_Reverse(void);	/* $1F72B6 fragment */
void Seg_ObjPosUnclamped_Done(void);	/* $1F72F6 fragment */
long Seg_CollideBlock(void *, void *, void *);	/* $1F7300 */
void Seg_ApplyPushOut(void *, void *);	/* $1F7628 */
void Seg_ApplyPushOut_Block(void);	/* $1F7686 fragment */
void Seg_ApplyPushOut_Default(void);	/* $1F76D6 fragment */
void Seg_ApplyPushOut_Walls(void);	/* $1F7848 fragment */
void Seg_ApplyPushOut_Move(void);	/* $1F78CA fragment */
void Seg_SetLayerFromObj(void *, void *);	/* $1F7940 */
void Obj_InheritLayer(void *, void *);	/* $1F7956 */
void Seg_ComputeLength(void *);	/* $1F79A2 */
void Seg_ComputeLength_Flat(void);	/* $1F7A7C fragment */
void Seg_ComputeLength_Max(void);	/* $1F7A82 fragment */
void Seg_InitDefaults(void *, char, char);	/* $1F7A9E */
void Seg_ComputeSlope(void *);	/* $1F7AF8 */
void Seg_ComputeSlope_Flat(void);	/* $1F7B36 fragment */
void Seg_ComputeSlope_Calc(void);	/* $1F7B3C fragment */
void Seg_Setup(void *, short, short, short, short, short, short);	/* $1F7BB6 */
short Seg_Alloc(void);	/* $1F7C0A */
long Seg_Create(char, short, short, short, short, short);	/* $1F7C6A */
void Seg_Free(void *);	/* $1F7CFE */
void Seg_ForEachOfObj(void *, void *);	/* $1F7D5E */
long Obj_SnapToNearestSeg(void *, short, short);	/* $1F7DAE */
void Obj_SnapToNearestFloor(void *, short, short);	/* $1F7F38 */
Seg *Seg_FindNearestOfType(short x, short y, short type);	/* $1F80CA */
long Seg_FindContaining(short, short, short);	/* $1F82CC */
Seg *Seg_FindInBlock(short type, short col, short row);	/* $1F8454 */
void Seg_SetEndpoints(long, void *, short, short, short, short);	/* $1F8494 asm */
void Seg_BuildGrid(void);	/* $1F84BE */
void Seg_GatherNearObj(void *, void *);	/* $1F85B6 */
long Seg_CollideObj(void *, void *, void *);	/* $1F87B2 */
void Seg_CollideObj_Cases(void);	/* $1F883A fragment */
void Seg_CollideObj_Trigger(void);	/* $1F895C fragment */
void Seg_CollideObj_CeilFloor(void);	/* $1F89A4 fragment */
void Seg_CollideObj_RetNone(void);	/* $1F8ACA fragment */
void Seg_CollideObj_RetHit(void);	/* $1F8AD6 fragment */
void Seg_CollideObj_Ret(void);	/* $1F8AEE fragment */
long Obj_CollideSegs(void *);	/* $1F8B42 */
long DrawList_Init(void);	/* $1F8EF6 */
long DrawList_AllocNode(void);	/* $1F8F4C */
short DrawList_Add(long, void *, short, char, short, char);	/* $1F8FA2 */
char DrawList_Remove(short);	/* $1F9122 */
char DrawList_SetPriority(short, short, char, short);	/* $1F91E8 */
void DrawList_DrawAll(void);	/* $1F9422 */
void Scene_EndAfter(short);	/* $1F944E */
void Sys_Nop1F945C(void);	/* $1F945C */
char Scene_Register(long, long);	/* $1F9464 */
void Sys_Nop1F949E(void);	/* $1F949E */
long Game_TickFrameCounter(void);	/* $1F94A6 */
void Scene_ClearActivateHook(void);	/* $1F94B6 */
long Obj_FindHitPoint(void *, void *, void *, short, short, void *, void *);	/* $1F94C4 */
void Music_Play(short);	/* $1F97A4 */
long Pad_P1StartDown(void);	/* $1F97DE */
void ClassList_CallSlot5A(void);	/* $1F9804 */
void Sys_InitGameVideo(void);	/* $1F9850 */
void Level_Load(void *, long, void *, long, long, long, char, char, char, char, short, short, short);	/* $1F98A8 */
void Timer_InitAll(void);	/* $1F9C60 */
Timer *Timer_Create(TimerFn func, short period, void *arg, char user);	/* $1F9C92 */
void Timer_Start(Timer *t);	/* $1F9D04 */
void Timer_Stop(Timer *t);	/* $1F9D1E */
void Timer_Free(Timer *t);	/* $1F9D38 */
void Timer_RunAll(void);	/* $1F9D52 */
void Demo_ReadInput(void);	/* $1F9DE8 */
void Pal_RunCycleScript(void);	/* $1F9E46 */
void Collide_ObjVsList(void *, void *);	/* $1F9F0E */
void Collide_RunAll(void);	/* $1FA05A */
char ObjList_ForEachAtPoint(short, short, void *, void *, char, long);	/* $1FA0AE */
void Level_FrameLoop(void);	/* $1FA124 */
void Level_Run(void);	/* $1FA6FE */
void ClassList_InitAll(void);	/* $1FA7E8 */
void ClassList_CallSlot62(void);	/* $1FA878 */
void Sys_Nop1FA8C4(void);	/* $1FA8C4 */
void Sys_Nop1FA8CC(void);	/* $1FA8CC */
void Sys_Nop1FA8D4(void);	/* $1FA8D4 */
void *Mem_StackAlloc(long size, char *tag);	/* $1FA8DC */
void Mem_StackFree(void *);	/* $1FA924 */
void Spr_InitLinks(void);	/* $1FA956 */
void Spr_BeginFrame(void);	/* $1FA99C */
void Spr_EndFrame(void);	/* $1FA9D2 */
short Spr_PreloadTiles(void *, short);	/* $1FAA08 */
short Spr_PreloadToHeap(long, short);	/* $1FAD08 */
void Spr_UploadFrame(void *, char);	/* $1FAD44 */
short Spr_Init(void *, long, void *, short, short, short, char);	/* $1FB026 */
void Spr_InvalidateFrame(void *);	/* $1FB126 */
void Spr_SetFrame(void *, short);	/* $1FB142 */
void Obj_ShiftTree(long, void *);	/* $1FB178 */
void Spr_PlaceAttached(void *, short, short);	/* $1FB1C2 */
void Spr_Draw(void *, short, short);	/* $1FB346 */
void Spr_EmitPreLinked(void *, short, short, short);	/* $1FB592 */
void Spr_EmitPreLinked_Piece(void);	/* $1FB6D6 fragment */
void Spr_EmitPre(void *, short, short, short);	/* $1FB75A */
void Spr_EmitPre_Piece(void);	/* $1FB874 fragment */
void Spr_EmitPreClip(void *, short, short, short);	/* $1FB8A2 */
void Spr_EmitPreClip_Piece(void);	/* $1FB9B8 fragment */
void Spr_EmitStream(void *, short, short, short);	/* $1FBA0C */
void Spr_EmitStream_Piece(void);	/* $1FBB2E fragment */
void Spr_EmitStreamClip(void *, short, short, short);	/* $1FBB5E */
void Spr_EmitStreamClip_Piece(void);	/* $1FBC76 fragment */
void Spr_EmitStreamLinked(void *, short, short, short);	/* $1FBCCE */
void Spr_EmitStreamLinked_Piece(void);	/* $1FBE1A fragment */
long Art_LoadToVramRecolor(void *, short, short, short, short);	/* $1FBE96 */
void Scaler_ExpandSource(void);	/* $1FC1CC asm */
long Scaler_BuildStepTables(void);	/* $1FC1FA asm */
void Scaler_ClearRows(void);	/* $1FC2A6 asm */
void Scaler_InitParams(void);	/* $1FC2E8 asm */
void Spr_RenderToBitmap(void *, short, void *);	/* $1FC35C */
void Spr_RenderToBitmap_Loop(void);	/* $1FC466 fragment */
ScaleParams *Scaler_Start(ScaleParams *params, void *srcA, void *srcB, void *buf, short vram);	/* $1FC5FE */
long Scaler_Step(void);	/* $1FC6CA asm */
long Scaler_Resume(void);	/* $1FC6F6 asm */
long Scaler_BeginPass(void);	/* $1FC708 asm */
long Scaler_RowBlocks(void);	/* $1FC8FE asm */
void Scaler_RowBlocksLast80(void);	/* $1FCE9E fragment */
void Scaler_RowEnd(void);	/* $1FD43E fragment */
long Scaler_EmitSprites(short, short, short, char);	/* $1FD520 */
void Scaler_EmitSprites_Loop(void);	/* $1FD5A0 fragment */
long Pad_ReadPort(void);	/* $1FD61E asm */
void VDP_ResetDmaQueue(void);	/* $1FD720 */
long Art_LoadNemToVram(void *, short);	/* $1FD72C */
void Pal_ApplyFade(void *, void *);	/* $1FD77E */
void Pal_Update(void);	/* $1FD8D2 */
void Pal_StartFade(short, short, long);	/* $1FD998 */
short Pad_Get(char);	/* $1FD9E8 */
void Pad_Update(void);	/* $1FDA56 */
void Scroll_SetParallax(void *);	/* $1FDAEC */
void Scroll_SetParallax_Band0(void);	/* $1FDB6A fragment */
void Scroll_SetParallax_Gradient(void);	/* $1FDB76 fragment */
void Scroll_SetParallax_Band2(void);	/* $1FDBC4 fragment */
void Scroll_SetParallax_Band3(void);	/* $1FDBD0 fragment */
void Scroll_SetParallax_Next(void);	/* $1FDBDC fragment */
void Scroll_UpdateParallax(void);	/* $1FDC18 */
void VDP_WaitVBlank(void);	/* $1FDC5C */
void VDP_FrameUpdate(void);	/* $1FDC90 */
void Irq_VBlank(void);	/* $1FDCCA */
void Irq_HBlank(void);	/* $1FDD0C */
short Pad_ConfigButtonMask(short);	/* $1FDD26 */
long Sys_ReturnTrue(void);	/* $1FDD4C */
long Math_CosDeg(short);	/* $1FDEEA asm */
long Math_SinDeg(short);	/* $1FDEF8 asm */
void VDP_ClearVram(void);	/* $1FDF6C asm */
void VDP_ClearVsram(void);	/* $1FDFCE asm */
void VDP_ClearCram(void);	/* $1FDFE8 asm */
void VDP_WriteWords(void *, long);	/* $1FE002 asm */
short Pad_GetP1Clean(void);	/* $1FE018 asm */
short Pad_GetP2Clean(void);	/* $1FE01E asm */
void Pad_ReadBoth(void);	/* $1FE03C asm */
short Pad_IsP2Connected(void);	/* $1FE052 asm */
void unk_VDP_1FE05C(void);	/* $1FE05C asm */
void VDP_DmaToVram(long, short, long, short);	/* $1FE0EC asm */
void VDP_QueueDma(void *src, long dest, long len);	/* $1FE17A asm */
void VDP_FlushDmaQueue(void);	/* $1FE28C asm */
void VDP_DmaPalette(long, long, void *);	/* $1FE316 asm */
void Z80_BusRequest(void);	/* $1FE3D8 asm */
void Z80_BusFree(void);	/* $1FE3EC asm */
void Z80_DriverPause(void);	/* $1FE3F6 asm */
void Z80_DriverResume(void);	/* $1FE434 asm */
void Z80_LoadDriver(void);	/* $1FE44E asm */
void Z80_ResetRun(void);	/* $1FE48E asm */
long Z80_BusAcquire(void);	/* $1FE4B6 asm */
long Snd_PutCmd(void);	/* $1FE4FE asm */
long Snd_PutByte(void);	/* $1FE50A asm */
void Snd_SendByte(void);	/* $1FE518 asm */
void Snd_SendAddr24(void);	/* $1FE528 asm */
void Snd_Init(long, long, long, long);	/* $1FE544 */
void Snd_Play(long id);	/* $1FE584 asm */
void Snd_Stop(long id);	/* $1FE5A2 asm */
void unk_Snd_1FE5B4(void);	/* $1FE5B4 asm */
void Snd_Pause(void);	/* $1FE5C6 asm */
void Snd_Resume(void);	/* $1FE5DC asm */
void Snd_StopAll(void);	/* $1FE5F2 asm */
void Snd_SendCmd2(void);	/* $1FE618 fragment */
void Snd_SetParam(long slot, long value);	/* $1FE630 asm */
char Snd_ReadDriverByte(long, long);	/* $1FE644 asm */

#endif /* XMEN2_FUNCS_H */
