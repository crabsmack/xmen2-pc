/* Pause_HandleInput - one frame of input on the pause screen (Pause_Loop).
 *
 * Pad 1 button chords (A/B/C with directions, & $7F of the latch) use a cheat when it is
 * unlocked (g_ProgressFlags), else start entering its unlock code: g_PauseCheatPtr walks the
 * code's button sequence (the Alt table when pad 2 holds Left+Right, $C) and g_PauseCheatCode
 * remembers which one it is. Every new pad 1 press must be the sequence's next byte (a wrong
 * one drops the code); its $FF end unlocks the cheat with a sound (Pause_CheatUnlock).
 *   $21 health: both live players' health to $900     (code 1, flag $10)
 *   $22 lives (code 2 sets 99 lives each)              (code 2, flag $20)
 *   $24 / $28 previous / next scene                    (code 3, flags $40 / $80)
 *   $12 the players hurt each other (Player2_MakeHostile, both alive)  (code 5, flag $100)
 *   $14 / $18 previous / next stage group              (code 7, flag $400)
 *   $11 invincible: health $7FFF                       (code 8, flag $200)
 * Codes 4, 6 and 9 have no chord here (4 = code 3's flags, 6 sets $2000, 9 sets $27F0 and 99
 * lives). A scene change ends the pause (g_PauseExit) and the scene (g_SceneExitTimer 1). Start
 * on a pad of a live player ends the pause. */
#include "xmen2.h"

#define g_PauseCheatCode	g_TextWinTop	/* reused: the cheat whose code is being entered */

/* Take stage group g (its Cerebro scene) unless one was found: the first one before the current
   scene (previous) / after it (next), the groups tried nearest first. Statements on their own. */
#define PREV_GROUP(g)	if (!scene && (g) + 1 < g_SceneIndex) scene = (g)
#define NEXT_GROUP(g)	if (!scene && (g) > g_SceneIndex) scene = (g)

