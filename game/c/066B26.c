/* Cerebro_Run - run a Cerebro screen, as its scene asked (g_SceneVar3B0C intro, g_SceneVar3B14
 * text script, g_SceneVar3B0A player select).
 *
 * The Cerebro level loads (Level_Load: maps and tiles Lvl_CerebroFg / Lvl_CerebroBg, class
 * list Tbl_CerebroClassList), no HUD, a fixed background, the sprite set, the frame hook
 * (Cerebro_FrameHook), the fonts, the palette animations reset, the text window rows 3-$16,
 * columns 5-$22 (the cursor homed, swapped as in Cerebro_ClearTextWindow), the frame counters
 * cleared, the display on and the window plane off, and the palette fades in. FinalText clears
 * the whole plane and uses a window 2 cells inside it (and music $A instead of 9); every screen but
 * Failure clears the screen. Every sound stops first unless the scene is the Credits (no music).
 * Intro: after the fade, Cerebro_PlayIntro; with text or a player select to follow, palette lines
 * 1-3 black, the screen and window cleared, line 1 = Pal_SceneDefaultLine1, the fonts again.
 * Text: its music (restarted, every sound stopped first), the text colours of line 3, then
 * Cerebro_RunTextScript. Player select: music 8, the window cleared, line 3's colours, the
 * players' characters (Player_CharIndexOf of their class, -1 out of play: g_P1Health /
 * g_P2Health 0) into g_P1CharIndex / g_P2CharIndex, both players not present, PlayerSelect_Run,
 * every sound stopped. Then the palette fades out (unless a fade is running), one more frame,
 * and the players' health comes back from g_P1Health / g_P2Health. */
#include "xmen2.h"

#define g_CerebroIntro		(*(s16 *)g_SceneVar3B0C)	/* scene variable: play the intro first */
#define g_CerebroSelect		(*(s16 *)g_SceneVar3B0A)	/* scene variable: run the player select after the text */
#define g_CerebroScript		(*(PTR(void) *)g_SceneVar3B14)	/* scene variable: the text script (0: none) */
#define g_TextCol		(*(s16 *)g_SceneVar3B04)	/* scene variable: Cerebro text cursor column */
#define g_TextRow		(*(s16 *)g_SceneVar3B06)	/* scene variable: Cerebro text cursor row */
#define g_PalAnim1Timer		(*(s16 *)g_SceneVar3B34)	/* scene variables: two palette animations nothing here runs */
#define g_PalAnim1Stage		(*(s16 *)g_SceneVar3B3C)
#define g_PalAnim1Dir		(*(s16 *)g_SceneVar3B44)
#define g_PalAnim2Timer		(*(s16 *)g_SceneVar3B36)
#define g_PalAnim2Stage		(*(s16 *)g_SceneVar3B3E)
#define g_PalAnim2Dir		(*(s16 *)g_SceneVar3B46)
#define g_GlowTimer		(*(s16 *)g_SceneVar3B38)	/* scene variable: Cerebro_AnimatePalettes' glow: frames to its next stage */
#define g_GlowStage		(*(s16 *)g_SceneVar3B40)	/* its stage 0-7 */
#define g_GlowDir		(*(s16 *)g_SceneVar3B48)	/* its direction (+1, -1) */
#if defined(XMEN2_NATIVE)
/* the screen's objects: 6, and with three or more players a cursor and a portrait for each player 3-4
   (PlayerSelect_Run: P34_SelectStart) */
#define CEREBRO_OBJECTS		(g_opt.players > 2 ? 6 + 2 * P34_COUNT : 6)
#else
#define CEREBRO_OBJECTS		6
#endif

