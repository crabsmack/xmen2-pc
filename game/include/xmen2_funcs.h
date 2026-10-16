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
void Game_Main(void);	/* $005466 */
void Scene_RegisterAll(void);	/* $0054C6 */
void Game_ResetPlayers(void);	/* $005C7C */
void Game_SceneLoop(void);	/* $005CC6 */
void Game_AttractDemoNightcrawler(void);	/* $005E4E fragment */
void Game_AttractDemoCyclops(void);	/* $005E6C fragment */
void Game_PlayAttractDemo(void);	/* $005EB8 fragment */
void Player2_MakeHostile(void);	/* $005F18 */
void Player_SetCharacter(Class *cls, XPlayer *player);	/* $005FA2 */
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
short Player_CharIndexOf(Class *cls);	/* $0061D0 */
char Player1_StartLife(void);	/* $006242 */
void Player1_StartLife_Beast(void);	/* $0062AE fragment */
void Player1_StartLife_Gambit(void);	/* $0062BA fragment */
void Player1_StartLife_Nightcrawler(void);	/* $0062C0 fragment */
void Player1_StartLife_Psylocke(void);	/* $0062C6 fragment */
void Player1_StartLife_Wolverine(void);	/* $0062CC fragment */
char Player2_StartLife(void);	/* $0062E4 */
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
void Players_PlaceAtStart(long x, long y, long x2, long y2);	/* $006BD0 */
void AvalonB1_InitIntroPalette(void);	/* $00F59A */
void AvalonB1_RestorePalette(void);	/* $00F60E */
void AvalonB1_PalFadeStep(short row, s8 *levels);	/* $00F648 */
void AvalonB1_CamHookWait2(Camera *cam);	/* $00F778 */
void AvalonB1_CamHookWait60(Camera *cam);	/* $00F80A */
void AvalonB1_CamHookScroll120(Camera *cam);	/* $00F896 */
void AvalonB1_CamHookFadeIn(Camera *cam);	/* $00F942 */
void AvalonB1_CamHookFinish(Camera *cam);	/* $00FA24 */
void AvalonB1_InitCamera(void);	/* $00FB54 */
void AvalonB1_ScrollCameraTo(short x, short y);	/* $00FC12 */
void AvalonB1_CyclePalettes(void);	/* $00FCA2 */
long AvalonB1_TileHook(short col, short row);	/* $00FCE2 */
void AvalonB1_InstallTileHooks(void);	/* $00FD44 */
void AvalonB1_FrameHook(void);	/* $00FD78 */
void AvalonB1_RunScene(void);	/* $00FDAA */
void BigAcolyte_RenderScaled(ScalePicture *pic, ScaleParams *sp, short vram, char first);	/* $00FEF6 */
long BigAcBullet_GetSize(void);	/* $00FFAE */
void BigAcBullet_Init(BigAcBullet *self, long x, long y, Placement *pl);	/* $00FFBC */
void BigAcBullet_Update(BigAcBullet *self);	/* $010050 */
void BigAcBullet_Attach(void *bullet, void *owner, short dx, short dy, short dir);	/* $010098 */
short BigAcBullet_StateLaunch(BigAcBullet *self);	/* $0100D4 */
void BigAcBullet_StateFly(BigAcBullet *self);	/* $01014C */
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
void CapedAcolyte_Nop6A(CapedAcolyte *self);	/* $013350 */
void CapedAcolyte_ClassInit(void);	/* $013358 */
void CapedAcolyte_Init(CapedAcolyte *self, long x, long y, Placement *pl);	/* $013360 */
void CapedAcolyte_Move(CapedAcolyte *self, short steps);	/* $013596 */
void CapedAcolyte_OnDeath(CapedAcolyte *self, char damage, char type);	/* $013698 */
void CapedAcolyte_EndHurt(CapedAcolyte *self);	/* $0136F0 */
void CapedAcolyte_OnHurt(CapedAcolyte *self, char damage, char type);	/* $013722 */
void CapedAcolyte_OnHurt_KnockY(void);	/* $01376A fragment */
void CapedAcolyte_OnHurt_KnockRight(void);	/* $013782 fragment */
void CapedAcolyte_OnHurt_SetDecel(void);	/* $013788 fragment */
short CapedAcolyte_StateKnockback(CapedAcolyte *self);	/* $0137C4 */
short CapedAcolyte_StateKnockbackRun(CapedAcolyte *self);	/* $0137EE */
void CapedAcolyte_StateReturnHome(CapedAcolyte *self);	/* $0138D4 */
void CapedAcolyte_ActStartMainScript(CapedAcolyte *self);	/* $01396E */
void CapedAcolyte_ActFacePlayer(CapedAcolyte *self);	/* $0139BE */
short CapedAcolyte_ActShootBurst(CapedAcolyte *self);	/* $013A5C */
short CapedAcolyte_StateShoot(CapedAcolyte *self);	/* $013A86 */
short CapedAcolyte_StateHoverNearPlayer(CapedAcolyte *self);	/* $013BAE */
short CapedAcolyte_ActHover(CapedAcolyte *self);	/* $013D26 */
void CapedAcolyte_ActAnimHigh(CapedAcolyte *self);	/* $013D50 */
void CapedAcolyte_ActAnimLow(CapedAcolyte *self);	/* $013DB0 */
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
void ShootAcolyte_Nop66(ShootAcolyte *self);	/* $017FE4 */
void ShootAcolyte_Nop6A(ShootAcolyte *self);	/* $017FFA */
void ShootAcolyte_ClassInit(void);	/* $018002 */
void ShootAcolyte_Init(ShootAcolyte *self, long x, long y, Placement *pl);	/* $01800A */
void ShootAcolyte_Move(ShootAcolyte *self, short steps);	/* $01818A */
void ShootAcolyte_ActRestartScript(ShootAcolyte *self);	/* $0181F8 */
void ShootAcolyte_ActWait30(ShootAcolyte *self);	/* $01823C */
void ShootAcolyte_ActFacePlayer(ShootAcolyte *self);	/* $018290 */
short ShootAcolyte_ActWalk(ShootAcolyte *self);	/* $01830A */
short ShootAcolyte_StateWalkToPlayer(ShootAcolyte *self);	/* $018344 */
short ShootAcolyte_ActShootBurst(ShootAcolyte *self);	/* $0184A4 */
short ShootAcolyte_StateAim(ShootAcolyte *self);	/* $0184CE */
short ShootAcolyte_StateFire(ShootAcolyte *self);	/* $018504 */
short ShootAcolyte_StateNextShot(ShootAcolyte *self);	/* $0185BE */
void ShootAcolyte_Script(void);	/* $0186FA fragment */
void ShootAcolyte_ScriptEnd(void);	/* $018706 fragment */
long Tower_GetSize(void);	/* $01AAA2 */
void Tower_ClassInit(void);	/* $01AAB0 */
void Tower_Init(Tower *self, long x, long y, Placement *pl);	/* $01AABC */
short Tower_StateIdle(Tower *self);	/* $01AB96 */
void Tower_NopStatic(void);	/* $01ABBC */
long YoyoAcolyte_GetSize(void);	/* $01BFF8 */
void YoyoAcolyte_Nop66(YoyoAcolyte *self);	/* $01C006 */
void YoyoAcolyte_Nop6A(YoyoAcolyte *self);	/* $01C01C */
void YoyoAcolyte_ClassInit(void);	/* $01C024 */
void YoyoAcolyte_Init(YoyoAcolyte *self, long x, long y, Placement *pl);	/* $01C042 */
void YoyoAcolyte_Init_Script0(void);	/* $01C176 fragment */
void YoyoAcolyte_Init_Script1to4(void);	/* $01C182 fragment */
void YoyoAcolyte_Init_SetScript(void);	/* $01C1B0 fragment */
void YoyoAcolyte_Update(YoyoAcolyte *self);	/* $01C1DA */
long YoyoAcolyte_OutsideActiveWindow(YoyoAcolyte *self);	/* $01C24C */
void YoyoAcolyte_Draw(YoyoAcolyte *self);	/* $01C282 */
void YoyoAcolyte_ThrowYoyo(YoyoAcolyte *self, short kind);	/* $01C552 */
short YoyoAcolyte_ActIdle100(YoyoAcolyte *self);	/* $01C612 */
void YoyoAcolyte_StateIdleRestart(YoyoAcolyte *self);	/* $01C64C */
short YoyoAcolyte_ActIdle(YoyoAcolyte *self);	/* $01C6BE */
void YoyoAcolyte_StateIdleNext(YoyoAcolyte *self);	/* $01C6E8 */
short YoyoAcolyte_ActThrowLevel(YoyoAcolyte *self);	/* $01C77C */
short YoyoAcolyte_StateThrowLevel(YoyoAcolyte *self);	/* $01C7B2 */
short YoyoAcolyte_ActThrowHigh(YoyoAcolyte *self);	/* $01C7E6 */
short YoyoAcolyte_StateThrowHigh(YoyoAcolyte *self);	/* $01C81C */
short YoyoAcolyte_ActThrowLow(YoyoAcolyte *self);	/* $01C850 */
short YoyoAcolyte_StateThrowLow(YoyoAcolyte *self);	/* $01C886 */
void YoyoAcolyte_StateYoyoOut(YoyoAcolyte *self);	/* $01C8BA */
void YoyoAcolyte_StateDie(YoyoAcolyte *self);	/* $01C94A */
void YoyoAcolyte_ScriptLevel(void);	/* $01CA50 fragment */
void YoyoAcolyte_ScriptHigh(void);	/* $01CA5A fragment */
void YoyoAcolyte_ScriptLow(void);	/* $01CA64 fragment */
void YoyoAcolyte_ScriptMixA(void);	/* $01CA6E fragment */
void YoyoAcolyte_ScriptMixB(void);	/* $01CA88 fragment */
void AvalonB2_CamHook(Camera *cam);	/* $0226B2 */
void AvalonB2_AnimSprite(void);	/* $0226D0 */
void AvalonB2_FrameHook(void);	/* $0226F4 */
void AvalonB2_InitSprite(void);	/* $022716 */
long AvalonB2_TileHook(short col, short row);	/* $022742 */
void AvalonB2_InstallTileHooks(void);	/* $0227A4 */
void AvalonB2_NopStatic(void);	/* $0227D8 */
void AvalonB2_RunScene(void);	/* $0227E0 */
void AvalonA2_EmptyStub1(void);	/* $0228C2 */
void AvalonA2_ResetCounters(void);	/* $0228CA */
void AvalonA2_DisabledFrameEffect(void);	/* $0228DC */
void AvalonA2_DisabledFrameEffect_exit(void);	/* $022958 fragment */
long AvalonA2_ToggleTileHook(short col, short row);	/* $02295C */
void AvalonA2_InstallTileHooks(void);	/* $0229E8 */
void AvalonA2_FrameHook(void);	/* $022A1C */
void AvalonA2_LoadLevel(void);	/* $022A4E */
void AvalonA2_EmptyStub2(void);	/* $022ADA */
void AvalonA2_RaiseCameraFloor(void);	/* $022AE2 */
void AvalonA2_PlacePlayers(void);	/* $022AF6 */
short AvalonA2_HookReturn0(short msg);	/* $022B18 */
short AvalonA2_HookEndOnEvent3(short msg);	/* $022B2E */
void AvalonA2_InstallEventHooks(void);	/* $022B4E */
void AvalonA2_CallEmptyStubs(void);	/* $022B66 */
void AvalonA2_EmptyStub3(void);	/* $022B76 */
void AvalonA2_InstallHooks(void);	/* $022B7E */
void AvalonA2_RunScene(void);	/* $022BA2 */
long RetractFloor_GetSize(void);	/* $0268FE */
void RetractFloor_TriggerNearest(short row);	/* $02690C */
void RetractFloor_Nop66(RetractFloor *self);	/* $0269CE */
void RetractFloor_Nop6A(RetractFloor *self);	/* $0269E4 */
void RetractFloor_ClassInit(void);	/* $0269EC */
void RetractFloor_Init(RetractFloor *self, long x, long y, Placement *pl);	/* $026A0A */
void RetractFloor_Draw(RetractFloor *self);	/* $026AA0 */
void RetractFloor_StartRetract(RetractFloor *self);	/* $026CFA */
short RetractFloor_StateInit(RetractFloor *self);	/* $026D8E */
short RetractFloor_StateWait(RetractFloor *self);	/* $026DB2 */
short RetractFloor_StateDrop(RetractFloor *self);	/* $026E10 */
short RetractFloor_StateSlide(RetractFloor *self);	/* $026ED8 */
void RetractFloor_StateKill(RetractFloor *self);	/* $026F2E */
long Zapper_GetSize(void);	/* $027230 */
void Zapper_Nop66(Zapper *self);	/* $02723E */
void Zapper_Nop6A(Zapper *self);	/* $027254 */
void Zapper_ClassInit(void);	/* $02725C */
void Zapper_Init(Zapper *self, long x, long y, Placement *pl);	/* $02727A */
short Zapper_StateLoopAnim0(Zapper *self);	/* $027352 */
short Zapper_StateLoopAnim1(Zapper *self);	/* $027388 */
short Zapper_StateLoopAnim2(Zapper *self);	/* $0273BE */
void AvalonA3_EmptyStub2(void);	/* $02C902 */
void AvalonA3_PsiFlashStep(void);	/* $02C90A */
void AvalonA3_FrameHook(void);	/* $02C9AA */
void AvalonA3_LoadLevel(void);	/* $02CAF0 */
void AvalonA3_PlacePlayers(void);	/* $02CBA8 */
void AvalonA3_CamHook(Camera *cam);	/* $02CBBE */
void AvalonA3_InitCamera(void);	/* $02CBD0 */
void AvalonA3_SavePalettes(s16 *pal0, s16 *pal2);	/* $02CBF6 */
void AvalonA3_InitSprites(void);	/* $02CC5C */
void AvalonA3_EmptyStub3(void);	/* $02CD30 */
void AvalonA3_InstallHooks(void);	/* $02CD38 */
void AvalonA3_EmptyStub1(void);	/* $02CD48 */
void AvalonA3_RunScene(void);	/* $02CD50 */
long Exodus_GetSize(void);	/* $02CD96 */
short Exodus_StateStart(Exodus *self);	/* $02CDA4 */
short Exodus_StateDescend(Exodus *self);	/* $02CDCA */
short Exodus_StateWaitLanded(Exodus *self);	/* $02CE1A */
void Exodus_NextPattern(Exodus *self);	/* $02CE52 */
void Exodus_NextPattern_part1(void);	/* $02CE94 fragment */
void Exodus_NextPattern_part2(void);	/* $02CEBC fragment */
void Exodus_NextPattern_part3(void);	/* $02CEF6 fragment */
short Exodus_StateNextAttack(Exodus *self);	/* $02CF04 */
void Exodus_StateNextAttack_part1(void);	/* $02CF34 fragment */
void Exodus_StateNextAttack_part2(void);	/* $02CF64 fragment */
void Exodus_StateNextAttack_part3(void);	/* $02CFE8 fragment */
void Exodus_StateNextAttack_part4(void);	/* $02CFF4 fragment */
void Exodus_StateNextAttack_part5(void);	/* $02D0B8 fragment */
void Exodus_StateNextAttack_part6(void);	/* $02D13E fragment */
void Exodus_HurtPlayer(XPlayer *player);	/* $02D146 */
short Exodus_AtkBlast17(Exodus *self);	/* $02D1A6 */
short Exodus_AtkBlast2(Exodus *self);	/* $02D21C */
short Exodus_AtkBlast3(Exodus *self);	/* $02D27A */
short Exodus_AtkBlast4(Exodus *self);	/* $02D2D8 */
short Exodus_AtkSwoopA(Exodus *self);	/* $02D336 */
short Exodus_AtkSwoopB(Exodus *self);	/* $02D372 */
short Exodus_StateSwoopStart(Exodus *self);	/* $02D3AE */
short Exodus_StateSwoopToY(Exodus *self);	/* $02D456 */
short Exodus_StateSwoopBlast(Exodus *self);	/* $02D4BA */
short Exodus_StateRiseStart(Exodus *self);	/* $02D522 */
short Exodus_StateRise(Exodus *self);	/* $02D552 */
short Exodus_StateRiseUntil(Exodus *self);	/* $02D5EA */
short Exodus_StateRiseEnd(Exodus *self);	/* $02D61C */
short Exodus_StateVulnerable(Exodus *self);	/* $02D658 */
short Exodus_StateEndVulnerable(Exodus *self);	/* $02D6D8 */
short Exodus_StateWaitBlastsGone(Exodus *self);	/* $02D720 */
void Exodus_NopStatic(Exodus *self);	/* $02D756 */
void Exodus_InitSpawnRec(SpawnRec *rec, Placement *pl);	/* $02D75E */
void Exodus_ClassInit(Exodus *self);	/* $02D78E */
void Exodus_Nop66(Exodus *self);	/* $02D7B2 */
void Exodus_Move(Exodus *self);	/* $02D7C8 */
void Exodus_Draw(Exodus *self);	/* $02D91C */
void Exodus_HurtOnHotTiles(XPlayer *player);	/* $02D96C */
void Exodus_Update(Exodus *self);	/* $02DA16 */
void Exodus_TakeDamage(Exodus *self, Actor *attacker, char type, char damage, short hx, short hy);	/* $02DADE */
void Exodus_TakeDamage_part1(void);	/* $02DB62 fragment */
void Exodus_TakeDamage_part2(void);	/* $02DB70 fragment */
void Exodus_TakeDamage_part3(void);	/* $02DB84 fragment */
void Exodus_TakeDamage_part4(void);	/* $02DB8E fragment */
void Exodus_TakeDamage_part5(void);	/* $02DB98 fragment */
void Exodus_Init(Exodus *self, long x, long y, Placement *pl);	/* $02DBCE */
void Exodus_StartPsiFlash(void);	/* $02DC80 */
void Exodus_DropPlayerFromSeg(XPlayer *player, Seg *seg);	/* $02DC9E */
void Exodus_BreakFloorTiles(char left);	/* $02DCE4 */
void Exodus_CheckFloorHit(void);	/* $02DDDC */
long ExodusBlast_GetSize(void);	/* $02DF96 */
short ExodusBlast_StateStart(ExodusBlast *self);	/* $02DFA4 */
short ExodusBlast_StateLaunch(ExodusBlast *self);	/* $02DFD8 */
void ExodusBlast_StateLaunch_part1(void);	/* $02E026 fragment */
void ExodusBlast_StateLaunch_part2(void);	/* $02E04E fragment */
void ExodusBlast_StateLaunch_part3(void);	/* $02E062 fragment */
void ExodusBlast_StateLaunch_part4(void);	/* $02E0B0 fragment */
short ExodusBlast_LaunchRight(ExodusBlast *self);	/* $02E0B8 */
short ExodusBlast_LaunchLeft(ExodusBlast *self);	/* $02E0F0 */
short ExodusBlast_LaunchDownLeft(ExodusBlast *self);	/* $02E128 */
short ExodusBlast_LaunchDownRight(ExodusBlast *self);	/* $02E168 */
short ExodusBlast_LaunchDown(ExodusBlast *self);	/* $02E1A8 */
short ExodusBlast_LaunchDropA(ExodusBlast *self);	/* $02E1E0 */
short ExodusBlast_LaunchDropB(ExodusBlast *self);	/* $02E218 */
short ExodusBlast_StateFlyRight(ExodusBlast *self);	/* $02E250 */
short ExodusBlast_StateFlyLeft(ExodusBlast *self);	/* $02E282 */
short ExodusBlast_StateFlyDown(ExodusBlast *self);	/* $02E2B4 */
void ExodusBlast_StateDie(ExodusBlast *self);	/* $02E2E6 */
void ExodusBlast_NopStatic(ExodusBlast *self);	/* $02E306 */
void ExodusBlast_ClassInit(ExodusBlast *self);	/* $02E30E */
void ExodusBlast_Draw(ExodusBlast *self);	/* $02E332 */
void ExodusBlast_Nop66(ExodusBlast *self);	/* $02E492 */
void ExodusBlast_Init(ExodusBlast *self, long x, long y, Placement *pl);	/* $02E4A8 */
ExodusBlast *ExodusBlast_Spawn(long x, long y, char kind);	/* $02E526 */
void AvalonA4_NopStatic(void);	/* $034D4E */
void AvalonA4_ResetEffectCounters(void);	/* $034D56 */
void AvalonA4_AnimatePalettes(void);	/* $034D7A */
void AvalonA4_AnimatePalettes_part1(void);	/* $034DF8 fragment */
void AvalonA4_AnimatePalettes_part2(void);	/* $034EA0 fragment */
void AvalonA4_AnimatePalettes_part3(void);	/* $034EC6 fragment */
void AvalonA4_AnimatePalettes_part4(void);	/* $034F62 fragment */
void AvalonA4_AnimatePalettes_part5(void);	/* $034FDC fragment */
void AvalonA4_AnimatePalettes_part6(void);	/* $035174 fragment */
long AvalonA4_SwitchTileHook(short col, short row);	/* $03518C */
long AvalonA4_GlobCrackBlockHook(short col, short row, Actor *obj);	/* $0351F8 */
long AvalonA4_GlobCrackColumnHook(short col, short row, Actor *obj);	/* $035322 */
long AvalonA4_CrackFloorHook(short col, short row, Actor *obj);	/* $0355A6 */
void AvalonA4_InstallTileHooks(void);	/* $035710 */
void AvalonA4_FrameHook(void);	/* $035750 */
void AvalonA4_LoadLevel(void);	/* $035782 */
void AvalonA4_CamHook(Camera *cam);	/* $035802 */
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
short AvalonA4_EventZoneFree0(short msg);	/* $03597A */
short AvalonA4_EventZoneFree1(short msg);	/* $03599A */
short AvalonA4_EventZoneFree2(short msg);	/* $0359BA */
short AvalonA4_EventZoneFree3(short msg);	/* $0359DA */
short AvalonA4_EventZoneFree4(short msg);	/* $0359FA */
short AvalonA4_EventZoneFree5(short msg);	/* $035A1A */
void AvalonA4_InstallEventHooks(void);	/* $035A3A */
void AvalonA4_EmptyStub1(void);	/* $035A76 */
void AvalonA4_EmptyStub2(void);	/* $035A7E */
void AvalonA4_InstallHooks(void);	/* $035A86 */
void AvalonA4_RunScene(void);	/* $035A9E */
long MagnetoBoss_GetSize(void);	/* $035BDA */
void MagnetoBoss_RubbleTimer(MagnetoRubble *p, Timer *t);	/* $035BE8 */
void MagnetoBoss_SpawnRubble(short col, short row0, short row1, short perRow, char mirror, short floorY, short tile);	/* $035DC2 */
void MagnetoBoss_Nop66(MagnetoBoss *self);	/* $03603E */
void MagnetoBoss_Nop6A(MagnetoBoss *self);	/* $036054 */
void MagnetoBoss_ClassInit(MagnetoBoss *self, short classIndex);	/* $03605C */
void MagnetoBoss_Init(MagnetoBoss *self, long x, long y, Placement *pl);	/* $0360A6 */
void MagnetoBoss_Activate(MagnetoBoss *self);	/* $03628E */
void MagnetoBoss_Update(MagnetoBoss *self);	/* $0363A2 */
void MagnetoBoss_Move(MagnetoBoss *self, short n);	/* $03645C */
void MagnetoBoss_Move_part1(void);	/* $03653E fragment */
void MagnetoBoss_Move_part2(void);	/* $0366E6 fragment */
void MagnetoBoss_TakeDamage(MagnetoBoss *self, Actor *attacker, char type, char damage);	/* $036730 */
long MagnetoBoss_Return0V6E(MagnetoBoss *self);	/* $0367B6 */
void MagnetoBoss_SmashSegment(MagnetoBoss *self, short col);	/* $0367C0 */
void MagnetoBoss_SmashSegmentStaggered(MagnetoBoss *self, short col);	/* $0368D0 */
char MagnetoBoss_CrackWall(short col, short row0, short row1, char instant);	/* $0369F0 */
void MagnetoBoss_EnterSpinState(MagnetoBoss *self);	/* $036AC6 */
void MagnetoBoss_EnterOutroState(MagnetoBoss *self);	/* $036B10 */
short MagnetoBoss_DirFromTo(MagnetoBoss *self, Actor *t);	/* $036B34 */
short MagnetoBoss_StateIntroStart(MagnetoBoss *self);	/* $036C0A */
short MagnetoBoss_StateIntroShake1(MagnetoBoss *self);	/* $036C58 */
short MagnetoBoss_StateIntroWait1(MagnetoBoss *self);	/* $036C9E */
short MagnetoBoss_StateIntroShake2(MagnetoBoss *self);	/* $036CCA */
short MagnetoBoss_StateIntroWait2(MagnetoBoss *self);	/* $036D10 */
short MagnetoBoss_StateIntroShake3(MagnetoBoss *self);	/* $036D3C */
short MagnetoBoss_StateIntroWait3(MagnetoBoss *self);	/* $036DC4 */
short MagnetoBoss_StateBurstIn(MagnetoBoss *self);	/* $036DF0 */
short MagnetoBoss_Zone0Setup(MagnetoBoss *self);	/* $036E82 */
short MagnetoBoss_StateZone0(MagnetoBoss *self);	/* $036EC0 */
short MagnetoBoss_Zone1Setup(MagnetoBoss *self);	/* $036F50 */
short MagnetoBoss_StateZone1(MagnetoBoss *self);	/* $036F9A */
short MagnetoBoss_Zone1Exit(MagnetoBoss *self);	/* $0370CC */
short MagnetoBoss_StateFlyToSmash1(MagnetoBoss *self);	/* $0370FE */
short MagnetoBoss_Zone2Setup(MagnetoBoss *self);	/* $03713C */
short MagnetoBoss_StateZone2Approach(MagnetoBoss *self);	/* $037180 */
short MagnetoBoss_Zone2Hover(MagnetoBoss *self);	/* $037210 */
short MagnetoBoss_StateZone2(MagnetoBoss *self);	/* $03725A */
short MagnetoBoss_Zone2Exit(MagnetoBoss *self);	/* $03738C */
short MagnetoBoss_StateFlyToSmash2(MagnetoBoss *self);	/* $0373BE */
short MagnetoBoss_StateSmash2b(MagnetoBoss *self);	/* $0373FE */
short MagnetoBoss_Zone4Setup(MagnetoBoss *self);	/* $03742E */
short MagnetoBoss_StateZone4Approach(MagnetoBoss *self);	/* $037472 */
short MagnetoBoss_StateResumeShoot(MagnetoBoss *self);	/* $03750A */
short MagnetoBoss_Zone4Hover(MagnetoBoss *self);	/* $03755A */
short MagnetoBoss_StateZone4(MagnetoBoss *self);	/* $0375A4 */
short MagnetoBoss_Zone4Exit(MagnetoBoss *self);	/* $0376C2 */
short MagnetoBoss_StateBreakWall(MagnetoBoss *self);	/* $037700 */
short MagnetoBoss_StateFlyToFinal(MagnetoBoss *self);	/* $037772 */
short MagnetoBoss_StateWaitCamera(MagnetoBoss *self);	/* $0377D6 */
short MagnetoBoss_FinalSetup(MagnetoBoss *self);	/* $03781C */
short MagnetoBoss_StateFinalFight(MagnetoBoss *self);	/* $03784E */
short MagnetoBoss_StateShoot(MagnetoBoss *self);	/* $0378A8 */
short MagnetoBoss_StateAimShoot(MagnetoBoss *self);	/* $037924 */
void MagnetoBoss_StateShootRecover(MagnetoBoss *self);	/* $03795E */
void MagnetoBoss_StateSpin(MagnetoBoss *self);	/* $03799C */
short MagnetoBoss_StateOutro(MagnetoBoss *self);	/* $0379CA */
void MagnetoBoss_StateOutroLoop(MagnetoBoss *self);	/* $037A00 */
long ThronePhone_GetSize(void);	/* $037A96 */
void ThronePhone_Nop66(ThronePhone *self);	/* $037AA4 */
void ThronePhone_Nop6A(ThronePhone *self);	/* $037ABA */
void ThronePhone_ClassInit(ThronePhone *self);	/* $037AC2 */
void ThronePhone_Init(ThronePhone *self, long x, long y, Placement *pl);	/* $037AE0 */
void ThronePhone_SetMode(ThronePhone *self, short mode);	/* $037B74 */
void ThronePhone_Draw(ThronePhone *self);	/* $037BFE */
short ThronePhone_StateInit(ThronePhone *self);	/* $037E58 */
short ThronePhone_StateWaitMagneto(ThronePhone *self);	/* $037E82 */
short ThronePhone_StateCatch(ThronePhone *self);	/* $037F26 */
short ThronePhone_StatePullIn(ThronePhone *self);	/* $037F7E */
void ThronePhone_StateDone(ThronePhone *self);	/* $038036 */
void BaniMazaA1_EmptyStub(void);	/* $0383D4 */
void BaniMazaA1_LoadLevel(void);	/* $0383DC */
void BaniMazaA1_InitSprites(void);	/* $03845C */
long BaniMazaA1_SwitchTileHook(short col, short row);	/* $038742 */
void BaniMazaA1_InstallTileHooks(void);	/* $0387D0 */
void BaniMazaA1_InitTablePtrs(void);	/* $038808 */
void BaniMazaA1_UploadBlockTiles(SprInst *slot, short frame);	/* $038838 */
void BaniMazaA1_AnimateBlocks(void);	/* $0388D4 */
void BaniMazaA1_FrameHook(void);	/* $0389B6 */
void BaniMazaA1_FrameHook_PulseColor(void);	/* $038A0E fragment */
void BaniMazaA1_FrameHook_tail(void);	/* $038A58 fragment */
void BaniMazaA1_CamHook(Camera *cam);	/* $038A6C */
void BaniMazaA1_InitCamera(void);	/* $038A7E */
void BaniMazaA1_InitPalette(void);	/* $038AA4 */
void BaniMazaA1_InstallFrameHook(void);	/* $038AEE */
void BaniMazaA1_PlacePlayers(void);	/* $038AFE */
void BaniMazaA1_InitScroll(void);	/* $038BAC */
void BaniMazaA1_RunScene(void);	/* $038BFA */
long BanimazaDoor_GetSize(void);	/* $039D16 */
short BanimazaDoor_PushCallback(short msg, Seg *seg, Actor *obj, SegContact *c);	/* $039D24 */
void BanimazaDoor_InitDebris(DoorShard *s, short n, long x, long y, unsigned char mirror);	/* $039DBA */
void BanimazaDoor_DrawDebris(DoorShard *s, short n, short tileBase);	/* $039E94 */
char BanimazaDoor_PlayerClear(BanimazaDoor *self, XPlayer *pl);	/* $039F80 */
short BanimazaDoor_StateReset(BanimazaDoor *self);	/* $03A01C */
short BanimazaDoor_StateWaitPlayer(BanimazaDoor *self);	/* $03A05A */
short BanimazaDoor_StateStartRise(BanimazaDoor *self);	/* $03A0AA */
short BanimazaDoor_StateRising(BanimazaDoor *self);	/* $03A11A */
void BanimazaDoor_StateRaised(BanimazaDoor *self);	/* $03A156 */
short BanimazaDoor_Shatter(BanimazaDoor *self);	/* $03A172 */
short BanimazaDoor_StateShattered(BanimazaDoor *self);	/* $03A210 */
short BanimazaDoor_StateWaitRespawn(BanimazaDoor *self);	/* $03A246 */
void BanimazaDoor_NopStatic(BanimazaDoor *self);	/* $03A29E */
void BanimazaDoor_ClassInit(BanimazaDoor *self);	/* $03A2A6 */
void BanimazaDoor_DrawColumn(BanimazaDoor *self);	/* $03A2F4 */
void BanimazaDoor_Draw(BanimazaDoor *self);	/* $03A4EE */
void BanimazaDoor_Draw_part1(void);	/* $03A584 fragment */
void BanimazaDoor_Draw_part2(void);	/* $03A5B4 fragment */
void BanimazaDoor_TakeDamage(BanimazaDoor *self, Actor *attacker, char type, char damage, short hx, short hy);	/* $03A5BE */
char BanimazaDoor_InActivateWindow(BanimazaDoor *self);	/* $03A632 */
char BanimazaDoor_OutsideActiveWindow(BanimazaDoor *self);	/* $03A69A */
void BanimazaDoor_Nop66(BanimazaDoor *self);	/* $03A714 */
void BanimazaDoor_Init(BanimazaDoor *self, long x, long y, Placement *pl);	/* $03A72A */
long RollingDiscs_GetSize(void);	/* $03A86C */
void RollingDiscs_InitStub(void);	/* $03A87A */
void RollingDiscs_UpdateScroll(void);	/* $03A882 */
void RollingDiscs_ReverseAtY(short y);	/* $03ABCE */
void RollingDiscs_NopStatic(RollingDiscs *self);	/* $03AC36 */
void RollingDiscs_ClassInit(RollingDiscs *self);	/* $03AC3E */
void RollingDiscs_Init(RollingDiscs *self, long x, long y, Placement *pl);	/* $03AD14 */
void RollingDiscs_Update(RollingDiscs *self);	/* $03ADC2 */
void RollingDiscs_Move(RollingDiscs *self, short steps);	/* $03ADEE */
void RollingDiscs_Draw(RollingDiscs *self);	/* $03AE14 */
void RollingDiscs_Nop66(RollingDiscs *self);	/* $03AE1C */
void BaniMazaA2_InitDebrisBuf(DebrisGroup *buf);	/* $0472CC */
void BaniMazaA2_UpdateDebrisGroup(DebrisGroup *g);	/* $0472E0 */
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
void BaniMazaA2_SetPaletteStep(void);	/* $04760A */
void unk_BaniMazaA2_04766A(void);	/* $04766A */
void BaniMazaA2_PlacePlayers(void);	/* $047712 */
void BaniMazaA2_FrameHook(void);	/* $0477B4 */
void BaniMazaA2_DrawHook(void);	/* $0477E4 */
void BaniMazaA2_CamHook(Camera *cam);	/* $047834 */
void BaniMazaA2_InitCamera(void);	/* $047858 */
void BaniMazaA2_CallEmptyStub(void);	/* $047886 */
void BaniMazaA2_LoadLevel(void);	/* $047892 */
void BaniMazaA2_RunScene(void);	/* $04791A */
void Apocalypse_ScriptA(void);	/* $04B186 fragment */
void Apocalypse_ScriptBomb(void);	/* $04B1A0 fragment */
void Apocalypse_ScriptB(void);	/* $04B1A6 fragment */
void Apocalypse_ScriptC(void);	/* $04B1C2 fragment */
long Apocalypse_GetSize(void);	/* $04B336 */
void Apocalypse_SelectBombPattern(Apocalypse *self);	/* $04B344 */
void Apocalypse_SelectBombPattern_part1(void);	/* $04B374 fragment */
void Apocalypse_SelectBombPattern_part2(void);	/* $04B3A6 fragment */
void Apocalypse_SelectBombPattern_part3(void);	/* $04B3B0 fragment */
void Apocalypse_NopStatic1(void);	/* $04B3C6 */
void Apocalypse_MoveTo(Apocalypse *self, long x, long y, long shift);	/* $04B3CE */
void Apocalypse_StartJitter(Apocalypse *self);	/* $04B462 */
short Apocalypse_StateEntrance(Apocalypse *self);	/* $04B52E */
void Apocalypse_StateWaitEntrance(Apocalypse *self);	/* $04B59E */
short Apocalypse_ActFlyOff(Apocalypse *self);	/* $04B616 */
void Apocalypse_StateFlyOffWait(Apocalypse *self);	/* $04B654 */
void Apocalypse_NopStatic2(void);	/* $04B6E2 */
void Apocalypse_ActStartMainScript(Apocalypse *self);	/* $04B6EA */
short Apocalypse_ActStartBombs(Apocalypse *self);	/* $04B73A */
short Apocalypse_StateDropBombs(Apocalypse *self);	/* $04B790 */
void Apocalypse_StateBombsDone(Apocalypse *self);	/* $04B848 */
short Apocalypse_ActOrbitFrom180(Apocalypse *self);	/* $04B996 */
short Apocalypse_ActOrbitFrom0(Apocalypse *self);	/* $04B9D0 */
short Apocalypse_StateOrbitStep(Apocalypse *self);	/* $04BA08 */
void Apocalypse_StateWaitArrive(Apocalypse *self);	/* $04BA9C */
short Apocalypse_ActRotateTo0(Apocalypse *self);	/* $04BAF8 */
short Apocalypse_StateRotateTo0(Apocalypse *self);	/* $04BB22 */
short Apocalypse_StateRotateTo180(Apocalypse *self);	/* $04BB98 */
void Apocalypse_NopStatic3(void);	/* $04BC10 */
void Apocalypse_ActSetMoveRates(Apocalypse *self);	/* $04BC18 */
void Apocalypse_ActPickScript(Apocalypse *self);	/* $04BC74 */
void Apocalypse_ActPickScript_part1(void);	/* $04BC9E fragment */
void Apocalypse_ActPickScript_part2(void);	/* $04BCB2 fragment */
void Apocalypse_ActPickScript_part3(void);	/* $04BCEC fragment */
short Apocalypse_ActNextScriptChoice(Apocalypse *self);	/* $04BD22 */
void Apocalypse_ActGoLeft(Apocalypse *self);	/* $04BD5A */
void Apocalypse_ActGoCentre(Apocalypse *self);	/* $04BDB4 */
void Apocalypse_ActGoRight(Apocalypse *self);	/* $04BE1C */
void Apocalypse_ActGoNearPlayer(Apocalypse *self);	/* $04BE76 */
char Apocalypse_TargetWithin(Apocalypse *self, short dist);	/* $04BEFA */
short Apocalypse_StateApproach(Apocalypse *self);	/* $04BF52 */
short Apocalypse_StateLunge(Apocalypse *self);	/* $04C030 */
void Apocalypse_StateLungeEnd(Apocalypse *self);	/* $04C0B2 */
short Apocalypse_StateDropStart(Apocalypse *self);	/* $04C114 */
short Apocalypse_StateDrop(Apocalypse *self);	/* $04C168 */
void Apocalypse_StateDropFall(Apocalypse *self);	/* $04C1D0 */
short Apocalypse_StateDefeated(Apocalypse *self);	/* $04C240 */
short Apocalypse_StateDefeatedWait(Apocalypse *self);	/* $04C2AE */
short Apocalypse_StateDefeatedRise(Apocalypse *self);	/* $04C306 */
void Apocalypse_StateVanish(Apocalypse *self);	/* $04C332 */
short Apocalypse_StateHurt(Apocalypse *self);	/* $04C36C */
short Apocalypse_StateKnockback(Apocalypse *self);	/* $04C432 */
void Apocalypse_StateResumeScript(Apocalypse *self);	/* $04C4C6 */
void Apocalypse_CallNopStatics(void);	/* $04C4F0 */
void Apocalypse_Nop6A(Apocalypse *self);	/* $04C504 */
void Apocalypse_Nop66(Apocalypse *self);	/* $04C510 */
void Apocalypse_ClassInit(void);	/* $04C526 */
void Apocalypse_Init(Apocalypse *self, long x, long y, Placement *pl);	/* $04C584 */
void Apocalypse_SetFrame(Apocalypse *self, short frame);	/* $04C76C */
void Apocalypse_Draw(Apocalypse *self);	/* $04C832 */
void Apocalypse_Move(Apocalypse *self, short steps);	/* $04C9D8 */
void Apocalypse_Update(Apocalypse *self);	/* $04CC42 */
void Apocalypse_Deactivate(Apocalypse *self);	/* $04CCA0 */
void Apocalypse_TakeDamage(Apocalypse *self, Actor *attacker, char type, char damage, short hx, short hy);	/* $04CD3A */
long ApocBomb_GetSize(void);	/* $04DF1A */
void ApocBomb_NopStatic(void);	/* $04DF28 */
void ApocBomb_Nop6A(ApocBomb *self);	/* $04DF30 */
void ApocBomb_Nop66(ApocBomb *self);	/* $04DF3C */
void ApocBomb_SetSpeed(short period, short speed, short accel);	/* $04DF52 */
void ApocBomb_AccelTimer(void);	/* $04DF7A */
void ApocBomb_StartAccelTimer(void);	/* $04DF92 */
void ApocBomb_ClassInit(void);	/* $04DFDC */
void ApocBomb_Init(ApocBomb *self, long x, long y, Placement *pl);	/* $04E000 */
void ApocBomb_Move(ApocBomb *self, short steps);	/* $04E0AC */
void ApocBomb_Update(ApocBomb *self);	/* $04E238 */
void ApocBomb_Draw(ApocBomb *self);	/* $04E3F6 */
void ApocBomb_DrawShadow(Actor *obj);	/* $04E644 */
void ApocBomb_Explode(ApocBomb *self);	/* $04E81A */
void ApocBomb_TakeDamage(ApocBomb *self, Actor *attacker, char type, char damage, short hx, short hy);	/* $04E87C */
void ApocBomb_Attack(ApocBomb *self, Actor *target, char type, char damage, short hx, short hy);	/* $04E8F2 */
void ApocBomb_Spawn(short speed, short param);	/* $04E94A */
void ApocBomb_Spawn_part1(void);	/* $04E9C2 fragment */
void ApocBomb_Spawn_part2(void);	/* $04E9C8 fragment */
void ApocBomb_Spawn_part3(void);	/* $04E9D0 fragment */
void ApocBomb_Spawn_part4(void);	/* $04E9EA fragment */
long BeamScooter_GetSize(void);	/* $04EABA */
void BeamScooter_Spawn(long x, long y);	/* $04EAC8 */
void BeamScooter_NopStatic(void);	/* $04EAF4 */
short BeamScooter_StateEnter(BeamScooter *self);	/* $04EAFC */
short BeamScooter_StateDescend(BeamScooter *self);	/* $04EB3C */
short BeamScooter_StatePickUp(BeamScooter *self);	/* $04EB8A */
void BeamScooter_Nop6A(BeamScooter *self);	/* $04EC64 */
void BeamScooter_ClassInit(void);	/* $04EC70 */
void BeamScooter_Init(BeamScooter *self, long x, long y, Placement *pl);	/* $04EC7C */
void BaniMazaB2_SetParallax(void);	/* $05A8E4 */
short Seq_NextWrap(s16 *seq, s16 *idx, s16 *out);	/* $05A8F8 */
void BaniMazaB2_NopStatic(Actor *obj);	/* $05A938 */
long BaniMazaB2_CrackFloorHook(short col, short row);	/* $05A940 */
void BaniMazaB2_InstallTileHooks(void);	/* $05AB58 */
void BaniMazaB2_FrameHook(void);	/* $05AB8C */
void BaniMazaB2_LoadLevel(void);	/* $05ABAA */
void BaniMazaB2_PlacePlayers(void);	/* $05AC24 */
void BaniMazaB2_CamHook(Camera *cam);	/* $05AC76 */
void BaniMazaB2_InitCamera(void);	/* $05AC88 */
void BaniMazaB2_InitPalette(void);	/* $05ACAE */
void BaniMazaB2_InstallFrameHook(void);	/* $05ACDC */
void BaniMazaB2_RunScene(void);	/* $05ACEC */
long Boulder_GetSize(void);	/* $05AD20 */
void Boulder_FindFloorBelow(Boulder *self);	/* $05AD2E */
void Boulder_SplitIntoPieces(Boulder *self);	/* $05AE10 */
void Boulder_SpawnerTick(long yLimit);	/* $05AED2 */
void Boulder_InitDebrisPool(BoulderDebris *buf);	/* $05B016 */
BoulderDebris *Boulder_AllocDebris(void);	/* $05B058 */
void Boulder_FreeDebris(BoulderDebris *d);	/* $05B08E */
void Boulder_UpdateDebris(void);	/* $05B0C8 */
void Boulder_SpawnDebris(short x, short y, short floorY);	/* $05B322 */
short Boulder_StateWaitGround(Boulder *self);	/* $05B58E */
short Boulder_StateStart(Boulder *self);	/* $05B5D4 */
short Boulder_StateStartRoll(Boulder *self);	/* $05B614 */
short Boulder_StateBounce(Boulder *self);	/* $05B65A */
short Boulder_StatePiecesFall(Boulder *self);	/* $05B98C */
short Boulder_StateShatter(Boulder *self);	/* $05BAAC */
void Boulder_Nop6A(Boulder *self);	/* $05BB06 */
void Boulder_ClassInit(Boulder *self);	/* $05BB0E */
void Boulder_Deactivate(Boulder *self);	/* $05BB86 */
void Boulder_Kill(Boulder *self);	/* $05BBA4 */
void Boulder_Update(Boulder *self);	/* $05BBC2 */
void Boulder_Move(Boulder *self, short steps);	/* $05BC9C */
void Boulder_TakeDamage(Boulder *self, Actor *attacker, char type, char damage, short hx, short hy);	/* $05BD34 */
void Boulder_Draw(Boulder *self);	/* $05BD92 */
void Boulder_Init(Boulder *self, long x, long y, Placement *pl);	/* $05BF4A */
Boulder *Boulder_Spawn(long x, long y, long kind);	/* $05C07C */
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
void Tusk_InitSpawnRec(SpawnRec *, Placement *);	/* $05CD4E */
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
void Cerebro_BigSpriteDecode(BigSprite *bs);	/* $0644D4 */
void Cerebro_BigSpriteDraw(BigSprite *bs);	/* $06451E */
void Cerebro_BigSpriteInit(BigSprite *bs, ScalePicture *pic);	/* $064588 */
short PlayerSelect_AllocCharBuffers(void);	/* $064638 */
void PlayerSelect_AllocCharBuffers_Cases(void);	/* $064674 fragment */
void PlayerSelect_AllocCharBuffers_Measure(void);	/* $0646D6 fragment */
void PlayerSelect_LoadCharFrame(BigSprite *slot);	/* $0647D0 */
void PlayerSelect_LoadCharFrame_Beast(void);	/* $064806 fragment */
void PlayerSelect_LoadCharFrame_Cyclops(void);	/* $064814 fragment */
void PlayerSelect_LoadCharFrame_Nightcrawler(void);	/* $064830 fragment */
void PlayerSelect_LoadCharFrame_Psylocke(void);	/* $06483E fragment */
void PlayerSelect_LoadCharFrame_Wolverine(void);	/* $06484C fragment */
void PlayerSelect_LoadCharFrame_Magneto(void);	/* $06485A fragment */
void PlayerSelect_LoadCharFrame_part1(void);	/* $064868 fragment */
void *PlayerSelect_CharSpriteDef(BigSprite *slot);	/* $06489A */
void PlayerSelect_CharSpriteDef_Beast(void);	/* $0648CC fragment */
void PlayerSelect_CharSpriteDef_Cyclops(void);	/* $0648D4 fragment */
void PlayerSelect_CharSpriteDef_Nightcrawler(void);	/* $0648E4 fragment */
void PlayerSelect_CharSpriteDef_Psylocke(void);	/* $0648EC fragment */
void PlayerSelect_CharSpriteDef_Wolverine(void);	/* $0648F4 fragment */
void PlayerSelect_CharSpriteDef_Magneto(void);	/* $0648FC fragment */
void PlayerSelect_CharSpriteDef_None(void);	/* $064904 fragment */
short PlayerSelect_CharFrameNum(BigSprite *slot);	/* $064912 */
void PlayerSelect_CharFrameNum_Beast(void);	/* $064944 fragment */
void PlayerSelect_CharFrameNum_Cyclops(void);	/* $06494C fragment */
void PlayerSelect_CharFrameNum_Nightcrawler(void);	/* $06495C fragment */
void PlayerSelect_CharFrameNum_Psylocke(void);	/* $064964 fragment */
void PlayerSelect_CharFrameNum_Wolverine(void);	/* $06496C fragment */
void PlayerSelect_CharFrameNum_Magneto(void);	/* $064974 fragment */
void PlayerSelect_CharFrameNum_None(void);	/* $06497C fragment */
void PlayerSelect_DrawFrame(short idx, short x, short y, short pal, char prio);	/* $06498A */
void PlayerSelect_DrawFrames(void);	/* $064AA2 */
void PlayerSelect_PlaceCursor(BigSprite *slot);	/* $064AEC */
void PlayerSelect_RevertCursor(BigSprite *slot);	/* $064B54 */
short PlayerSelect_PrevFreeChar(short c);	/* $064B76 */
short PlayerSelect_NextFreeChar(short c);	/* $064BF0 */
short PlayerSelect_FirstFreeChar(void);	/* $064C66 */
void Cerebro_AnimatePalettes(void);	/* $064CA2 */
void Cerebro_AnimatePalettes_CasesA(void);	/* $064CF2 fragment */
void Cerebro_AnimatePalettes_StepB(void);	/* $064DD2 fragment */
void Cerebro_AnimatePalettes_CasesB(void);	/* $064E14 fragment */
void Cerebro_AnimatePalettes_EndB(void);	/* $064E82 fragment */
void Cerebro_FrameHook(void);	/* $064E9C */
void Cerebro_SetupPalCycles(void);	/* $064EDE */
void Cerebro_PrintText(char *text, short style);	/* $064F70 */
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
void ScalingActor_Init(ScalingActor *self, long x, long y, BigSprite *slot);	/* $06718C */
void ScalingActor_Activate(ScalingActor *self);	/* $067202 */
long ScalingActor_OutsideActiveWindow(ScalingActor *self);	/* $067244 */
void ScalingActor_Move(ScalingActor *self);	/* $06724E */
void ScalingActor_Draw(ScalingActor *self);	/* $06726C */
void ScalingActor_SetFrame(ScalingActor *self, short f);	/* $0672CE */
void ScalingActor_SetFrame_part1(void);	/* $06730C fragment */
void ScalingActor_SetFrame_part2(void);	/* $067360 fragment */
void ScalingActor_SetFrame_part3(void);	/* $06736C fragment */
void PlayerSelect_CalcPortraitPos(ScalingActor *self);	/* $067376 */
void PlayerSelect_PlayPickAnim(ScalingActor *self);	/* $067420 */
void PlayerSelect_PlayPickAnim_part1(void);	/* $06745C fragment */
void PlayerSelect_PlayPickAnim_part2(void);	/* $067478 fragment */
void PlayerSelect_PlayPickAnim_part3(void);	/* $06750E fragment */
void PlayerSelect_PlayIdleAnim(ScalingActor *self);	/* $067542 */
void PlayerSelect_PlayIdleAnim_part1(void);	/* $06757C fragment */
void PlayerSelect_PlayIdleAnim_part2(void);	/* $06758E fragment */
void PlayerSelect_PlayIdleAnim_part3(void);	/* $0675B2 fragment */
void PlayerSelect_PlayIdleAnim_part4(void);	/* $0675C4 fragment */
void PlayerSelect_PlayIdleAnim_part5(void);	/* $0675D6 fragment */
void PlayerSelect_PlayIdleAnim_part6(void);	/* $0675E8 fragment */
void PlayerSelect_PlayIdleAnim_part7(void);	/* $0675F8 fragment */
short PortraitActor_StateInit(ScalingActor *self);	/* $067600 */
short PortraitActor_StateIdle(ScalingActor *self);	/* $0676F0 */
short PortraitActor_StartFlyOut(ScalingActor *self);	/* $06777A */
short PortraitActor_StateFlyOut(ScalingActor *self);	/* $06789C */
short PortraitActor_StateSwapChar(ScalingActor *self);	/* $067936 */
short PortraitActor_StateWaitTurn(ScalingActor *self);	/* $067984 */
short PortraitActor_StateFirstChar(ScalingActor *self);	/* $067A56 */
short PortraitActor_StartFlyIn(ScalingActor *self);	/* $067AE0 */
short PortraitActor_StateFlyIn(ScalingActor *self);	/* $067C32 */
short PortraitActor_StateLanded(ScalingActor *self);	/* $067CC4 */
long BoxInputActor_GetSize(void);	/* $067D6C */
void BoxInputActor_Init(BoxInputActor *self, long x, long y, BigSprite *slot);	/* $067D7A */
long BoxInputActor_OutsideActiveWindow(BoxInputActor *self);	/* $067DF0 */
void BoxInputActor_Move(BoxInputActor *self);	/* $067DFA */
void BoxInputActor_Draw(BoxInputActor *self);	/* $067E18 */
short CursorInput_StatePoll(BoxInputActor *self);	/* $067E5E */
short CursorInput_MoveLeft(BoxInputActor *self);	/* $067FCA */
short CursorInput_MoveRight(BoxInputActor *self);	/* $068078 */
short CursorInput_Confirm(BoxInputActor *self);	/* $068126 */
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
void CloneA1_AnimTilesTimer(void);	/* $077256 asm */
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
void LightBeam_Nop66(LightBeam *);	/* $0777CA */
void LightBeam_ClassInit(void);	/* $0777E0 */
void LightBeam_Activate(LightBeam *);	/* $077814 */
void LightBeam_Init(LightBeam *, long, long, Placement *);	/* $077848 */
void LightBeam_Draw(LightBeam *);	/* $077988 */
short LightBeam_StateIdle(LightBeam *);	/* $077CBC */
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
void CloneA2_CamHookNormal(Camera *cam);	/* $081EDC */
void CloneA2_CamHookAlt(Camera *cam);	/* $081F02 */
void CloneA2_InitCamera(void);	/* $081F28 */
void CloneA2_EmptyStub3(void);	/* $081F5A */
void CloneA2_BgPalTimer(Timer *t);	/* $081F62 */
void CloneA2_BgPalTimer_Case0(void);	/* $081FB4 fragment */
void CloneA2_BgPalTimer_Copy(void);	/* $081FDA fragment */
void CloneA2_StartBgPalTimer(void);	/* $081FFE */
void CloneA2_CamAltTimer(Timer *t);	/* $082038 */
void CloneA2_StartCamAltMode(void);	/* $08206A */
void CloneA2_SetCameraZone(short zone);	/* $0820AE */
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
void BroodBossParts_Init(BroodBossParts *, long, long, Placement *);	/* $08594C */
void BroodBossParts_Draw(BroodBossParts *);	/* $0859B0 */
void BroodBossParts_Update(BroodBossParts *);	/* $085AA6 */
long LittleLeg_GetSize(void);	/* $085BDE */
void LittleLeg_SetWalking(LittleLeg *, unsigned char);	/* $085BE8 */
LittleLeg *LittleLeg_New(long unused, short x, short y);	/* $085C5E */
void LittleLeg_Update(LittleLeg *);	/* $085D4C */
void LittleLeg_Draw(LittleLeg *);	/* $085F0A */
void LittleLeg_Destroy(LittleLeg *);	/* $085F82 */
long BroodBossWall_GetSize(void);	/* $085FCE */
void BroodBossWall_Init(BroodBossWall *, long, long);	/* $085FD8 */
void BroodBossWall_Update(BroodBossWall *);	/* $086032 */
void BroodBossWall_Draw(BroodBossWall *);	/* $086136 */
long BroodBossArm_GetSize(void);	/* $0861F4 */
void BroodBossArm_Init(BroodBossArm *, long, long, Placement *);	/* $086202 */
void BroodBossArm_Draw(BroodBossArm *);	/* $08627E */
long BroodBossDamage_GetSize(void);	/* $0862CA */
void BroodBossDamage_Init(BroodBossDamage *, long, long, Placement *);	/* $0862D8 */
void BroodBossDamage_Draw(BroodBossDamage *);	/* $0864E2 */
void BroodBossDamage_Update(BroodBossDamage *);	/* $0864EA */
short BroodBossDamage_StateIdle(BroodBossDamage *);	/* $08669C */
long BroodBoss_GetSize(void);	/* $0866C0 */
void BroodBoss_Init(BroodBoss *, long, long, Placement *);	/* $0866CE */
long BroodBoss_SpawnRecInView(Class *, SpawnRec *);	/* $0868C6 */
void BroodBoss_Update(BroodBoss *);	/* $0868F0 */
short BroodBoss_StateSetup(BroodBoss *);	/* $086ABC */
short BroodBoss_StateRoar(BroodBoss *);	/* $086C02 */
short BroodBoss_StateEnter(BroodBoss *);	/* $086C3E */
short BroodBoss_StateWaitArrive(BroodBoss *);	/* $086CC4 */
void BroodBoss_ActWait60(BroodBoss *);	/* $086D5E */
void BroodBoss_ActWait30(BroodBoss *);	/* $086DB2 */
void BroodBoss_ActClaw(BroodBoss *);	/* $086E06 */
short BroodBoss_ActDropBomb(BroodBoss *);	/* $086E86 */
short BroodBoss_StateDropBomb(BroodBoss *);	/* $086ECE */
short BroodBoss_ActDropBomb2(BroodBoss *);	/* $086F28 */
short BroodBoss_StateDropBomb2(BroodBoss *);	/* $086F70 */
void BroodBoss_StateAfterBomb(BroodBoss *);	/* $086FCA */
short BroodBoss_ActStomp(BroodBoss *);	/* $08702A */
short BroodBoss_StateStompLand(BroodBoss *);	/* $087072 */
short BroodBoss_StateShockOn(BroodBoss *);	/* $0870AC */
void BroodBoss_StateShockOff(BroodBoss *);	/* $0870EA */
void BroodBoss_ActStop(BroodBoss *);	/* $08718A */
short BroodBoss_ActBringWall(BroodBoss *);	/* $087208 */
short BroodBoss_StateWallStop(BroodBoss *);	/* $087294 */
short BroodBoss_StateChargeStart(BroodBoss *);	/* $087304 */
short BroodBoss_StateCharge(BroodBoss *);	/* $087370 */
short BroodBoss_StateRetreat(BroodBoss *);	/* $087426 */
void BroodBoss_SpawnDebris(BroodBoss *);	/* $0874DC */
short BroodBoss_ActChargeWall(BroodBoss *);	/* $087516 */
short BroodBoss_StateChargeWall(BroodBoss *);	/* $087582 */
short BroodBoss_StateHitWall(BroodBoss *);	/* $087600 */
short BroodBoss_StateRecoil(BroodBoss *);	/* $08765E */
short BroodBoss_StateRecoilReturn(BroodBoss *);	/* $0876BA */
short BroodBoss_ActAdvance(BroodBoss *);	/* $087748 */
short BroodBoss_StateAdvance(BroodBoss *);	/* $0877D2 */
void BroodBoss_StateMakeUnhittable(BroodBoss *);	/* $087832 */
short BroodBoss_ActScream(BroodBoss *);	/* $08789C */
short BroodBoss_StateScream(BroodBoss *);	/* $0878E4 */
void BroodBoss_Defeat(BroodBoss *);	/* $08797A */
short BroodBoss_StateHurt(BroodBoss *);	/* $0879FE */
short BroodBoss_StateHurtEnd(BroodBoss *);	/* $087B22 */
short BroodBoss_StateDeathStart(BroodBoss *);	/* $087BAC */
short BroodBoss_StateDeathSlide(BroodBoss *);	/* $087BFE */
short BroodBoss_StateDeathAnim(BroodBoss *);	/* $087CB4 */
short BroodBoss_StateDeathFx(BroodBoss *);	/* $087CEA */
short BroodBoss_StateDeathSink(BroodBoss *);	/* $087DB0 */
short BroodBoss_StateExplode(BroodBoss *);	/* $087E0C */
short BroodBoss_StateVanish(BroodBoss *);	/* $087EBE */
short BroodBoss_StateClawOut(BroodBoss *);	/* $087EFA */
short BroodBoss_StateRemoveParts(BroodBoss *);	/* $087F3C */
void BroodBoss_StateFinish(BroodBoss *);	/* $087FEA */
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
void EvilMaker_Nop6A(EvilMaker *);	/* $0945D4 */
void EvilMaker_Init(EvilMaker *, long, long, Placement *);	/* $0945E0 */
short EvilMaker_StateWaitKills(EvilMaker *);	/* $0946BA */
short EvilMaker_StateIdle(EvilMaker *);	/* $09472E */
void EvilMaker_ActWait60(EvilMaker *);	/* $094752 */
void EvilMaker_ActDropBeast(EvilMaker *);	/* $0947A6 */
void EvilMaker_ActKill(EvilMaker *);	/* $094834 */
void EvilMaker_ActDropCyclops(EvilMaker *);	/* $094850 */
void EvilMaker_ActDropGambit(EvilMaker *);	/* $0948DE */
void EvilMaker_ActDropMagneto(EvilMaker *);	/* $09496C */
void EvilMaker_ActDropNightcrawler(EvilMaker *);	/* $0949FA */
void EvilMaker_ActDropPsylocke(EvilMaker *);	/* $094A88 */
void EvilMaker_ActDropWolverine(EvilMaker *);	/* $094B16 */
void EvilMaker_NopStatic(void);	/* $094BA4 */
void EvilMaker_ScrStepKill(void);	/* $094C26 fragment */
long EvilDrop_GetSize(void);	/* $094C82 */
EvilDrop *EvilDrop_Spawn(Placement *, void *);	/* $094C90 */
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
void EvilBeast_Init(EvilBeast *self, long x, long y, Placement *place);	/* $097DC2 */
void EvilBeast_UpdateThrows(EvilBeast *self);	/* $097E32 */
short EvilBeast_ActThrow(EvilBeast *self);	/* $097EE0 */
void EvilBeast_StateThrow(EvilBeast *self);	/* $097F60 */
long EvilBeam_GetSize(void);	/* $097FD0 */
EvilBeam *EvilBeam_Construct(EvilBeam *self, short x, short y, long unused, char hflip);	/* $097FDA */
void EvilBeam_Update(EvilBeam *self);	/* $09806C */
void EvilBeam_Draw(EvilBeam *self);	/* $0981FC */
void EvilBeam_Destroy(EvilBeam *self);	/* $098236 */
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
BroodClaw *BroodClaw_Spawn(Placement *rec, char pair);	/* $09AD96 */
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
long Player_MusicVoice(XPlayer *player);	/* $0C1372 */
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
void Hologram_SetXPowerSpawns(char enable);	/* $0E99EC */
void Hologram_SetPose(short pose);	/* $0E9A5C */
void Hologram_FindLifts(Hologram *self);	/* $0E9A8C */
void Hologram_FindGateSeg(Hologram *self);	/* $0E9B2E */
char Hologram_PlayerInArena(Hologram *self, XPlayer *player);	/* $0E9B72 */
void Hologram_LockArena(Hologram *self);	/* $0E9BCE */
void Hologram_BeginDying(void);	/* $0E9C04 */
void Hologram_Explode(Hologram *self);	/* $0E9C5E */
void Hologram_OpenExit(Hologram *self);	/* $0E9D3A */
short Hologram_StateStart(Hologram *self);	/* $0E9D88 */
short Hologram_StateSetup(Hologram *self);	/* $0E9DAE */
short Hologram_WaitLowerLiftBoarded(Hologram *self);	/* $0E9DEA */
short Hologram_WaitLowerLiftDown(Hologram *self);	/* $0E9E36 */
void Hologram_WaitPlayersInArena(Hologram *self);	/* $0E9E68 */
void Hologram_StartDyingScript(Hologram *self);	/* $0E9ED4 */
short Hologram_ExplodeWhenReady(Hologram *self);	/* $0E9F24 */
short Hologram_StateOpenExit(Hologram *self);	/* $0E9F58 */
short Hologram_WaitLowerLiftBoarded2(Hologram *self);	/* $0E9F84 */
short Hologram_WaitLowerLiftUp(Hologram *self);	/* $0E9FC0 */
short Hologram_WaitUpperLiftBoarded(Hologram *self);	/* $0E9FF2 */
short Hologram_RideUpperLift(Hologram *self);	/* $0EA02E */
void Hologram_StateDone(void);	/* $0EA098 */
void Hologram_StartAttackScript(Hologram *self);	/* $0EA0A0 */
void Hologram_ActPose0(Hologram *self);	/* $0EA100 */
short Hologram_SeqA_Pose1(Hologram *self);	/* $0EA15E */
short Hologram_SeqA_Pose2(Hologram *self);	/* $0EA18E */
short Hologram_SeqA_Pose1b(Hologram *self);	/* $0EA1BE */
void Hologram_SeqA_Pose0(Hologram *self);	/* $0EA1EE */
short Hologram_SeqB_Pose3(Hologram *self);	/* $0EA24C */
short Hologram_SeqB_Pose4(Hologram *self);	/* $0EA27C */
short Hologram_SeqB_Pose3b(Hologram *self);	/* $0EA2AC */
void Hologram_SeqB_Pose0(Hologram *self);	/* $0EA2DC */
short Hologram_Raise_Pose3(Hologram *self);	/* $0EA33A */
void Hologram_Raise_Pose4(Hologram *self);	/* $0EA36A */
short Hologram_Lower_Pose3(Hologram *self);	/* $0EA3C8 */
void Hologram_Lower_Pose0(Hologram *self);	/* $0EA3F8 */
short Hologram_Fire1_Pose6(Hologram *self);	/* $0EA456 */
void Hologram_Fire1_Bolts(Hologram *self);	/* $0EA486 */
short Hologram_Fire2_Pose6(Hologram *self);	/* $0EA512 */
void Hologram_Fire2_Bolts(Hologram *self);	/* $0EA542 */
short Hologram_Fire3_Pose5(Hologram *self);	/* $0EA5CE */
void Hologram_Fire3_Bolts(Hologram *self);	/* $0EA5FE */
short Hologram_Fire4_Pose5(Hologram *self);	/* $0EA68A */
void Hologram_Fire4_Bolts(Hologram *self);	/* $0EA6BA */
void Hologram_ActPose0WaitA(Hologram *self);	/* $0EA746 */
void Hologram_ActWaitB(Hologram *self);	/* $0EA7A4 */
void Hologram_ActWait30(Hologram *self);	/* $0EA7F8 */
void Hologram_Reset(Hologram *self);	/* $0EA84C */
void Hologram_InitSpawnRec(SpawnRec *rec, Placement *pl);	/* $0EA858 */
void Hologram_ClassInit(Hologram *self);	/* $0EA888 */
void Hologram_Nop66(Hologram *self);	/* $0EA898 */
void Hologram_Draw(Hologram *self);	/* $0EA8AE */
void Hologram_Init(Hologram *self, long x, long y, Placement *pl);	/* $0EAD14 */
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
char CoreElevator_CarriesAllPlayers(void *);	/* $0EC740 */
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
void SiberiaB1_CamApply(Camera *cam);	/* $0FC78A */
void SiberiaB1_InitCamera(Camera *cam);	/* $0FC890 */
void SiberiaB1_InitWind(void);	/* $0FC8A4 */
void SiberiaB1_StepWind(void);	/* $0FC954 */
void SiberiaB1_InitWindSprite(void);	/* $0FCB8E */
void SiberiaB1_UpdateWindSprite(void);	/* $0FCBDA */
void SiberiaB1_EmptyStub(void);	/* $0FCC62 */
void SiberiaB1_PreLoad(void);	/* $0FCC6A */
void SiberiaB1_PostLoad(void);	/* $0FCC86 */
void SiberiaB1_FrameHook(void);	/* $0FCCAE */
void SiberiaB1_LoadResources(void);	/* $0FCDEE */
void SiberiaB1_DebrisInit(IceShard *pool);	/* $0FCE32 */
IceShard *SiberiaB1_DebrisAlloc(void);	/* $0FCE74 */
void SiberiaB1_DebrisFree(IceShard *p);	/* $0FCEAA */
void SiberiaB1_DebrisUpdate(void);	/* $0FCEDE */
void SiberiaB1_SpawnDebris(short x, short y);	/* $0FD0E0 */
long SiberiaB1_IceTileHit(short col, short row);	/* $0FD24C */
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
long SpaceA1_SwitchTileHit(short col, short row, void *obj, long, long);	/* $10B6BE */
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
void Deathbird_MovePolar(s16 *speed, s16 *accel, s16 *maxSpeed, s32 *heading, s32 *turnRate, s32 *turnAccel, s32 *x, s32 *y, s16 *vx, s16 *vy, s16 *prevVx, s16 *prevVy, short steps);	/* $1111EA */
void Deathbird_MovePolar_Case0(void);	/* $1112FE fragment */
void Deathbird_MovePolar_Case3(void);	/* $111336 fragment */
void Deathbird_MovePolar_Case4(void);	/* $111352 fragment */
void Deathbird_MovePolar_Case16(void);	/* $1114FC fragment */
long Deathbird_GetSize(void);	/* $11157A */
void Deathbird_EmptyStub2(void);	/* $111588 */
void Deathbird_Nop66(Deathbird *self);	/* $111590 */
void Deathbird_LaunchBurst(Deathbird *self);	/* $1115A6 */
void Deathbird_EmptyStub3(void);	/* $111790 */
void Deathbird_EmptyStub4(void);	/* $111798 */
void Deathbird_SetMoveMode(Deathbird *self, short mode);	/* $1117A0 */
void Deathbird_SetMoveMode_Case0(void);	/* $1117DC fragment */
void Deathbird_SetMoveMode_Case1(void);	/* $1117F0 fragment */
void Deathbird_SetMoveMode_Case2(void);	/* $111842 fragment */
void Deathbird_SetMoveMode_Case4(void);	/* $1118D8 fragment */
void Deathbird_SetMoveMode_Case5(void);	/* $111920 fragment */
void Deathbird_SetMoveMode_Case7(void);	/* $111A00 fragment */
void Deathbird_SetMoveMode_Case9(void);	/* $111ABA fragment */
void Deathbird_SetMoveMode_Case10(void);	/* $111B50 fragment */
void Deathbird_SetMoveMode_Default(void);	/* $111B9A fragment */
void Deathbird_UpdateBurst(Deathbird *self);	/* $111BAA */
void Deathbird_ClassInit(void);	/* $111DEA */
void Deathbird_Init(Deathbird *self, long x, long y, Placement *pl);	/* $111E44 */
void Deathbird_Update(Deathbird *self);	/* $112016 */
void Deathbird_Move(Deathbird *self, short steps);	/* $11214E */
void Deathbird_Move_Polar(void);	/* $112186 fragment */
void Deathbird_Move_Wobble(void);	/* $1121C6 fragment */
void Deathbird_Move_Done(void);	/* $112330 fragment */
void Deathbird_SetFrame(Deathbird *self, short frame);	/* $11233A */
void Deathbird_Draw(Deathbird *self);	/* $1123FA */
char Deathbird_OutsideActiveWindow(void);	/* $112596 */
void Deathbird_Reset(void);	/* $1125A0 */
short Deathbird_StateHurt(Deathbird *self);	/* $1125B8 */
short Deathbird_StateHurtFlash(Deathbird *self);	/* $112664 */
short Deathbird_StateFalling(Deathbird *self);	/* $112754 */
short Deathbird_StateDeadFlicker(Deathbird *self);	/* $11286E */
short Deathbird_StateStart(Deathbird *self);	/* $1128AE */
short Deathbird_StateSendHornets(Deathbird *self);	/* $1128E4 */
short Deathbird_StateChooseAttack(Deathbird *self);	/* $112A2E */
short Deathbird_StateSwoopDown(Deathbird *self);	/* $112B2E */
short Deathbird_StateHoverPause(Deathbird *self);	/* $112B90 */
short Deathbird_StateThrowBurst(Deathbird *self);	/* $112BDE */
short Deathbird_StateWaitThenLeave(Deathbird *self);	/* $112C50 */
short Deathbird_StateFlyOff(Deathbird *self);	/* $112C9E */
short Deathbird_StateDiveLow(Deathbird *self);	/* $112D1E */
short Deathbird_StateGoToSwitch(Deathbird *self);	/* $112D9E */
short Deathbird_StateLandOnSwitch(Deathbird *self);	/* $112EA4 */
short Deathbird_StatePeck(Deathbird *self);	/* $112F58 */
short Deathbird_StateHitSwitch(Deathbird *self);	/* $112F8E */
long XNpc_GetSize(void);	/* $113B9A */
void XNpc_Nop66(XNpc *self);	/* $113BA8 */
void XNpc_Init(XNpc *self, long x, long y, Placement *pl);	/* $113BBE */
void XNpc_Explode(XNpc *self);	/* $113C14 */
void XNpc_Move(XNpc *self, short steps);	/* $113C80 */
void XNpc_Attack(XNpc *self, Actor *target, char type, char damage, short hx, short hy);	/* $113CA6 */
short XNpc_GetMoveLimit(XNpc *self, char dir, u8 *kind);	/* $113D0E */
void XNpc_TakeDamage(XNpc *self, Actor *attacker, char type, char damage, short hx, short hy);	/* $113D20 */
void XNpc_TakeDamage_Type4(void);	/* $113D60 fragment */
void XNpc_TakeDamage_Default(void);	/* $113D64 fragment */
void XNpc_TakeDamage_Hurt(void);	/* $113DA0 fragment */
void XNpc_TakeDamage_Done(void);	/* $113E76 fragment */
short XNpc_KnockbackDist(XNpc *self, char strength);	/* $113E80 */
void XNpc_OnHurt(XNpc *self, char damage, char type);	/* $113EDA */
void XNpc_EndHurt(XNpc *self);	/* $113EFE */
void XNpc_OnDeath(XNpc *self, char damage, char type);	/* $113F38 */
void XNpc_HurtReact(XNpc *self, char strength, char type);	/* $113F5C */
void XNpc_HurtReact_Stun(void);	/* $113FFC fragment */
void XNpc_HurtReact_Done(void);	/* $114100 fragment */
void XNpc_DeathReact(XNpc *self, char strength, char type);	/* $11410E */
void XNpc_DeathReact_Explode(void);	/* $1141C6 fragment */
void XNpc_DeathReact_Done(void);	/* $1142D2 fragment */
void XNpc_IgnoreHurt(void);	/* $1142E0 */
void XNpc_KillOrExplode(XNpc *self, char damage, char type);	/* $1142E8 */
void XNpc_StateExplode(XNpc *self);	/* $11432E */
void XNpc_ShakeStep(XNpc *self);	/* $11434A */
void XNpc_CalcKnockback(XNpc *self, short dist, s16 *speed, s16 *decel);	/* $11438A */
short XNpc_StateStunStart(XNpc *self);	/* $1143BC */
short XNpc_StateStun(XNpc *self);	/* $1143EC */
short XNpc_StateKnockStart(XNpc *self);	/* $11446C */
short XNpc_StateKnockShake(XNpc *self);	/* $11449C */
short XNpc_StateKnockSlide(XNpc *self);	/* $114550 */
short XNpc_StateKnockBStart(XNpc *self);	/* $1146C6 */
short XNpc_StateKnockBShake(XNpc *self);	/* $1146F6 */
short XNpc_StateKnockBSlide(XNpc *self);	/* $1147B6 */
short XNpc_StateKnockBExpire(XNpc *self);	/* $114860 */
void XNpc_StartDeathFling(XNpc *self);	/* $1148B2 */
void XNpc_StateDeathFlingHigh(XNpc *self);	/* $114914 */
void XNpc_StateDeathFling(XNpc *self);	/* $11494A */
short XNpc_StateDeathFlying(XNpc *self);	/* $11497C */
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
void ICandy_SetAnim(void *obj, void *anim);	/* $118874 */
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
void BabyBio_Nop66(BabyBio *);	/* $11C154 */
void BabyBio_Nop6A(BabyBio *);	/* $11C16A */
void BabyBio_ClassInit(BabyBio *);	/* $11C172 */
void BabyBio_LevelStart(void);	/* $11C184 */
void BabyBio_Init(BabyBio *self, long x, long y, Placement *pl);	/* $11C190 */
void BabyBio_Move(BabyBio *self, short steps);	/* $11C3C2 */
void BabyBio_Deactivate(BabyBio *self);	/* $11C576 */
void BabyBio_TakeDamage(BabyBio *self, Actor *attacker, char type, char damage, short hx, short hy);	/* $11C5BA */
void BabyBio_EndHurt(BabyBio *self);	/* $11C630 */
short BabyBio_GetMoveLimit(BabyBio *self, char dir, u8 *kind);	/* $11C662 */
void BabyBio_GetMoveLimit_Horiz(void);	/* $11C694 fragment */
void BabyBio_GetMoveLimit_Vert(void);	/* $11C6BC fragment */
void BabyBio_GetMoveLimit_Done(void);	/* $11C6C4 fragment */
void BabyBio_StateStart(BabyBio *);	/* $11C6CE */
void BabyBio_ActNext(BabyBio *);	/* $11C71C */
void BabyBio_StateExplode(BabyBio *);	/* $11C782 */
short BabyBio_StateFallOff(BabyBio *);	/* $11C79E */
void BabyBio_ActFacePlayer(BabyBio *);	/* $11C7E6 */
void BabyBio_ActPlayAnimA(BabyBio *);	/* $11C88C */
void BabyBio_ActWaitParam(BabyBio *);	/* $11C918 */
void BabyBio_ActStopWaitParam(BabyBio *);	/* $11C974 */
void BabyBio_ActStopWait60(BabyBio *);	/* $11C9DC */
void BabyBio_ActStopWait30(BabyBio *);	/* $11CA3C */
void BabyBio_ActPlayAnimB(BabyBio *);	/* $11CA9C */
void BabyBio_ActPlayAnimC(BabyBio *);	/* $11CAFC */
void BabyBio_ActPlayAnimD(BabyBio *);	/* $11CB5C */
short BabyBio_ActWaitPlayerNear192(BabyBio *);	/* $11CBBC */
short BabyBio_ActWaitPlayerNear128(BabyBio *);	/* $11CC7E */
void BabyBio_WaitReachVLimit(BabyBio *);	/* $11CD42 */
short BabyBio_ActMoveVertical(BabyBio *);	/* $11CDA8 */
void BabyBio_WaitReachHLimit(BabyBio *);	/* $11CE34 */
short BabyBio_ActMoveHorizontal(BabyBio *);	/* $11CE9A */
short BabyBio_ActWalkUntilPlayerAhead(BabyBio *);	/* $11CFE8 */
short BabyBio_ActStartWalk(BabyBio *);	/* $11D0D4 */
short BabyBio_WaitStopOrLimit(BabyBio *);	/* $11D184 */
void BabyBio_ActWalkForward(BabyBio *);	/* $11D210 */
short BabyBio_ActFireLoop(BabyBio *);	/* $11D2C6 */
short BabyBio_ActStartFiring(BabyBio *);	/* $11D3F2 */
void BabyBio_ActAttachFlame(BabyBio *);	/* $11D422 */
void BabyBio_ActDropFlame(BabyBio *);	/* $11D4F0 */
short BabyBio_ActCrawl(BabyBio *);	/* $11D55C */
short BabyBio_WaitCrawlLimit(BabyBio *);	/* $11D646 */
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
void BioBalls_Init(BioBalls *self, long x, long y, Placement *pl);	/* $11E7C4 */
void BioBalls_Update(BioBalls *self);	/* $11E8F2 */
void BioBalls_Draw(BioBalls *self);	/* $11E9CC */
void BioBalls_Move(BioBalls *self, short steps);	/* $11ED04 */
void BioBalls_ActCollapse(BioBalls *self);	/* $11EEA8 */
short BioBalls_StateIdle(BioBalls *self);	/* $11EF1E */
void BioBalls_ActClearDone(BioBalls *self);	/* $11EF42 */
void BioBalls_ActSetDone(BioBalls *self);	/* $11EF96 */
short BioBalls_WaitPathDone(BioBalls *self);	/* $11EFF2 */
short BioBalls_WaitMotionDone(BioBalls *self);	/* $11F05C */
void BioBalls_ActClearAnim(BioBalls *self);	/* $11F0C6 */
short BioBalls_StateAnimA(BioBalls *self);	/* $11F11C */
short BioBalls_StateAnimB(BioBalls *self);	/* $11F152 */
char BioBalls_IsDone(BioBalls *self);	/* $11F188 */
s16 *BioBalls_AllocBallList(short n);	/* $11F1A6 */
void BioBalls_UpdateBox(BioBalls *self);	/* $11F22C */
void BioBalls_SetFormation(BioBalls *self, void *formation);	/* $11F2F2 */
void BioBalls_StartPath(BioBalls *self, void *path, char flags, short dx, short dy);	/* $11F3E2 */
void BioBalls_SetPathCollapsed(BioBalls *self, void *path);	/* $11F42C */
void BioBalls_FillFromFirst(BioBalls *self);	/* $11F4C0 */
void BioBalls_ResetToPos(BioBalls *self);	/* $11F538 */
char BioBalls_MotionGather(BioBalls *self, short steps);	/* $11F588 */
char BioBalls_MotionOrbit(BioBalls *self, short steps);	/* $11F67C */
char BioBalls_MotionRing(BioBalls *self, short steps);	/* $11FAA2 */
char BioBalls_MotionHelix(BioBalls *self, short steps);	/* $11FCB8 */
char BioBalls_MotionPath(BioBalls *self, short steps);	/* $11FEE4 */
void BioBalls_SpawnPuff(short x, short y, char flip);	/* $1200AE */
char BioBalls_MotionRoll(BioBalls *self, short steps);	/* $120124 */
void BioBalls_EmptyStub(void);	/* $1203A4 */
void BigBio_Script(void);	/* $120828 fragment */
long BasicBioSentinel_GetSize(void);	/* $120868 */
void BasicBioSentinel_SetFrame(void *, short);	/* $120876 */
long BioSentinel_GetSize(void);	/* $120936 */
void BioSentinel_Deactivate(BioSentinel *self);	/* $120944 */
void BioSentinel_Init(BioSentinel *self, long x, long y, Placement *pl);	/* $1209D8 */
short BioSentinel_StateStart(BioSentinel *self);	/* $120B4A */
short BioSentinel_StateCeilingCrawl(BioSentinel *self);	/* $120C02 */
short BioSentinel_StateDropFlip(BioSentinel *self);	/* $120D32 */
short BioSentinel_StateFalling(BioSentinel *self);	/* $120DA2 */
void BioSentinel_FireBullet(BioSentinel *self, char dir);	/* $120E2C */
short BioSentinel_StateFireUp(BioSentinel *self);	/* $120F00 */
short BioSentinel_StateFireDown(BioSentinel *self);	/* $120F64 */
short BioSentinel_StateFireLevel(BioSentinel *self);	/* $120FC8 */
short BioSentinel_StateAimLevel(BioSentinel *self);	/* $12102C */
short BioSentinel_StateAimUp(BioSentinel *self);	/* $121062 */
short BioSentinel_StateAimDown(BioSentinel *self);	/* $121098 */
short BioSentinel_StateAfterSwipe(BioSentinel *self);	/* $1210CE */
short BioSentinel_StateWalking(BioSentinel *self);	/* $12110E */
short BioSentinel_StateThink(BioSentinel *self);	/* $12123E */
short BioSentinel_StateHurt(BioSentinel *self);	/* $121408 */
void BioSentinel_TakeDamage(BioSentinel *self, Actor *attacker, char type, char damage, short hx, short hy);	/* $1214B2 */
void BioSentinel_EmptyStub(void);	/* $121566 */
void BioSentinel_ClassInit(void);	/* $12156E */
void BioSentinel_Nop6A(BioSentinel *self);	/* $12157A */
void BioSentinel_Nop66(BioSentinel *self);	/* $121582 */
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
void BigAcBullet_Attack(BigAcBullet *self, Actor *target, char type, char damage, short hx, short hy);	/* $133E4A */
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
void FlyBrood_EnterFromSide(FlyBrood *self, char fromLeft);	/* $1345A6 */
long FlyBrood_MoveHoming(FlyBrood *self, short steps);	/* $13463C */
void FlyBrood_MovePath(FlyBrood *self, short steps);	/* $13478C */
void FlyBrood_MoveBallistic(FlyBrood *self, short steps);	/* $1347F8 */
void FlyBrood_MoveSine(FlyBrood *self, short count);	/* $13486A */
void FlyBrood_Nop6A(FlyBrood *self);	/* $1348E8 */
void FlyBrood_Nop66(FlyBrood *self);	/* $1348F0 */
void FlyBrood_ClassInit(void);	/* $134906 */
void FlyBrood_Init(FlyBrood *self, long x, long y, Placement *pl);	/* $134934 */
void FlyBrood_Init_Case0(void);	/* $134AE4 fragment */
void FlyBrood_Init_Case1(void);	/* $134AFA fragment */
void FlyBrood_Init_Case4(void);	/* $134B42 fragment */
void FlyBrood_Init_Tail(void);	/* $134BE6 fragment */
void FlyBrood_Deactivate(FlyBrood *self);	/* $134C3A */
void FlyBrood_Draw(FlyBrood *self);	/* $134C60 */
void FlyBrood_Update(FlyBrood *self);	/* $134D64 */
void FlyBrood_ActRestartScript(FlyBrood *self);	/* $134E2A */
short FlyBrood_WaitPathDone(FlyBrood *self);	/* $134E6A */
short FlyBrood_ActHover90(FlyBrood *self);	/* $134EC4 */
void FlyBrood_StateHoverFacePlayer(FlyBrood *self);	/* $134F20 */
short FlyBrood_ActHoverUntilNear(FlyBrood *self);	/* $134F9C */
void FlyBrood_StateWaitPlayerInRange(FlyBrood *self);	/* $134FF2 */
void FlyBrood_StateFacePlayer(FlyBrood *self);	/* $1350FE */
short FlyBrood_ActHoverForever(FlyBrood *self);	/* $135134 */
void FlyBrood_StateFlyOffKill(FlyBrood *self);	/* $13518A */
short FlyBrood_ActDiveAtPlayer(FlyBrood *self);	/* $1351EA */
short FlyBrood_StateWaitPlayerPassed(FlyBrood *self);	/* $1352D4 */
short FlyBrood_ActDiveAtPlayerLow(FlyBrood *self);	/* $13536A */
short FlyBrood_StateWaitPlayerPassedLow(FlyBrood *self);	/* $135452 */
short FlyBrood_ActSineFlyBy(FlyBrood *self);	/* $1354E8 */
short FlyBrood_StateWaitPlayerPassedSine(FlyBrood *self);	/* $13557A */
short FlyBrood_StateSwoopPatrol(FlyBrood *self);	/* $135610 */
short FlyBrood_WaitSwoopDone(FlyBrood *self);	/* $13570A */
void FlyBrood_StateHit(FlyBrood *self);	/* $135794 */
long Hornet_GetSize(void);	/* $1364B4 */
void Hornet_NopStatic(void);	/* $1364C2 */
void Hornet_MoveSwing(Hornet *self);	/* $1364CA */
void Hornet_MoveStartSwing(Hornet *self);	/* $136584 */
void Hornet_StepCirclePath(Hornet *self);	/* $136598 */
void Hornet_MoveVelocity(Hornet *self);	/* $136642 */
void Hornet_MoveBounce(Hornet *self);	/* $1366A2 */
void Hornet_MoveFlyOff(Hornet *self);	/* $13679A */
void Hornet_MoveWaitThenFlyOff(Hornet *self);	/* $136816 */
void Hornet_MoveRiseThenLeave(Hornet *self);	/* $136880 */
void Hornet_MoveWaitThenRiseLeave(Hornet *self);	/* $1368F4 */
void Hornet_MoveDiveOnce(Hornet *self);	/* $13695C */
void Hornet_MoveDiveAgain(Hornet *self);	/* $136992 */
void Hornet_MoveWaitThenDive(Hornet *self);	/* $1369C8 */
void Hornet_MoveRiseThenWait(Hornet *self);	/* $136A32 */
void Hornet_MoveWaitThenRise(Hornet *self);	/* $136AA6 */
void Hornet_MoveDiveRepeat(Hornet *self);	/* $136B0E */
short Hornet_StateAttackDelay(Hornet *self);	/* $136B44 */
short Hornet_StateFacePlayer(Hornet *self);	/* $136B84 */
short Hornet_StateShotLoop(Hornet *self);	/* $136BD4 */
short Hornet_StateFire(Hornet *self);	/* $136C28 */
short Hornet_StateShotRecover(Hornet *self);	/* $136CCA */
short Hornet_StateDropIn(Hornet *self);	/* $136D1A */
short Hornet_StateDropping(Hornet *self);	/* $136D44 */
short Hornet_StatePickPattern(Hornet *self);	/* $136D86 */
void Hornet_StatePickPattern_CaseDefault(void);	/* $136DAE fragment */
void Hornet_StatePickPattern_Case1(void);	/* $136DC4 fragment */
void Hornet_StatePickPattern_StartAttack(void);	/* $136E04 fragment */
void Hornet_Nop66(Hornet *self);	/* $136E3A */
void Hornet_Reset(void);	/* $136E50 */
void Hornet_ClassInit(void);	/* $136E5C */
void Hornet_Init(Hornet *self, long x, long y, Placement *pl);	/* $136E7C */
void Hornet_Move(Hornet *self);	/* $13707A */
void Hornet_Update(Hornet *self);	/* $13709E */
void Hornet_Explode(Hornet *self);	/* $1370E8 */
Hornet *Hornet_SpawnForDeathbird(long x, long y, Placement *pl, short kind);	/* $137142 */
void Hornet_SpawnForDeathbird_CaseDefault(void);	/* $1371C8 fragment */
void Hornet_SpawnForDeathbird_Case2(void);	/* $1371D8 fragment */
void Hornet_SpawnForDeathbird_Finish(void);	/* $13722C fragment */
long HornetBullet_GetSize(void);	/* $137340 */
void HornetBullet_NopStatic(void);	/* $13734E */
void HornetBullet_Reset(void);	/* $137356 */
void HornetBullet_ClassInit(void);	/* $137362 */
void HornetBullet_Init(HornetBullet *self, long x, long y, Placement *pl);	/* $137382 */
void HornetBullet_Update(HornetBullet *self);	/* $13745E */
void HornetBullet_Draw(HornetBullet *self);	/* $1375DA */
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
void ExplodeFx_Free(ExplodeFx *fx);	/* $13D8F6 */
void ExplodeFx_BurstStart(ExplodeFx *fx, unsigned char layer, char unk43, char prio, char flag);	/* $13D954 */
void ExplodeFx_BurstPuffAim(FxPuff *p, long cx, long cy);	/* $13DA0C */
void ExplodeFx_BurstPuffInit(FxPuff *p, long x, long y);	/* $13DA50 */
void ExplodeFx_BurstUpdate(ExplodeFx *fx);	/* $13DA80 */
void ExplodeFx_BurstDraw(ExplodeFx *fx);	/* $13DBD8 */
void ExplodeFx_Burst(void *obj, short kind);	/* $13DCB8 */
void ExplodeFx_BurstMedium(void *obj);	/* $13DE36 */
void ExplodeFx_BurstSmall(void *obj);	/* $13DE4A */
void ExplodeFx_TrailStart(ExplodeFx *fx, unsigned char layer, char unk43, char prio, char flag);	/* $13DE5E */
void ExplodeFx_TrailSetHead(FxPuff *p, long x, long y, short vx, short vy, short ax, short ay);	/* $13DF16 */
void ExplodeFx_TrailPuffInit(FxPuff *p, long x, long y);	/* $13DF44 */
void ExplodeFx_TrailUpdate(ExplodeFx *fx);	/* $13DF74 */
void ExplodeFx_TrailDraw(ExplodeFx *fx);	/* $13E14A */
void ExplodeFx_Trail(void *obj, short vx, short vy, short ax, short ay);	/* $13E23E */
void ExplodeFx_TrailArc(void *obj);	/* $13E30E */
void ExplodeFx_BounceStart(ExplodeFx *fx, unsigned char layer, char unk43, char prio, char flag);	/* $13E36E */
void ExplodeFx_BounceGroupAim(FxGroup *g, long cx, long cy);	/* $13E426 */
void ExplodeFx_BounceGroupInit(FxGroup *g, long x, long y);	/* $13E490 */
void ExplodeFx_BounceUpdate(ExplodeFx *fx);	/* $13E4EA */
void ExplodeFx_BounceDraw(ExplodeFx *fx);	/* $13E6EE */
short ExplodeFx_Bounce(void *obj);	/* $13E808 */
void ExplodeFx_ChainStart(ExplodeFxChain *fx, unsigned char layer, char unk43, char prio, char flag, short x, short y, char damaging, FxChainStep *path, short repeats);	/* $13E928 */
void ExplodeFx_ChainStep(ExplodeFxChain *fx);	/* $13EA46 */
void ExplodeFx_ChainHit(Actor *o, short x, short y);	/* $13EBB6 */
void ExplodeFx_ChainUpdate(ExplodeFxChain *fx);	/* $13EC26 */
void ExplodeFx_ChainDraw(ExplodeFxChain *fx);	/* $13EE20 */
void ExplodeFx_Chain(void *obj, void *path, unsigned char damaging, short dx, short dy);	/* $13EEFA */
void ExplodeFx_ChainRepeat(void *obj, void *path, unsigned char damaging, short dx, short dy, short repeats);	/* $13EFA0 */
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
WallGunBullet *TechWallGun_FindFreeBullet(TechWallGun *self);	/* $14225C */
void TechWallGun_NopStatic(void);	/* $1422A0 */
void TechWallGun_Explode(TechWallGun *self);	/* $1422A8 */
short TechWallGun_StateHurt(TechWallGun *self);	/* $142322 */
void TechWallGun_StateBurst(TechWallGun *self);	/* $1423D6 */
void TechWallGun_SetupBurst(TechWallGun *self, void *anim, StateFn aim, short dir);	/* $14241C */
short TechWallGun_BurstDiagUp(TechWallGun *self);	/* $142446 */
short TechWallGun_BurstLevel(TechWallGun *self);	/* $142490 */
short TechWallGun_BurstDiagDown(TechWallGun *self);	/* $1424DA */
short TechWallGun_BurstCeilLeft(TechWallGun *self);	/* $142524 */
short TechWallGun_BurstCeilDown(TechWallGun *self);	/* $142562 */
short TechWallGun_BurstCeilRight(TechWallGun *self);	/* $1425A0 */
short TechWallGun_AimAngle(short dx, short dy);	/* $1425DE */
char TechWallGun_AimWall(TechWallGun *self, s16 *angle);	/* $1426FE */
char TechWallGun_AimCeiling(TechWallGun *self, s16 *angle);	/* $14278E */
short TechWallGun_StateAimUp(TechWallGun *self);	/* $14280A */
short TechWallGun_StateAimLevel(TechWallGun *self);	/* $14286A */
short TechWallGun_StateAimDown(TechWallGun *self);	/* $1428E6 */
short TechWallGun_StateCeilAimLeft(TechWallGun *self);	/* $142946 */
short TechWallGun_StateCeilAimDown(TechWallGun *self);	/* $1429A6 */
short TechWallGun_StateCeilAimRight(TechWallGun *self);	/* $142A22 */
short TechWallGun_StartWall(TechWallGun *self);	/* $142A82 */
short TechWallGun_StartCeiling(TechWallGun *self);	/* $142AA6 */
void TechWallGun_Nop66(TechWallGun *self);	/* $142ACA */
void TechWallGun_ClassInit(void);	/* $142AE0 */
void TechWallGun_Init(TechWallGun *self, long x, long y, Placement *pl);	/* $142B54 */
void TechWallGun_UpdateBullet(WallGunBullet *b);	/* $142CD6 */
void TechWallGun_UpdateBullet_Case1(void);	/* $142F0C fragment */
void TechWallGun_UpdateBullet_Case4(void);	/* $142F2A fragment */
void TechWallGun_UpdateBullet_Case5(void);	/* $142F38 fragment */
void TechWallGun_UpdateBullet_Case8(void);	/* $142F5C fragment */
void TechWallGun_UpdateBullet_Case9(void);	/* $142F64 fragment */
void TechWallGun_UpdateBullet_Draw(void);	/* $142F70 fragment */
void TechWallGun_UpdateBullets(void *recs);	/* $14304E */
void TechWallGun_Fire(TechWallGun *self, short dir);	/* $143082 */
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
void *Magneto_AirShotAnim(short dir);	/* $159092 */
void Magneto_AirShotAnim_Case0(void);	/* $1590C6 fragment */
void Magneto_AirShotAnim_Case2(void);	/* $1590DA fragment */
void Magneto_AirShotAnim_Case3(void);	/* $1590E4 fragment */
void Magneto_AirShotAnim_Case7(void);	/* $15910C fragment */
void Magneto_AirShotAnim_Case9(void);	/* $159120 fragment */
void Magneto_AirShotAnim_Default(void);	/* $15912A fragment */
void Magneto_AirShotAnim_Result(void);	/* $15912E fragment */
void Magneto_AirShotAnim_Ret(void);	/* $159130 fragment */
void *Magneto_AirShotEndAnim(short dir);	/* $159138 */
void Magneto_AirShotEndAnim_Case0(void);	/* $15916C fragment */
void Magneto_AirShotEndAnim_Case2(void);	/* $159180 fragment */
void Magneto_AirShotEndAnim_Case3(void);	/* $15918A fragment */
void Magneto_AirShotEndAnim_Case7(void);	/* $1591B2 fragment */
void Magneto_AirShotEndAnim_Case9(void);	/* $1591C6 fragment */
void Magneto_AirShotEndAnim_Default(void);	/* $1591D0 fragment */
void Magneto_AirShotEndAnim_Result(void);	/* $1591D4 fragment */
void Magneto_AirShotEndAnim_Ret(void);	/* $1591D6 fragment */
void *Magneto_ShotAnim(short dir);	/* $1591DE */
void Magneto_ShotAnim_Case1(void);	/* $15920C fragment */
void Magneto_ShotAnim_Case2(void);	/* $159216 fragment */
void Magneto_ShotAnim_Case4(void);	/* $15922A fragment */
void Magneto_ShotAnim_Case7(void);	/* $159248 fragment */
void Magneto_ShotAnim_Case8(void);	/* $159252 fragment */
void Magneto_ShotAnim_Default(void);	/* $15925C fragment */
void Magneto_ShotAnim_Result(void);	/* $159260 fragment */
void Magneto_ShotAnim_Ret(void);	/* $159262 fragment */
void *Magneto_ShotEndAnim(short dir);	/* $15926A */
void Magneto_ShotEndAnim_Case1(void);	/* $159298 fragment */
void Magneto_ShotEndAnim_Case2(void);	/* $1592A2 fragment */
void Magneto_ShotEndAnim_Case4(void);	/* $1592B6 fragment */
void Magneto_ShotEndAnim_Case7(void);	/* $1592D4 fragment */
void Magneto_ShotEndAnim_Case8(void);	/* $1592DE fragment */
void Magneto_ShotEndAnim_Default(void);	/* $1592E8 fragment */
void Magneto_ShotEndAnim_Result(void);	/* $1592EC fragment */
void Magneto_ShotEndAnim_Ret(void);	/* $1592EE fragment */
void Magneto_ShotOriginCrouch(char dir, short *dx, short *dy, long *angle, char *type);	/* $1592F6 */
void Magneto_ShotOriginAir(char dir, short *dx, short *dy, long *angle, char *type);	/* $159354 */
void Magneto_ShotOriginAir_Case0(void);	/* $159398 fragment */
void Magneto_ShotOriginAir_Case9(void);	/* $1593B6 fragment */
void Magneto_ShotOriginAir_Case1(void);	/* $1593D4 fragment */
void Magneto_ShotOriginAir_Case2(void);	/* $1593F2 fragment */
void Magneto_ShotOriginAir_Case3(void);	/* $159410 fragment */
void Magneto_ShotOriginAir_Case7(void);	/* $159482 fragment */
void Magneto_ShotOriginAir_Ret(void);	/* $1594B6 fragment */
void Magneto_ShotOrigin(char dir, short *dx, short *dy, long *angle, char *type);	/* $1594C0 */
void Magneto_ShotOrigin_Case1(void);	/* $159502 fragment */
void Magneto_ShotOrigin_Case7(void);	/* $1595B0 fragment */
void Magneto_ShotOrigin_Case8(void);	/* $1595CA fragment */
void Magneto_ShotOrigin_Ret(void);	/* $1595E4 fragment */
MagneticGlob *Magneto_SpawnShot(long x, long y, char dir, unsigned char byPlayer, unsigned char crouch, unsigned char priority, unsigned char air, short tiles);	/* $1595EE */
void Magneto_BombOriginCrouch(char dir, short *dx, short *dy, long *angle);	/* $1597BE */
void Magneto_BombOrigin(char dir, short *dx, short *dy, long *angle);	/* $15980C */
void Magneto_BombOrigin_Case1(void);	/* $15984E fragment */
void Magneto_BombOrigin_Case2(void);	/* $159864 fragment */
void Magneto_BombOrigin_Case4(void);	/* $15988C fragment */
void Magneto_BombOrigin_Case7(void);	/* $1598C8 fragment */
void Magneto_BombOrigin_Case8(void);	/* $1598DA fragment */
void Magneto_BombOrigin_Ret(void);	/* $1598EC fragment */
FluxBomb *Magneto_SpawnFluxBomb(long x, long y, char dir, unsigned char byPlayer, unsigned char crouch, unsigned char strong);	/* $1598F6 */
void *Magneto_FrameBank(short frame);	/* $1599CC */
short Magneto_FrameInBank(short frame);	/* $159A06 */
void Magneto_SetFrame(Magneto *self, short frame);	/* $159A46 */
void Cam_ClampLookToPlayers(Camera *cam);	/* $159ECC */
void Cam_CommitLook(Camera *cam);	/* $159FB8 */
void Pal_CycleStep(void *, void *, void *, void *, char);	/* $159FD6 */
void Seg_MoveAlong(void *obj, short dist);	/* $15A0AC */
void Seg_MoveAlongSteps(void *obj, short n);	/* $15A168 */
void Obj_RideRail(void *obj, short steps);	/* $15A19C */
void AiScript_Set(AiCursor *cursor, void *table);	/* $15A6DE */
char Obj_InScreenWindow(Actor *self);	/* $15A6FC */
char Obj_OutsideActivateWindow(Actor *self);	/* $15A79C */
long Seg_GridCell(long x, long y);	/* $15A7C8 */
Seg *Seg_FindAt(short x, short y, short maxLayer, short height);	/* $15A82A */
Seg *Seg_FindBelow(short x, short y, short maxLayer);	/* $15A8FE */
Seg *Seg_FindAbove(short x, short y, short maxLayer);	/* $15AA20 */
Seg *Seg_FindLeft(short x, short y, short type);	/* $15AB44 */
Seg *Seg_FindRight(short x, short y, short type);	/* $15AC68 */
long SegHook_HurtPlayerStop(short, long, void *, void *);	/* $15AD8A */
long SegHook_HurtPlayer(short, long, void *, void *);	/* $15ADEC */
void Spr_EmitFlicker(Actor *obj, u8 *tiles, SprInst *base, char flicker, unsigned char mask, unsigned char pal, unsigned char hmask, unsigned char vmask);	/* $15AE48 */
void Timer_SetVBlankHookCb(long, long);	/* $15B0D0 */
void Spr_ReloadDefs(void (*load)(void), char repack);	/* $15B0E6 */
void Spr_SetFromDef(SprInst *spr, unsigned char n);	/* $15B18A */
long Math_LMul(long a, long b);	/* $15C1FC asm regs(d0,d1)->d0 */
long Math_LDivS(long n, long d);	/* $15C222 asm regs(d0,d1)->d0 */
long Math_LDivS_Sel(long n, long d);	/* $15C22E asm regs(d0,d1)->d0 */
void Math_LDivS_68000(void);	/* $15C236 fragment */
long Math_LModS(long n, long d);	/* $15C246 asm regs(d0,d1)->d0 */
long Math_LModS_68000(long n, long d);	/* $15C252 asm regs(d0,d1)->d0 */
long Math_DivModS(long n, long d, long *q);	/* $15C25C asm regs(d0,d1,&d1)->d0 */
unsigned long Math_DivModU(unsigned long n, unsigned long d, unsigned long *q);	/* $15C2AC asm regs(d0,d1,&d1)->d0 */
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
long Tile_TestAt(void *obj, short x, short y, char fgMap, short mask, char runHook);	/* $15CECC */
void Map_RedrawBlock(short, short, char);	/* $15D00E */
void Map_RefreshBlock(short, short);	/* $15D7D8 */
void Map_SetBlockFG(short, short, short, char);	/* $15D7F8 */
void Map_SetBlockFGFlip(short, short, short, char, char, char);	/* $15D858 */
void Map_SetBlockBG(short, short, short, char);	/* $15D8CC */
long Actor_GetSize(void);	/* $15D92C */
void Actor_InitSpawnRec(SpawnRec *rec, Placement *pl);	/* $15D93A */
void Actor_Nop5A(void);	/* $15D970 */
void Actor_ClassInit(void);	/* $15D978 */
void Actor_Nop62(void);	/* $15D980 */
void Obj_AllocPool(short count);	/* $15D988 */
Actor *Actor_New(long unused, Class *cls, long x, long y, void *pl);	/* $15DA38 */
short Actor_StateFallDead(Actor *self);	/* $15DA90 */
short Actor_StateHurt(Actor *self);	/* $15DAD8 */
long Actor_SpawnRecInView(SpawnRec *self, SpawnRec *rec);	/* $15DB26 */
Actor *Actor_SpawnFromRec(SpawnRec *self, SpawnRec *rec);	/* $15DB8E */
char Actor_SpawnRecOutOfView(Actor *self, SpawnRec *rec);	/* $15DBF2 */
void Actor_Despawn(Actor *self, SpawnRec *rec);	/* $15DC78 */
void Actor_Init(Actor *self, long x, long y, void *pl);	/* $15DCC4 */
char Actor_InActivateWindow(Actor *self);	/* $15DE1E */
void HitList_Init(void);	/* $15DEAE */
long HitList_AllocNode(void);	/* $15DEEE */
ListNode *HitList_Add(void *obj, PTR(ListNode) *head);	/* $15DF2A */
void List_Unlink(ListNode *node, PTR(ListNode) *head);	/* $15DF66 */
void Actor_Activate(Actor *self);	/* $15DFA8 */
char Actor_OutsideActiveWindow(Actor *self);	/* $15E18C */
void Actor_Deactivate(Actor *self);	/* $15E222 */
long Actor_ContainsPoint(Actor *self, short x, short y);	/* $15E3D4 */
void Actor_SetFrame(void *obj, short f);	/* $15E410 */
void Actor_DrawSprite(Actor *self);	/* $15E452 */
void Actor_Draw(Actor *self);	/* $15E55A */
void Actor_Attack(Actor *self, Actor *target, char type, char damage, short hx, short hy);	/* $15E630 */
void Actor_TakeDamage(Actor *self, Actor *attacker, char type, char damage, short hx, short hy);	/* $15E680 */
void Actor_Kill(Actor *self);	/* $15E6B2 */
void Obj_CarryChild(Actor *parent, Actor *child);	/* $15E76C */
void Actor_FreeSegCb(Actor *obj, Seg *seg);	/* $15E7EA */
void Actor_Update(Actor *self);	/* $15E7FA */
short Obj_CountOfClass(Class *cls);	/* $15EA78 */
void Actor_Nop6A(void);	/* $15EAC8 */
void Actor_Nop66(Actor *self);	/* $15EAD0 */
void Map_PutRowBlockA(u16 *defs, unsigned short cell, short r, short vramRow, short *vramCol);	/* $15EAE2 asm regs(a2,d2,d1,d6,&d7) clobbers(d4,d5) */
void Map_PutRowBlockB(u16 *defs, unsigned short cell, short r, short vramRow, short *vramCol);	/* $15EB78 asm regs(a2,d2,d1,d6,&d7) clobbers(d4,d5) */
void Map_PutRowPartA(u16 *defs, unsigned short cell, short r, short from, short to, short vramRow, short *vramCol);	/* $15EC0E asm regs(a2,d2,d1,d4,d3,d6,&d7) clobbers(d3,d4,d5) */
void Map_PutRowPartB(u16 *defs, unsigned short cell, short r, short from, short to, short vramRow, short *vramCol);	/* $15ECB2 asm regs(a2,d2,d1,d4,d3,d6,&d7) clobbers(d3,d4,d5) */
void Map_DrawRowA(s16 *map, u16 *defs, short tileX, short tileY, short vramRow, short vramCol);	/* $15ED56 asm regs(a4,a2,d4,d5,d6,d7) clobbers(d3,a3) */
void Map_DrawRowB(s16 *map, u16 *defs, short tileX, short tileY, short vramRow, short vramCol);	/* $15EDCC asm regs(a4,a2,d4,d5,d6,d7) clobbers(d3,a3) */
void Map_StreamRows(void);	/* $15EE48 asm */
void Map_PutColBlockA(u16 *defs, unsigned short cell, short c, short from, short to, short *vramRow, short vramCol);	/* $15EF90 asm regs(a2,d2,d1,d4,d3,&d6,d7) clobbers(d3,d4) */
void Map_PutColBlockB(u16 *defs, unsigned short cell, short c, short from, short to, short *vramRow, short vramCol);	/* $15F036 asm regs(a2,d2,d1,d4,d3,&d6,d7) clobbers(d3,d4) */
void Map_DrawColumnA(s16 *map, u16 *defs, short tileX, short tileY, short vramRow, short vramCol);	/* $15F0DC asm regs(a4,a2,d4,d5,d6,d7) clobbers(d3,a3) */
void Map_DrawColumnB(s16 *map, u16 *defs, short tileX, short tileY, short vramRow, short vramCol);	/* $15F16A asm regs(a4,a2,d4,d5,d6,d7) clobbers(d3,a3) */
void Map_StreamColumns(void);	/* $15F1F8 asm */
void Nem_Decode(void *src, NemCodeTable *table, long rows, char mode, long dest);	/* $15F37C */
void Nem_DecodeLoop(u8 *src, u16 *codes, short rows, char mode, u32 *dest, u8 *mask, unsigned short bits, short nbits, short nibbles, unsigned long row, unsigned long *fill);	/* $15F3F0 asm conv clobbers(d0,d1,d3,d4,d5,d6,a0,a3,a4) */
void Nem_DecodeLoop_Row(void);	/* $15F4BA fragment */
void Nem_DecodeLoop_Inline(void);	/* $15F540 fragment */
char Nem_PutRowVdp(unsigned long row, short *rowsLeft);	/* $15F566 asm conv */
char Nem_PutRowRam(unsigned long row, short *rowsLeft, u32 **dest);	/* $15F572 asm conv */
char Nem_PutRowMasked(unsigned long row, short *rowsLeft, char *skip, u8 *mask);	/* $15F57E asm conv */
u8 *Nem_BuildCodeTable(void *list, void *table);	/* $15F5CA asm */
void Eni_Decode(void *, void *, void *);	/* $15F5FC */
void Eni_Ops(short op, short count, u8 *src, unsigned long bits, short nbits, u16 *dest, short inc, short common, short flags, short base, short width);	/* $15F65C asm conv clobbers(d0,d1,d2,a0,a1) */
unsigned short Eni_ReadInline(u8 **src, unsigned long *bits, short *nbits, short width, short flags, short base, short *shift);	/* $15F6F0 asm regs(&a0,&d5,&d6,d4,d3,a3,&d7)->d1 */
void Anim_Advance(void *);	/* $15F740 */
char Anim_ApplyStep(void *, char);	/* $15F78E */
long Anim_AdvanceByDistance(void *, short, long, long);	/* $15F8B6 */
void Spawn_Nop(void);	/* $15F954 */
void Spawn_InitRecords(void);	/* $15F95C */
void VDP_WriteTileRect(long vram, s16 *src, long w, long h, long stride);	/* $15FA2C */
void Intro_SegaLogoFlash(long, short);	/* $15FA7A */
void Intro_WaitFrame(IntroFrameFn cb);	/* $15FBAE asm regs(@8(a6)) */
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
Actor *Obj_GetLiftCarrier(void *obj);	/* $1606F4 */
long Cam_GetSize(void);	/* $1607A0 */
Camera *Cam_New(Class *cls, CamFn hook);	/* $1607AA */
void Cam_Init(Camera *self);	/* $1607E8 */
void Cam_Nop12(Camera *self);	/* $160996 */
void Cam_Update(Camera *self);	/* $1609A8 */
void Cam_FollowBoth(Camera *self, short left, short top, short right, short bottom);	/* $1609CC */
void Cam_FollowPlayer(Camera *self, short left, short top, short right, short bottom);	/* $160A18 */
void Cam_FollowObject(Camera *self, void *obj, short left, short top, short right, short bottom);	/* $160ACA */
void Cam_EaseOffset(Camera *self);	/* $160B0E */
void Cam_ShakeStep(Camera *self);	/* $160BB0 */
void Cam_StepLookDown(Camera *self);	/* $160C3E */
void Cam_StepLookUp(Camera *self);	/* $160D06 */
char Cam_LookUp(Camera *self);	/* $160DCE */
char Cam_LookDown(Camera *self);	/* $160E0E */
void Cam_EndLookUp(Camera *self);	/* $160E4E */
void Cam_EndLookDown(Camera *self);	/* $160E60 */
void Cam_PanStep(Camera *self);	/* $160E72 */
void Cam_PanTo(Camera *self, short x, short y, short speedX, short speedY, char lockX, char lockY);	/* $160F36 */
void Cam_UnlockAxes(Camera *self);	/* $160FCC */
char Cam_IsPanning(Camera *self);	/* $160FDE */
void Cam_ApplyPos(Camera *self);	/* $160FFE */
void Cam_TrackBoth(Camera *self);	/* $16103C */
long Cam_EaseAxis(short ofs, short cam, short step, short target, short prev);	/* $16118E */
void Cam_SpringAxis(short target, short prev, short lo, short hi, short catchUp, short maxAccel, s16 *pos, s16 *speed, s16 *accel, s16 *accelStep);	/* $161222 */
void Cam_TrackPlayer(Camera *self);	/* $16136E */
void Cam_TrackObject(Camera *self);	/* $161744 */
void Cam_DefaultUpdate(Camera *self);	/* $161816 */
long Beast_GetSize(void);	/* $182AE6 */
void Beast_Jump(Beast *self);	/* $182AF4 */
void Beast_SetAirFrame(Beast *self);	/* $182B3C */
short Beast_WallJump(Beast *self);	/* $182BDE */
short Beast_StateGrabWall(Beast *self);	/* $182C74 */
short Beast_StateCling(Beast *self);	/* $182DC2 */
void Beast_DiveImpact(Beast *self);	/* $182F0C */
short Beast_StateDive(Beast *self);	/* $182FFA */
short Beast_StateDiveHold(Beast *self);	/* $1830BE */
short Beast_AirAttackDown(Beast *self);	/* $183136 */
short Beast_AirSpecial(Beast *self);	/* $1831D0 */
void Beast_ShockwaveInit(short x, s16 *leftX, s16 *rightX, SprInst *spr, char strong);	/* $18323E */
char Beast_ShockwaveHitCb(Actor *target, short x, short y, short type);	/* $1832B6 */
char Beast_ShockwaveStep(s16 *px, s16 *py, SprInst *spr, char right, unsigned char priority, char hitsEnemies);	/* $183320 */
void Beast_UpdateShockwaves(Beast *self);	/* $183512 */
void Beast_StateStompEnd(Beast *self);	/* $1835C2 */
short Beast_StateStomp(Beast *self);	/* $183630 */
short Beast_GroundSpecial(Beast *self);	/* $1836C0 */
void Beast_SetFrameKeepCenter(Beast *self, short frame);	/* $183724 */
void Beast_SetFrameKeepCenterX(Beast *self, short frame);	/* $1837B0 */
void Beast_RollLand(Beast *self);	/* $183810 */
short Beast_StateRollRecover(Beast *self);	/* $183870 */
short Beast_StateRollEnd(Beast *self);	/* $1838B8 */
short Beast_StateRoll(Beast *self);	/* $18393C */
void Beast_RollProbeCb(Seg *seg, Actor *obj, s16 *contact);	/* $183A24 */
char Beast_RollPathClear(Beast *self);	/* $183A3E */
short Beast_StartRoll(Beast *self);	/* $183AB6 */
short Beast_StateCrouch(Beast *self);	/* $183BAE */
short Beast_Crouch(Beast *self);	/* $183E1A */
void Beast_StateUpAttack(Beast *self);	/* $183EB2 */
short Beast_GroundAttackUp(Beast *self);	/* $183F70 */
void Beast_Init(Beast *self, long x, long y, Placement *pl);	/* $183FFE */
void Beast_Attack(Beast *self, Actor *target, char type, char damage, short sx, short sy);	/* $1840EC */
void Beast_SetFrame(Beast *self, short frame);	/* $184158 */
void *Beast_SelectSpriteDef(void);	/* $1841E4 */
short Beast_SelectFrame(void);	/* $1841F4 */
long Wolverine_GetSize(void);	/* $196552 */
void Wolverine_SetAirFrame(Wolverine *self);	/* $196560 */
short Wolverine_WallToCeiling(Wolverine *self);	/* $1965D2 */
short Wolverine_CeilingToWall(Wolverine *self);	/* $196686 */
short Wolverine_StateWallClimb(Wolverine *self);	/* $19673A */
short Wolverine_StartCeilingCrawl(Wolverine *self);	/* $1968D2 */
short Wolverine_StateWallCling(Wolverine *self);	/* $1969C2 */
short Wolverine_StateCeilingCrawl(Wolverine *self);	/* $196C82 */
short Wolverine_StartWallClimb(Wolverine *self);	/* $196E64 */
short Wolverine_StateCeilingHang(Wolverine *self);	/* $196F70 */
short Wolverine_StateGrab(Wolverine *self);	/* $197116 */
short Wolverine_StateDive(Wolverine *self);	/* $1972B8 */
short Wolverine_StateDiveHold(Wolverine *self);	/* $197348 */
short Wolverine_AirAttackDown(Wolverine *self);	/* $197412 */
void Wolverine_StateLungeEnd(Wolverine *self);	/* $197498 */
short Wolverine_StateLunge(Wolverine *self);	/* $1974EE */
short Wolverine_AirSpecial(Wolverine *self);	/* $1975E6 */
void Wolverine_LeapSlashEnd(Wolverine *self);	/* $19767C */
short Wolverine_GroundSpecial(Wolverine *self);	/* $1976D2 */
short Wolverine_StateCrouch(Wolverine *self);	/* $19777E */
short Wolverine_Crouch(Wolverine *self);	/* $1979A4 */
void Wolverine_Update(Wolverine *self);	/* $197A3C */
void Wolverine_Init(Wolverine *self, long x, long y, Placement *pl);	/* $197ACA */
void Wolverine_SetFrame(Wolverine *self, short frame);	/* $197B96 */
void *Wolverine_SelectSpriteDef(void);	/* $197C22 */
short Wolverine_SelectFrame(void);	/* $197C32 */
void Wolverine_Reset(Wolverine *self);	/* $197C3C */
char Nightcrawler_FlashPalette(short step);	/* $1A60B6 */
long Nightcrawler_GetSize(void);	/* $1A61DE */
void Nightcrawler_Jump(Nightcrawler *self);	/* $1A61EC */
void Nightcrawler_DropThrough(Nightcrawler *self);	/* $1A621C */
void Nightcrawler_SetAirFrame(Nightcrawler *self);	/* $1A6248 */
short Nightcrawler_WallAttackEnd(Nightcrawler *self);	/* $1A62A2 */
short Nightcrawler_StateWallClimb(Nightcrawler *self);	/* $1A62D6 */
short Nightcrawler_StateWaitUpRelease(Nightcrawler *self);	/* $1A6410 */
short Nightcrawler_StateWaitLeftRelease(Nightcrawler *self);	/* $1A646A */
short Nightcrawler_StateWaitRightRelease(Nightcrawler *self);	/* $1A64C4 */
short Nightcrawler_WallToCeiling(Nightcrawler *self);	/* $1A651E */
short Nightcrawler_StateWallCling(Nightcrawler *self);	/* $1A65EC */
void Nightcrawler_StateWallCling_Ret(void);	/* $1A6A52 fragment */
short Nightcrawler_StateCeilingCrawl(Nightcrawler *self);	/* $1A6A5C */
short Nightcrawler_CeilingToWall(Nightcrawler *self);	/* $1A6B36 */
short Nightcrawler_StateCeilingHang(Nightcrawler *self);	/* $1A6C48 */
short Nightcrawler_StateGrab(Nightcrawler *self);	/* $1A6FDC */
short Nightcrawler_StateDive(Nightcrawler *self);	/* $1A714C */
short Nightcrawler_StateDiveHold(Nightcrawler *self);	/* $1A71EE */
short Nightcrawler_AirAttackDown(Nightcrawler *self);	/* $1A72C6 */
short Nightcrawler_AirAttack(Nightcrawler *self);	/* $1A73B2 */
void Nightcrawler_SetTeleportVelocity(Nightcrawler *self);	/* $1A74A2 */
short Nightcrawler_StartTeleport(Nightcrawler *self);	/* $1A757E */
short Nightcrawler_TeleportVanish(Nightcrawler *self);	/* $1A76E6 */
void Nightcrawler_TeleportDown(Nightcrawler *self);	/* $1A7730 */
void Nightcrawler_StateTeleport(Nightcrawler *self);	/* $1A775E */
void Nightcrawler_TeleportDone(Nightcrawler *self);	/* $1A77F0 */
short Nightcrawler_StateTeleportFlash(Nightcrawler *self);	/* $1A7848 */
short Nightcrawler_TeleportAppear(Nightcrawler *self);	/* $1A78BC */
void Nightcrawler_GroundSpecial(Nightcrawler *self);	/* $1A7962 */
void Nightcrawler_AirSpecial(Nightcrawler *self);	/* $1A7984 */
void Nightcrawler_Attack(Nightcrawler *self, Actor *target, char type, char damage, short sx, short sy);	/* $1A79A6 */
void Nightcrawler_UpdateCharge(Nightcrawler *self);	/* $1A7A86 */
void Nightcrawler_Init(Nightcrawler *self, long x, long y, Placement *place);	/* $1A7B10 */
void Nightcrawler_SetFrame(Nightcrawler *self, short frame);	/* $1A7BFE */
void Nightcrawler_Reset(Nightcrawler *self);	/* $1A7CA4 */
void *Nightcrawler_SelectSpriteDef(void);	/* $1A7CAC */
short Nightcrawler_SelectFrame(void);	/* $1A7CBC */
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
void Magneto_HoverDamp(Magneto *self, char stopSlow, char face);	/* $1CDA48 */
void Magneto_AirSteer(Magneto *self);	/* $1CDB32 */
void Magneto_SetAirFrame(Magneto *self);	/* $1CDCE8 */
short Magneto_StateAirShotEnd(Magneto *self);	/* $1CDDA4 */
short Magneto_StateAirShot(Magneto *self);	/* $1CDEB2 */
void Magneto_StartAirShot(Magneto *self);	/* $1CDFD2 */
void Magneto_AirAttackUp(Magneto *self);	/* $1CE05E */
void Magneto_AirAttack(Magneto *self);	/* $1CE0F2 */
void Magneto_AirAttackDown(Magneto *self);	/* $1CE19A */
void Magneto_FireShot(Magneto *self);	/* $1CE22C */
short Magneto_StartShot(Magneto *self);	/* $1CE2CE */
short Magneto_StateAim(Magneto *self);	/* $1CE34E */
void Magneto_GroundAttack(Magneto *self);	/* $1CE618 */
void Magneto_GroundAttackUp(Magneto *self);	/* $1CE646 */
short Magneto_StateCrouchAim(Magneto *self);	/* $1CE674 */
void Magneto_GroundAttackDown(Magneto *self);	/* $1CE73E */
short Magneto_StateHoldBomb(Magneto *self);	/* $1CE778 */
short Magneto_SpawnBomb(Magneto *self);	/* $1CE826 */
short Magneto_StartBomb(Magneto *self);	/* $1CE902 */
void Magneto_GroundSpecial(Magneto *self);	/* $1CE982 */
void Magneto_GroundSpecialUp(Magneto *self);	/* $1CEA2E */
void Magneto_GroundSpecialDown(Magneto *self);	/* $1CEADA */
void Magneto_Init(Magneto *self, long x, long y, Placement *pl);	/* $1CEB18 */
void Magneto_TakeDamage(Magneto *self, Actor *attacker, char type, char damage, short hx, short hy);	/* $1CEBCC */
void *Magneto_SelectSpriteDef(void);	/* $1CEC28 */
short Magneto_SelectFrame(void);	/* $1CEC38 */
void Magneto_Reset(Magneto *self);	/* $1CEC42 */
long Gambit_GetSize(void);	/* $1CEF6E */
void Gambit_SetAirFrame(Gambit *self);	/* $1CEF7C */
char Gambit_CardHitEnemy(Actor *target, short x, short y, void *data);	/* $1CF070 */
char Gambit_CardCollide(GambitCard *card);	/* $1CF0E2 */
GambitCard *Gambit_FindFreeCard(Gambit *self);	/* $1CF1B6 */
GambitCard *Gambit_SpawnCard(Gambit *self, char kind, char damage, short dx, short dy, short angle, short speed, char up);	/* $1CF1EC */
char Gambit_PointNearBody(Gambit *self, GambitCard *card, short x, short y);	/* $1CF36E */
void Gambit_PointNearBody_Type0(void);	/* $1CF3AE fragment */
void Gambit_PointNearBody_Type1(void);	/* $1CF3C0 fragment */
void Gambit_PointNearBody_Test(void);	/* $1CF3E2 fragment */
void Gambit_UpdateCard(Gambit *self, GambitCard *card);	/* $1CF432 */
void Gambit_UpdateCard_TrailLoop(void);	/* $1CF74E fragment */
void Gambit_UpdateCard_TrailNext(void);	/* $1CF7E6 fragment */
void Gambit_AfterState(Gambit *self);	/* $1CF7FA */
void Gambit_ThrowCards(Gambit *self);	/* $1CF84A */
void Gambit_StateAirThrowEnd(Gambit *self);	/* $1CFA48 */
short Gambit_AirThrow(Gambit *self);	/* $1CFA90 */
void Gambit_GroundThrow(Gambit *self);	/* $1CFAD8 */
short Gambit_StateThrow(Gambit *self);	/* $1CFB14 */
short Gambit_StateGroundAim(Gambit *self);	/* $1CFB56 */
void Gambit_AirThrowAim(Gambit *self);	/* $1CFE26 */
void Gambit_StateAirThrowBrake(Gambit *self);	/* $1CFF7E */
void Gambit_StateAirCharge(Gambit *self);	/* $1CFFEA */
short Gambit_StateCrouchAim(Gambit *self);	/* $1D0058 */
void Gambit_GroundAim(Gambit *self);	/* $1D011E */
void Gambit_CrouchAim(Gambit *self);	/* $1D0148 */
void Gambit_AirCharge(Gambit *self);	/* $1D0182 */
void Gambit_GroundSpecial(Gambit *self);	/* $1D01A4 */
void Gambit_GroundSpecialUp(Gambit *self);	/* $1D01C6 */
void Gambit_GroundSpecialDown(Gambit *self);	/* $1D01E8 */
void Gambit_AirSpecial(Gambit *self);	/* $1D020A */
void Gambit_AirSpecialUp(Gambit *self);	/* $1D022C */
void Gambit_AirSpecialDown(Gambit *self);	/* $1D024E */
void Gambit_ClearCards(Gambit *self);	/* $1D0270 */
void Gambit_Init(Gambit *self, long x, long y, Placement *pl);	/* $1D029E */
void Gambit_SetFrame(Gambit *self, short frame);	/* $1D041E */
short Gambit_StateHardLanding(Gambit *self);	/* $1D04C4 */
void Gambit_HardLanding(Gambit *self);	/* $1D052A */
void Gambit_StateAirKick(Gambit *self);	/* $1D0560 */
short Gambit_AirKick(Gambit *self);	/* $1D059C */
short Gambit_AirAttack(Gambit *self);	/* $1D06AE */
void Gambit_Nop6A(void);	/* $1D073C */
void *Gambit_SelectSpriteDef(void);	/* $1D0744 */
short Gambit_SelectFrame(void);	/* $1D0754 */
void Cyclops_ResetBeamFlicker(void);	/* $1E15F4 asm */
void Cyclops_DrawBeam(Cyclops *self, CyclopsBeam *beam);	/* $1E1604 */
void Cyclops_DrawBeam_StepLevel(void);	/* $1E16BC fragment */
void Cyclops_DrawBeam_StepDown(void);	/* $1E16CC fragment */
void Cyclops_DrawBeam_StepDiag(void);	/* $1E16D4 fragment */
void Cyclops_DrawBeam_CutVertical(void);	/* $1E1768 fragment */
void Cyclops_DrawBeam_CutBoth(void);	/* $1E176E fragment */
void Cyclops_DrawBeam_Emit(void);	/* $1E177A fragment */
void Cyclops_DrawBeam_None(void);	/* $1E1916 fragment */
void Cyclops_BeamVBlankFlicker(void);	/* $1E1928 asm */
long Cyclops_GetSize(void);	/* $1E2AB0 */
void Cyclops_SetAirFrame(Cyclops *self);	/* $1E2ABE */
void Cyclops_AirAttack(Cyclops *self);	/* $1E2B48 */
void Cyclops_AirAttackDown(Cyclops *self);	/* $1E2C1A */
void Cyclops_ResetBeams(void);	/* $1E2C3E */
void Cyclops_EndBeam(void *obj);	/* $1E2CBC */
void Cyclops_StartBeam(Cyclops *obj, void *rec, char vflip);	/* $1E2CFC */
char Cyclops_BeamTimeUp(Cyclops *self);	/* $1E2F24 */
void Cyclops_BeamHitEnemies(Cyclops *self);	/* $1E2F46 */
short Cyclops_StateAirBeam(Cyclops *self);	/* $1E31BA */
void Cyclops_StateGroundBeam(Cyclops *self);	/* $1E3214 */
void Cyclops_StateCrouchBeam(Cyclops *self);	/* $1E3246 */
short Cyclops_StateHardLanding(Cyclops *self);	/* $1E3278 */
void Cyclops_HardLanding(Cyclops *self);	/* $1E32EC */
void Cyclops_AirBeamAim(Cyclops *self);	/* $1E3322 */
void Cyclops_StateAirBeamBrake(Cyclops *self);	/* $1E34BA */
void Cyclops_StateAirCharge(Cyclops *self);	/* $1E3526 */
short Cyclops_StateGroundAim(Cyclops *self);	/* $1E3594 */
short Cyclops_StateCrouchAim(Cyclops *self);	/* $1E3892 */
void Cyclops_AirCharge(Cyclops *self);	/* $1E3966 */
void Cyclops_GroundAim(Cyclops *self);	/* $1E3988 */
void Cyclops_CrouchAim(Cyclops *self);	/* $1E39B8 */
void Cyclops_GroundSpecial(Cyclops *self);	/* $1E39F2 */
void Cyclops_GroundSpecialUp(Cyclops *self);	/* $1E3A14 */
void Cyclops_GroundSpecialDown(Cyclops *self);	/* $1E3A36 */
void Cyclops_AirSpecial(Cyclops *self);	/* $1E3A58 */
void Cyclops_AirSpecialUp(Cyclops *self);	/* $1E3A7A */
void Cyclops_AirSpecialDown(Cyclops *self);	/* $1E3A9C */
void Cyclops_Init(Cyclops *self, long x, long y, Placement *place);	/* $1E3ABE */
void Cyclops_Draw(Cyclops *self);	/* $1E3BB0 */
void Cyclops_TakeDamage(Cyclops *self, Actor *attacker, char type, char damage, short hx, short hy);	/* $1E3BF2 */
void Cyclops_Nop6A(Cyclops *self);	/* $1E3C52 */
void *Cyclops_SelectSpriteDef(void);	/* $1E3C5A */
short Cyclops_SelectFrame(void);	/* $1E3C6A */
char XPlayer_ActionPressed(XPlayer *self);	/* $1F005C */
long XPlayer_GetSize(void);	/* $1F00E8 */
void XPlayer_EnterAirFrame(XPlayer *self);	/* $1F00F6 */
void XPlayer_LeaveGround(XPlayer *self);	/* $1F0188 */
char XPlayer_CanDoubleJumpNo(void);	/* $1F01F4 */
short XPlayer_DoubleJumpNone(XPlayer *self);	/* $1F01FE */
char XPlayer_CheckGrabNo(void);	/* $1F0222 */
short XPlayer_StateClimbNone(XPlayer *self);	/* $1F022C */
short XPlayer_AirSpecial(XPlayer *self);	/* $1F0250 */
void XPlayer_AirSpecialUp(XPlayer *self);	/* $1F0280 */
void XPlayer_AirSpecialDown(XPlayer *self);	/* $1F02A4 */
short XPlayer_GroundSpecial(XPlayer *self);	/* $1F02C8 */
void XPlayer_GroundSpecialUp(XPlayer *self);	/* $1F02EC */
void XPlayer_GroundSpecialDown(XPlayer *self);	/* $1F0310 */
short XPlayer_StartDeath(XPlayer *self);	/* $1F0334 */
short XPlayer_StateDeathBlink(XPlayer *self);	/* $1F039A */
void XPlayer_DeathDone(XPlayer *self);	/* $1F0430 */
char XPlayer_PushingTetherEdge(XPlayer *self);	/* $1F049E */
void XPlayer_TetherClamp(XPlayer *self, s32 *coord, long value, long flag);	/* $1F0542 */
char XPlayer_TetherToPartner(XPlayer *self);	/* $1F0582 */
short XPlayer_PlaceAtSafePos(XPlayer *self);	/* $1F074E */
char XPlayer_CanRejoinAt(XPlayer *self, XPlayer *partner);	/* $1F07CA */
void XPlayer_StateRejoin(XPlayer *self);	/* $1F0834 */
short XPlayer_StateFallOffScreen(XPlayer *self);	/* $1F0930 */
short XPlayer_StartFallOffScreen(XPlayer *self);	/* $1F0998 */
void XPlayer_StateLookUp(XPlayer *self);	/* $1F09D6 */
short XPlayer_StartLookUp(XPlayer *self);	/* $1F0A48 */
short XPlayer_StateControl(XPlayer *self);	/* $1F0A94 */
void XPlayer_SteerFacing(XPlayer *self, short max, short accel, char right);	/* $1F1372 */
char Player_TopAboveSeg(void *player, short margin, Seg *seg);	/* $1F13D0 */
char Player_BottomBelowSeg(void *player, short margin, Seg *seg);	/* $1F1424 */
char Player_FrontPastSeg(void *player, short margin, Seg *seg);	/* $1F1478 */
void XPlayer_SaveSafePos(XPlayer *self);	/* $1F1504 */
void XPlayer_Move(XPlayer *self, short step);	/* $1F153A */
void XPlayer_Update(XPlayer *self);	/* $1F1644 */
void Spr_UploadFrame18(SprInst *spr);	/* $1F196E */
void XPlayer_Draw(XPlayer *self);	/* $1F1A2E */
void XPlayer_Deactivate(XPlayer *self);	/* $1F1C06 */
void XPlayer_Activate(XPlayer *self);	/* $1F1CD4 */
void XPlayer_AddHealth(XPlayer *self, long unused, short units);	/* $1F1D88 */
char XPlayer_OutsideActiveWindow(void);	/* $1F1DE2 */
void XPlayer_TakeDamage(XPlayer *self, Actor *attacker, char type, char damage);	/* $1F1DEC */
void XPlayer_Attack(XPlayer *self, Actor *target, char type, short damage, short sx, short sy);	/* $1F1FEE */
void XPlayer_Nop72(void);	/* $1F20D0 */
void XPlayer_StateFrozen(XPlayer *self);	/* $1F20D8 */
void XPlayer_FreezeStanding(XPlayer *self);	/* $1F20EA */
void XPlayer_UpdateCharge(XPlayer *self);	/* $1F2126 */
void XPlayer_InitSpawnRec(SpawnRec *rec, Placement *place);	/* $1F21C0 */
void XPlayer_ClassInit(void);	/* $1F220E */
void XPlayer_LevelStart(void);	/* $1F2216 */
void XPlayer_Nop66(XPlayer *self);	/* $1F221E */
void XPlayer_Init(XPlayer *self, long x, long y, Placement *place);	/* $1F2234 */
void XPlayer_Jump(XPlayer *self);	/* $1F2306 */
void XPlayer_DropThrough(XPlayer *self);	/* $1F2376 */
void XPlayer_AirSteer(XPlayer *self);	/* $1F241C */
void XPlayer_Steer(XPlayer *self, short max, short accel, char right);	/* $1F2520 */
void XPlayer_StateFloatFall(XPlayer *self);	/* $1F2572 */
void XPlayer_StartFloatFall(XPlayer *self);	/* $1F26F2 */
void XPlayer_SetAirFrameNop(void);	/* $1F272E */
short XPlayer_StateAirAttack(XPlayer *self);	/* $1F2736 */
short XPlayer_AirAttackUp(XPlayer *self);	/* $1F27EA */
short XPlayer_AirAttack(XPlayer *self);	/* $1F285A */
short XPlayer_AirAttackDown(XPlayer *self);	/* $1F28CA */
char XPlayer_CanDoubleJump(XPlayer *self);	/* $1F293A */
void XPlayer_StateDoubleJump(XPlayer *self);	/* $1F2972 */
short XPlayer_DoubleJump(XPlayer *self);	/* $1F2AEA */
void XPlayer_AttackRecover(XPlayer *self);	/* $1F2B8E */
void XPlayer_CrouchAttackRecover(XPlayer *self);	/* $1F2C6E */
short XPlayer_AttackCombo2(XPlayer *self);	/* $1F2D74 */
short XPlayer_StateAttackCombo(XPlayer *self);	/* $1F2E56 */
short XPlayer_GroundAttack(XPlayer *self);	/* $1F2F0C */
short XPlayer_GroundAttackUp(XPlayer *self);	/* $1F2FAC */
short XPlayer_GroundAttackDown(XPlayer *self);	/* $1F3044 */
char XPlayer_Land(XPlayer *self);	/* $1F30DC */
void XPlayer_StateHardLanding(XPlayer *self);	/* $1F31F0 */
short XPlayer_HardLanding(XPlayer *self);	/* $1F3226 */
void XPlayer_WalkAccel(XPlayer *self, short max, short accel, char right);	/* $1F3282 */
void XPlayer_SetWalkAnim(XPlayer *self, void *slow, void *fast);	/* $1F32D0 */
void XPlayer_Walk(XPlayer *self);	/* $1F3342 */
short XPlayer_StateWalk(XPlayer *self);	/* $1F33D0 */
void XPlayer_StateCrouchLook(XPlayer *self);	/* $1F35DC */
short XPlayer_StateCrouch(XPlayer *self);	/* $1F364E */
void XPlayer_Crouch(XPlayer *self);	/* $1F37F2 */
short XPlayer_StateStand(XPlayer *self);	/* $1F383A */
short XPlayer_Stand(XPlayer *self);	/* $1F39C4 */
short XPlayer_StateHurtRecover(XPlayer *self);	/* $1F3ADE */
short XPlayer_StateHurt(XPlayer *self);	/* $1F3C7C */
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
char Player_IsClimber(void *player);	/* $1F41C2 */
char Player_IsNcOrWolverine(void *player);	/* $1F4208 */
char Player_IsBeastOrPsylocke(void *player);	/* $1F423A */
char Player_IsNcOrWolverine2(void *player);	/* $1F426C */
long Player_ClimbState(void *player);	/* $1F429E */
long Player_ClimbStateNcWolv(void *player);	/* $1F42D0 */
long Player_ClimbStateBeastPsy(void *player);	/* $1F4302 */
char Player_CeilingEndInView(void *player);	/* $1F4334 */
void EntryPoint(void);	/* $1F43AE fragment */
void Boot_RegionCheck(void);	/* $1F44A8 fragment */
void Boot_DrawText(char *s, char col, char row);	/* $1F45AE asm regs(a0,d0,d1,a4=$C00000,a5=$C00004) clobbers(d3) */
void Boot_StartApp(void);	/* $1F483E fragment */
void Irq_Enable(void);	/* $1F4854 asm */
void Irq_Disable(void);	/* $1F485A asm */
void Irq_VBlankVector(void);	/* $1F4860 fragment */
void Irq_HBlankVector(void);	/* $1F4870 fragment */
void *unk_Sys_1F4890(void);	/* $1F4890 asm regs()->a0 */
void Mem_Copy(void *dst, void *src, long n);	/* $1F48AA asm */
void *Mem_Set(void *dst, long c, long n);	/* $1F4900 asm */
Actor *Obj_FindFreeSlot(void);	/* $1F4A1A asm */
void Obj_UpdateBoxes(void *);	/* $1F4A40 asm */
void Obj_UpdateBox(void *);	/* $1F4BA2 asm */
void Actor_Move(void *obj);	/* $1F4C56 asm */
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
void Cam_SetMinXBlocks(short col);	/* $1F5F40 */
void Cam_SetMaxXBlocks(short col);	/* $1F5F5A */
void Cam_SetPos(short x, short y);	/* $1F5F78 */
void Cam_SetPosScaled(short x, short y);	/* $1F60E4 */
long Math_MulFix8(short, short);	/* $1F6258 */
void Obj_SetAnim(void *obj, void *anim);	/* $1F626A */
void Obj_ChangeAnim(void *obj, void *anim);	/* $1F6296 */
short SprDef_Add(void *def, short frame);	/* $1F62C0 */
void Obj_SetSpriteDef(Actor *self, unsigned char n);	/* $1F6334 */
char Seg_WouldLeaveEnd(void *obj, short dx);	/* $1F638A */
char Seg_AtOpenEndAhead(void *obj, short margin);	/* $1F6420 */
void Obj_AttachChild(void *p, void *c);	/* $1F64B4 */
void Obj_Detach(void *obj);	/* $1F6530 */
void Obj_ForEachChild(void *obj, ObjChildFn func);	/* $1F669A */
void Actor_DeactivateFree(void *obj);	/* $1F66F0 */
void Obj_ForEachChildCall(Actor *parent, ObjChildFn func);	/* $1F6774 */
void Pal_StartFadeDelay(short, short, short, long);	/* $1F67C8 */
void Pal_SaveBase(void);	/* $1F6816 */
void Pal_RestoreBase(void);	/* $1F684E */
void Pal_ClearBase(void);	/* $1F6886 */
void Pal_SaveCycle(void);	/* $1F68B2 */
void Pal_RestoreCycle(void);	/* $1F68C0 */
void Pal_FadeOutWait(long);	/* $1F68CE */
void Pal_FadeInWait(long);	/* $1F690E */
XPlayer *Player_Nearest(Actor *self);	/* $1F694A */
Actor *Player_NearestLive(void *obj);	/* $1F6A32 */
void Obj_StandOnSeg(void *obj, Seg *seg);	/* $1F6B56 */
void Obj_StandOnSegHere(void *obj);	/* $1F6B9C */
void Seg_ClearAll(void);	/* $1F6BD2 */
void Seg_LoadList(SegListEntry *list, char append);	/* $1F6C04 */
void Seg_ApplyObjPos(void *obj);	/* $1F6D76 */
void Seg_ApplyObjPos_Cases(void);	/* $1F6E42 fragment */
void Seg_ApplyObjPos_Store(void);	/* $1F6EA6 fragment */
void Seg_AlignObjVelocity(void *obj);	/* $1F6EC4 */
void Seg_AlignObjVelocity_Slope(void);	/* $1F6F80 fragment */
void Seg_AlignObjVelocity_Reverse(void);	/* $1F6F92 fragment */
void Seg_AlignObjVelocity_Flat(void);	/* $1F6FD6 fragment */
void Seg_AlignObjVelocity_Store(void);	/* $1F6FDA fragment */
void Seg_ComputeObjPos(Seg *seg, void *obj);	/* $1F6FF4 */
void Seg_ComputeObjPos_Cases(void);	/* $1F70A4 fragment */
void Seg_ComputeObjPos_Done(void);	/* $1F71E8 fragment */
void Seg_ObjPosUnclamped(Seg *seg, void *obj);	/* $1F71F2 */
void Seg_ObjPosUnclamped_Horiz(void);	/* $1F72A0 fragment */
void Seg_ObjPosUnclamped_Reverse(void);	/* $1F72B6 fragment */
void Seg_ObjPosUnclamped_Done(void);	/* $1F72F6 fragment */
short Seg_CollideBlock(void *obj, Seg *seg, SegContact *c);	/* $1F7300 */
void Seg_ApplyPushOut(Seg *seg, void *obj, SegContact *c);	/* $1F7628 */
void Seg_ApplyPushOut_Block(void);	/* $1F7686 fragment */
void Seg_ApplyPushOut_Default(void);	/* $1F76D6 fragment */
void Seg_ApplyPushOut_Walls(void);	/* $1F7848 fragment */
void Seg_ApplyPushOut_Move(void);	/* $1F78CA fragment */
void Seg_SetLayerFromObj(void *obj, Seg *seg);	/* $1F7940 */
void Obj_InheritLayer(Actor *parent, Actor *o);	/* $1F7956 */
void Seg_ComputeLength(Seg *seg);	/* $1F79A2 */
void Seg_ComputeLength_Flat(void);	/* $1F7A7C fragment */
void Seg_ComputeLength_Max(void);	/* $1F7A82 fragment */
void Seg_InitDefaults(Seg *seg, char type, char layer);	/* $1F7A9E */
void Seg_ComputeSlope(Seg *seg);	/* $1F7AF8 */
void Seg_ComputeSlope_Flat(void);	/* $1F7B36 fragment */
void Seg_ComputeSlope_Calc(void);	/* $1F7B3C fragment */
void Seg_Setup(Seg *seg, short type, short layer, short x1, short y1, short x2, short y2);	/* $1F7BB6 */
short Seg_Alloc(void);	/* $1F7C0A */
Seg *Seg_Create(char type, short layer, short x1, short y1, short x2, short y2);	/* $1F7C6A */
void Seg_Free(Seg *seg);	/* $1F7CFE */
void Seg_ForEachOfObj(void *obj, SegObjFn func);	/* $1F7D5E */
void Obj_SnapToNearestSeg(void *obj, short type, short pos);	/* $1F7DAE */
Seg *Obj_SnapToNearestFloor(void *obj, short type, short pos);	/* $1F7F38 */
Seg *Seg_FindNearestOfType(short x, short y, short type);	/* $1F80CA */
Seg *Seg_FindContaining(short x, short y, short type);	/* $1F82CC */
Seg *Seg_FindInBlock(short type, short col, short row);	/* $1F8454 */
void Seg_SetEndpoints(void *seg, short x1, short y1, short x2, short y2);	/* $1F8494 asm */
void Seg_BuildGrid(void);	/* $1F84BE */
void Seg_GatherNearObj(void *obj, SegContact *c);	/* $1F85B6 */
short Seg_CollideObj(Seg *seg, void *obj, SegContact *c);	/* $1F87B2 */
void Seg_CollideObj_Cases(void);	/* $1F883A fragment */
void Seg_CollideObj_Trigger(void);	/* $1F895C fragment */
void Seg_CollideObj_CeilFloor(void);	/* $1F89A4 fragment */
void Seg_CollideObj_RetNone(void);	/* $1F8ACA fragment */
void Seg_CollideObj_RetHit(void);	/* $1F8AD6 fragment */
void Seg_CollideObj_Ret(void);	/* $1F8AEE fragment */
void Obj_CollideSegs(void *obj);	/* $1F8B42 */
long DrawList_Init(void);	/* $1F8EF6 */
long DrawList_AllocNode(void);	/* $1F8F4C */
short DrawList_Add(AnyFn draw, void *obj, short, char, short, char);	/* $1F8FA2 */
char DrawList_Remove(short);	/* $1F9122 */
char DrawList_SetPriority(short, short, char, short);	/* $1F91E8 */
void DrawList_DrawAll(void);	/* $1F9422 */
void Scene_EndAfter(short n);	/* $1F944E */
void Sys_Nop1F945C(short x, short y);	/* $1F945C */
unsigned char Scene_Register(char *name, SceneFn run);	/* $1F9464 */
void Sys_Nop1F949E(void);	/* $1F949E */
long Game_TickFrameCounter(void);	/* $1F94A6 */
void Scene_ClearActivateHook(void);	/* $1F94B6 */
char Obj_FindHitPoint(Actor *att, Actor *tgt, s8 *pts, short ox, short oy, s16 *rx, s16 *ry);	/* $1F94C4 */
void Music_Play(short);	/* $1F97A4 */
long Pad_P1StartDown(void);	/* $1F97DE */
void ClassList_CallSlot5A(void);	/* $1F9804 */
void Sys_InitGameVideo(void);	/* $1F9850 */
void Level_Load(void *fgMap, void *fgTiles, void *bgMap, void *bgTiles, void *fgSegs, void *bgSegs, char flag0, char flag1, char flag2, char flag3, short classArg, short music, short gridShiftX, short gridShiftY, char spawnFromBg, void *classList);	/* $1F98A8 */
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
short Spr_PreloadTiles(SprInst *spr, short tile);	/* $1FAA08 */
short Spr_PreloadToHeap(SprInst *spr, short tile);	/* $1FAD08 */
void Spr_UploadFrame(SprInst *spr, char now);	/* $1FAD44 */
short Spr_Init(SprInst *spr, long unused, void *def, short frame, short pal, short tile, char upload);	/* $1FB026 */
void Spr_InvalidateFrame(SprInst *spr);	/* $1FB126 */
void Spr_SetFrame(SprInst *spr, short frame);	/* $1FB142 */
void Obj_ShiftTree(Actor *parent, Actor *o);	/* $1FB178 */
void Spr_PlaceAttached(SprInst *spr, short x, short y);	/* $1FB1C2 */
void Spr_Draw(SprInst *spr, short x, short y);	/* $1FB346 */
char Spr_EmitPreLinked(SprInst *spr, long x, long y, short tile);	/* $1FB592 */
void Spr_EmitPreLinked_Piece(void);	/* $1FB6D6 fragment */
void Spr_EmitPre(SprInst *spr, long x, long y, short tile);	/* $1FB75A */
void Spr_EmitPre_Piece(void);	/* $1FB874 fragment */
void Spr_EmitPreClip(SprInst *spr, long x, long y, short tile);	/* $1FB8A2 */
void Spr_EmitPreClip_Piece(void);	/* $1FB9B8 fragment */
void Spr_EmitStream(SprInst *spr, long x, long y, short tile);	/* $1FBA0C */
void Spr_EmitStream_Piece(void);	/* $1FBB2E fragment */
void Spr_EmitStreamClip(SprInst *spr, long x, long y, short tile);	/* $1FBB5E */
void Spr_EmitStreamClip_Piece(void);	/* $1FBC76 fragment */
char Spr_EmitStreamLinked(SprInst *spr, long x, long y, short tile);	/* $1FBCCE */
void Spr_EmitStreamLinked_Piece(void);	/* $1FBE1A fragment */
long Art_LoadToVramRecolor(void *, short, short, short, short);	/* $1FBE96 */
void Scaler_ExpandSource(void);	/* $1FC1CC asm */
void Scaler_BuildStepTables(long *pair, short *count);	/* $1FC1FA asm regs(&d1,&d7) clobbers(d3,a4) */
void Scaler_ClearRows(short *rows, u8 **dst);	/* $1FC2A6 asm regs(&d7,&a1) clobbers(d5,d6,a3,a4) */
void Scaler_InitParams(void);	/* $1FC2E8 asm */
void Spr_RenderToBitmap(SprDef *def, short frame, u8 *buf);	/* $1FC35C */
void Spr_RenderToBitmap_Loop(void);	/* $1FC466 fragment */
ScaleParams *Scaler_Start(ScaleParams *params, void *srcA, void *srcB, void *buf, short vram);	/* $1FC5FE */
void Scaler_Step(void);	/* $1FC6CA asm */
long Scaler_Resume(void);	/* $1FC6F6 asm regs()->d0 clobbers(d3,d4,d5,d6,a2,a3,a4) */
long Scaler_BeginPass(void);	/* $1FC708 asm regs()->d0 clobbers(d3,d4,d5,d6,a2,a3,a4) */
long Scaler_RowBlocks(ScalerRegs *r, short at);	/* $1FC8FE asm conv clobbers(d1,d2,d3,d4,d5,d6,a0,a1,a2,a3,a4) */
void Scaler_RowBlocksLast80(void);	/* $1FCE9E fragment */
void Scaler_RowEnd(void);	/* $1FD43E fragment */
long Scaler_EmitSprites(short, short, short, char);	/* $1FD520 */
void Scaler_EmitSprites_Loop(void);	/* $1FD5A0 fragment */
long Pad_ReadPort(short port);	/* $1FD61E asm regs(d0)->d0 */
void VDP_ResetDmaQueue(void);	/* $1FD720 */
long Art_LoadNemToVram(void *, short);	/* $1FD72C */
void Pal_ApplyFade(void *, void *);	/* $1FD77E */
void Pal_Update(void);	/* $1FD8D2 */
void Pal_StartFade(short level, short line, void *table);	/* $1FD998 */
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
void VDP_WriteWords(void *src, long n);	/* $1FE002 asm */
short Pad_GetP1Clean(void);	/* $1FE018 asm */
short Pad_GetP2Clean(void);	/* $1FE01E asm */
void Pad_ReadBoth(void);	/* $1FE03C asm */
short Pad_IsP2Connected(void);	/* $1FE052 asm */
void unk_VDP_1FE05C(void);	/* $1FE05C asm */
void VDP_DmaToVram(long src, long dest, long len);	/* $1FE0EC asm */
void VDP_QueueDma(void *src, long dest, long len);	/* $1FE17A asm */
void VDP_FlushDmaQueue(void);	/* $1FE28C asm */
void VDP_DmaPalette(s16 *pal);	/* $1FE316 asm */
void Z80_BusRequest(void);	/* $1FE3D8 asm */
void Z80_BusFree(void);	/* $1FE3EC asm */
void Z80_DriverPause(void);	/* $1FE3F6 asm */
void Z80_DriverResume(void);	/* $1FE434 asm */
void Z80_LoadDriver(void);	/* $1FE44E asm */
void Z80_ResetRun(void);	/* $1FE48E asm */
short Z80_BusAcquire(unsigned short *sr);	/* $1FE4B6 asm conv */
void Z80_BusRelease(unsigned short sr);	/* $1FE4EC asm conv */
short Snd_PutCmd(char cmd, short pos);	/* $1FE4FE asm regs(d0,d1,a0=$A00036,a1=$A01B40)->d1 */
short Snd_PutByte(char b, short pos);	/* $1FE50A asm regs(d0,d1,a0=$A00036,a1=$A01B40)->d1 */
void Snd_SendByte(long b);	/* $1FE518 asm */
void Snd_SendAddr24(long v);	/* $1FE528 asm */
void Snd_Init(long, long, long, long);	/* $1FE544 */
void Snd_Play(long id);	/* $1FE584 asm */
void Snd_Stop(long id);	/* $1FE5A2 asm */
void unk_Snd_1FE5B4(long arg);	/* $1FE5B4 asm */
void Snd_Pause(void);	/* $1FE5C6 asm */
void Snd_Resume(void);	/* $1FE5DC asm */
void Snd_StopAll(void);	/* $1FE5F2 asm */
void Snd_SendCmd2(void);	/* $1FE618 fragment */
void Snd_SetParam(long slot, long value);	/* $1FE630 asm */
char Snd_ReadDriverByte(long n);	/* $1FE644 asm */

#endif /* XMEN2_FUNCS_H */