void Pause_HandleInput(void)
{
	g_FrameTick = 0;
	Pad_Update();
	if (g_PauseCheatPtr) {
		if (g_Pad1Pressed & 0x7F) {
			register unsigned char key = g_Pad1Pressed & 0x7F;

			if (*g_PauseCheatPtr == key) {
				++g_PauseCheatPtr;
				if (*g_PauseCheatPtr == 0xFF) {
					/* Pause_CheatUnlock: the code is complete */
					switch (g_PauseCheatCode) {
					case 1:
						Snd_Stop(0x1B);
						Snd_Play(0x1B);
						g_ProgressFlags |= 0x10;
						break;
					case 2:
						Snd_Stop(0x22);
						Snd_Play(0x22);
						g_ProgressFlags |= 0x20;
						g_P1Lives = 0x63;
						g_P2Lives = 0x63;
						break;
					case 3:
						Snd_Stop(0x36);
						Snd_Play(0x36);
						g_ProgressFlags |= 0xC0;
						break;
					case 4:
						Snd_Stop(0x36);
						Snd_Play(0x36);
						g_ProgressFlags |= 0xC0;
						break;
					case 5:
						Snd_Stop(0x37);
						Snd_Play(0x37);
						if (IS_ALIVE(&g_Player1) && IS_ALIVE(&g_Player2))
							Player2_MakeHostile();
						break;
					case 6:
						Snd_Stop(0x39);
						Snd_Play(0x39);
						g_ProgressFlags |= 0x2000;
						break;
					case 7:
						Snd_Stop(0x26);
						Snd_Play(0x26);
						g_ProgressFlags |= 0x400;
						break;
					case 8:
						Snd_Stop(0x24);
						Snd_Play(0x24);
						g_ProgressFlags |= 0x200;
						break;
					case 9:
						Snd_Stop(0x4F);
						Snd_Play(0x4F);
						g_P1Lives = 0x63;
						g_P2Lives = 0x63;
						g_ProgressFlags |= 0x27F0;
						break;
					}
					/* Pause_HandleInput_EndCode */
					g_PauseCheatCode = 0;
					g_PauseCheatPtr = 0;
				}
			} else {
				g_PauseCheatCode = 0;
				g_PauseCheatPtr = 0;
			}
		}
	} else {
		register short chord = g_Pad1Latch & 0x7F;

		if (chord) {
			if (chord == 0x21) {
				if (g_ProgressFlags & 0x10) {
					Snd_Stop(0x1B);
					Snd_Play(0x1B);
					if (g_Player1.health > 0)
						g_Player1.health = 0x900;
					if (g_Player2.health > 0)
						g_Player2.health = 0x900;
				} else {
					g_PauseCheatCode = 1;
					if (g_Pad2Raw == 0xC)
						g_PauseCheatPtr = (u8 *)CODEPTR(Tbl_CheatCodeHealthAlt);
					else
						g_PauseCheatPtr = (u8 *)CODEPTR(Tbl_CheatCodeHealth);
				}
			} else if (chord == 0x22) {
				if (!(g_ProgressFlags & 0x20)) {
					g_PauseCheatCode = 2;
					if (g_Pad2Raw == 0xC)
						g_PauseCheatPtr = (u8 *)CODEPTR(Tbl_CheatCodeLivesAlt);
					else
						g_PauseCheatPtr = (u8 *)CODEPTR(Tbl_CheatCodeLives);
				}
			} else if (chord == 0x24) {
				if (g_ProgressFlags & 0x40) {
					--g_SceneIndex;
					g_PauseExit = 1;
					g_SceneIndexSet = 1;
					g_SceneExitTimer = 1;
				} else {
					g_PauseCheatCode = 3;
					if (g_Pad2Raw == 0xC)
						g_PauseCheatPtr = (u8 *)CODEPTR(Tbl_CheatCodeSceneSkipAlt);
					else
						g_PauseCheatPtr = (u8 *)CODEPTR(Tbl_CheatCodeSceneSkip);
				}
			} else if (chord == 0x28) {
				if (g_ProgressFlags & 0x80) {
					++g_SceneIndex;
					g_PauseExit = 1;
					g_SceneExitTimer = 1;
				} else {
					g_PauseCheatCode = 3;
					if (g_Pad2Raw == 0xC)
						g_PauseCheatPtr = (u8 *)CODEPTR(Tbl_CheatCodeSceneSkipAlt);
					else
						g_PauseCheatPtr = (u8 *)CODEPTR(Tbl_CheatCodeSceneSkip);
				}
			} else if (chord == 0x12) {
				if (g_ProgressFlags & 0x100) {
					Snd_Stop(0x37);
					Snd_Play(0x37);
					if (IS_ALIVE(&g_Player1) && IS_ALIVE(&g_Player2))
						Player2_MakeHostile();
				} else {
					g_PauseCheatCode = 5;
					if (g_Pad2Raw == 0xC)
						g_PauseCheatPtr = (u8 *)CODEPTR(Tbl_CheatCodeFriendlyFireAlt);
					else
						g_PauseCheatPtr = (u8 *)CODEPTR(Tbl_CheatCodeFriendlyFire);
				}
			} else if (chord == 0x14) {
				if (g_ProgressFlags & 0x400) {
					register short scene = 0;

					Snd_Stop(0x26);
					Snd_Play(0x26);
					PREV_GROUP(g_ScnCerebroClone);
					PREV_GROUP(g_ScnCerebroSpace);
					PREV_GROUP(g_ScnCerebroSavage);
					PREV_GROUP(g_ScnCerebroBaniMaza);
					PREV_GROUP(g_ScnCerebroAvalon);
					PREV_GROUP(g_ScnCerebroSentinels);
					if (scene) {
						g_SceneIndex = scene;
						g_PauseExit = 1;
						g_SceneExitTimer = 1;
						g_SceneIndexSet = 1;
					}
				} else {
					g_PauseCheatCode = 7;
					if (g_Pad2Raw == 0xC)
						g_PauseCheatPtr = (u8 *)CODEPTR(Tbl_CheatCodeStageSkipAlt);
					else
						g_PauseCheatPtr = (u8 *)CODEPTR(Tbl_CheatCodeStageSkip);
				}
			} else if (chord == 0x18) {
				if (g_ProgressFlags & 0x400) {
					register short scene = 0;

					Snd_Stop(0x26);
					Snd_Play(0x26);
					NEXT_GROUP(g_ScnCerebroSentinels);
					NEXT_GROUP(g_ScnCerebroAvalon);
					NEXT_GROUP(g_ScnCerebroBaniMaza);
					NEXT_GROUP(g_ScnCerebroSavage);
					NEXT_GROUP(g_ScnCerebroSpace);
					NEXT_GROUP(g_ScnCerebroClone);
					if (scene) {
						g_SceneIndex = scene;
						g_PauseExit = 1;
						g_SceneExitTimer = 1;
						g_SceneIndexSet = 1;
					}
				} else {
					g_PauseCheatCode = 7;
					if (g_Pad2Raw == 0xC)
						g_PauseCheatPtr = (u8 *)CODEPTR(Tbl_CheatCodeStageSkipAlt);
					else
						g_PauseCheatPtr = (u8 *)CODEPTR(Tbl_CheatCodeStageSkip);
				}
			} else if (chord == 0x11) {
				if (g_ProgressFlags & 0x200) {
					Snd_Stop(0x24);
					Snd_Play(0x24);
					if (g_Player1.health > 0)
						g_Player1.health = 0x7FFF;
					if (g_Player2.health > 0)
						g_Player2.health = 0x7FFF;
				} else {
					g_PauseCheatCode = 8;
					if (g_Pad2Raw == 0xC)
						g_PauseCheatPtr = (u8 *)CODEPTR(Tbl_CheatCodeInvincibleAlt);
					else
						g_PauseCheatPtr = (u8 *)CODEPTR(Tbl_CheatCodeInvincible);
				}
			}
		}
	}
	if (g_Player1.health > 0 && (g_Pad1Latch & 0x80))
		g_PauseExit = 1;
	if (g_Player2.health > 0 && (g_Pad2Latch & 0x80))
		g_PauseExit = 1;
}