void Cerebro_Run(void)
{
	register short music;
	register s16 *p;
	register s16 *q;

#if defined(XMEN2_NATIVE)
	plat_ws_cerebro(1);	/* phase 6 R5: a Cerebro screen (4:3), even under a stage's scene index */
#endif
	music = 9;
	if (g_ScnCredits == g_SceneIndex)
		music = -1;
	if (music != -1)
		Snd_StopAll();
	Level_Load(CODEPTR(Lvl_CerebroFg), CODEPTR(Lvl_CerebroFg), CODEPTR(Lvl_CerebroBg),
		CODEPTR(Lvl_CerebroBg), 0, 0, 0, 0, 1, 0, CEREBRO_OBJECTS, -1, 8, 8, 0, CODEPTR(Tbl_CerebroClassList));
	g_HudOn = 0;
	g_BgScrollFactorX = 0;
	g_BgScrollFactorY = 0;
	Cerebro_LoadSpriteTiles();
	g_SceneFrameHook = Cerebro_FrameHook;
	g_TwoPlayerStep = 0;
	Cerebro_LoadFonts();
	g_PalAnim1Timer = 2;
	g_PalAnim1Stage = 0;
	g_PalAnim1Dir = 1;
	g_PalAnim2Timer = 2;
	g_PalAnim2Stage = 0;
	g_PalAnim2Dir = 1;
	g_GlowTimer = 2;
	g_GlowStage = 0;
	g_GlowDir = 1;
	g_TextWinTop = 3;
	g_TextWinLeft = 5;
	g_TextWinBottom = 0x16;
	g_TextWinRight = 0x22;
	g_TextCol = g_TextWinTop;
	g_TextRow = g_TextWinLeft;
	g_SceneExitTimer = 0;
	g_VBlankCount = 0;
	g_DemoStepFrame = 0;
	g_LevelFrame = 0;
	g_SceneIndexSet = 0;
	g_LevelVramStart = g_VramTileCursor;
	Spr_InitLinks();
	Pad_Update();
	g_VdpReg1 |= 0x40;
	VDP_CTRL = g_VdpReg1;
	VDP_CTRL = 0x9100;
	VDP_CTRL = 0x929C;
	g_PalBaseDirty = 0;
	Pal_StartFade(0, -1, CODEPTR(Tbl_FadeBlack));
	if (g_ScnFinalText == g_SceneIndex) {
		g_TextWinTop = 0;
		g_TextWinLeft = 0;
		g_TextWinBottom = 0x1F;
		g_TextWinRight = 0x27;
		Cerebro_ClearTextWindow();
		g_TextWinTop += 2;
		g_TextWinLeft += 2;
		g_TextWinBottom -= 2;
		g_TextWinRight -= 2;
		music = 10;
	} else if (g_ScnFailure != g_SceneIndex)
		Cerebro_ClearScreen();
	if (g_CerebroIntro) {
		while (g_PalFadeActive)
			VDP_WaitVBlank();
		Cerebro_ClearScreen();
		Cerebro_PlayIntro();
		if (g_CerebroScript || g_CerebroSelect) {
			{
				register short i;

				for (i = 0, p = &g_PalBase[0x10]; i < 48; i++, p++)
					*p = 0;
			}
			g_PalBaseDirty = 1;
			g_DmaFlushReq = 1;
			VDP_WaitVBlank();
			Cerebro_ClearScreen();
			Cerebro_ClearTextWindow();
			p = (s16 *)CODEPTR(Pal_SceneDefaultLine1);
			q = &g_PalBase[0x10];
			{
				register long j;

				for (j = 0; j < 16; j++)
					*q++ = *p++;
			}
			Cerebro_LoadFonts();
		}
	}
	if (g_CerebroScript) {
		if (music != -1) {
			Snd_StopAll();
			Snd_Stop(music);
			Snd_Play(music);
		}
		while (g_PalFadeActive)
			VDP_WaitVBlank();
		g_PalBase[0x3F] = 0xA0;
		g_PalBase[0x3E] = 0x80;
		g_PalBase[0x3D] = 0x660;
		g_PalBase[0x3C] = 0x862;
		g_PalBase[0x3B] = 0xA0;
		g_PalBase[0x3A] = 0x80;
		g_PalBase[0x39] = 0xA;
		g_PalBase[0x38] = 8;
		g_PalBase[0x31] = 0;
		g_PalBaseDirty = 1;
		Cerebro_RunTextScript();
	}
	if (g_CerebroSelect) {
		register short c1;
		register short c2;

		Snd_StopAll();
		Snd_Stop(8);
		Snd_Play(8);
		while (g_PalFadeActive)
			VDP_WaitVBlank();
		Cerebro_ClearTextWindow();
		g_PalBase[0x3F] = 0xA0;
		g_PalBase[0x3E] = 0x80;
		g_PalBase[0x3D] = 0xA;
		g_PalBase[0x3C] = 8;
		g_PalBase[0x3B] = 0xA0;
		g_PalBase[0x3A] = 0x80;
		g_PalBase[0x39] = 0xA;
		g_PalBase[0x38] = 8;
		g_PalBase[0x31] = 0;
		g_PalBaseDirty = 1;
		c1 = -1;
		if (g_P1Health)
			c1 = Player_CharIndexOf(g_Player1.cls);
		c2 = -1;
		if (g_P2Health)
			c2 = Player_CharIndexOf(g_Player2.cls);
		g_P1CharIndex = c1;
		g_P2CharIndex = c2;
		g_Player1.flags = 0;
		g_Player2.flags = 0;
#if defined(XMEN2_NATIVE)
		if (g_opt.players > 2)
			P34_BeforeSelect();	/* phase 7: players 3 and 4 the same */
#endif
		PlayerSelect_Run();
		Snd_StopAll();
	}
	g_PalBaseDirty = 0;
	if (!g_PalFadeActive)
		Pal_StartFade(7, -1, CODEPTR(Tbl_FadeBlack));
	while (g_PalFadeActive)
		VDP_WaitVBlank();
	VDP_WaitVBlank();
	g_Player1.health = g_P1Health;
	g_Player2.health = g_P2Health;
#if defined(XMEN2_NATIVE)
	if (g_opt.players > 2)
		P34_AfterCerebro();
	plat_ws_cerebro(0);
#endif
}
