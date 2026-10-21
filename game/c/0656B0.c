/* PlayerSelect_Run - the player select screen, until the players have confirmed their
 * characters; then the players take them.
 *
 * Six portraits (Tbl_PlayerSelectSlots6) before the Magneto stage, seven after it (Slots7: a
 * count, then the frames' positions); with six, a player who was Magneto gets another random
 * character (Rand_Next % 6) that the other player doesn't have. One select slot per player
 * (BigSprite, on this frame; g_SceneVar3B28 / 3B2C): its character (g_P1CharIndex / g_P2CharIndex,
 * -1 none), nothing asked yet, cursor box sprite 7 / 8, its cursor placed, the scaler's buffers
 * (PlayerSelect_AllocCharBuffers, Scaler_InitParams) and its own run of VRAM tiles. A cursor
 * (BOXINPUTACTOR) and a portrait (SCALINGACTOR) per slot; "PRESS START" under each player who has
 * lives or a character. Text state 7.
 * Frame loop (as Level_FrameLoop, with the classes): the sprite list (a dummy first sprite), the
 * frame hook, the timers unless frozen, the draw hook, the portrait frames, then for each player
 * with a character "LIVES n" in sprites (font tiles $6A0 / $660, the white / blue banks; x $42 /
 * $CA, 4 px further left with two digits, y $C6; at most 99), the sprite list flushed,
 * PlayerSelect_CheckDone, the V-blank (two-player stepping waits for the frame tick). It ends 2
 * frames after both confirmed (g_SceneExitTimer, by 2 a frame with stepping).
 * Then each player whose character changed takes it (one that had none comes in with health
 * $700): present (flags $C000) with its class (cases PlayerSelect_Run_P1Beast .. _P1Magneto,
 * PlayerSelect_Run_P2Beast .. _P2Gambit), or none: no class, not present, character -1
 * (PlayerSelect_Run_SetP2, PlayerSelect_Run_P2None).
 * PC: the two slots are game memory (the cursors and portraits keep their addresses, and so do
 * g_SceneVar3B28 / 3B2C and the objects, which the trace compares): placed on the 68000 stack
 * where the original's frame had them, -$84(a6) and -$42(a6). Its a6 is always $FFC4C0: the only
 * way here is from the initial SSP $FFC518 through Game_Main (return address + frame: 16 bytes),
 * Game_SceneLoop (32), the scene's run (4), Cerebro_Run (28) and the call (4), and this C keeps
 * none of those frames on the 68000 stack. The rest of the frame (2 bytes of locals, 6 saved
 * registers) is reserved below.
 *
 * Inline here, separate blocks in the disassembly: PlayerSelect_Run_P1Cyclops, PlayerSelect_Run_P1Nightcrawler, PlayerSelect_Run_P1Psylocke, PlayerSelect_Run_P1Wolverine, PlayerSelect_Run_P1Magneto, PlayerSelect_Run_P2Cyclops, PlayerSelect_Run_P2Gambit.
 */
#include "xmen2.h"

#define g_TextState	(*(s16 *)g_SceneVar3B02)	/* scene variable: Cerebro text state */
#define g_TextCol	(*(s16 *)g_SceneVar3B04)	/* scene variable: Cerebro text cursor column */
#define g_TextRow	(*(s16 *)g_SceneVar3B06)	/* scene variable: Cerebro text cursor row */
#define g_SelectCount	(*(s16 *)g_SceneVar3B0E)	/* scene variable: selectable characters (6, or 7 with Magneto) */
#define g_SelectPos	(*(PTR(SelectPos) *)&g_SceneVar3B30)	/* scene variable: their portrait frames' screen positions */
#define g_P1Slot	(*(PTR(BigSprite) *)g_SceneVar3B28)	/* scene variable: player 1's select slot */
#define g_P2Slot	(*(PTR(BigSprite) *)g_SceneVar3B2C)	/* scene variable: player 2's select slot */
#define g_DrawHook	(*(FrameHookFn *)&g_SceneDrawHook)

/* The next hardware sprite of the list (as the sprite emitters take it). */
#define NEXT_SPRITE()	(g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link))

void PlayerSelect_Run(void)
{
	short y;
#ifdef XMEN2_MPW
	BigSprite slot1;
	BigSprite slot2;
#define SLOT1	(&slot1)
#define SLOT2	(&slot2)
#else
	unsigned long a7 = xm::frame_mark();
	unsigned long sp = a7 & ((1L << 24) - 1);	/* the 68000's 24 address bits */
	/* the original's a6: the initial SSP less the frames above this one (see the header) */
	unsigned long fp = ((const u32 *)Vectors)[0] - (16 + 32 + 4 + 28 + 4) - 4;
	BigSprite *slot1;
	BigSprite *slot2;

	if (sp < fp - 0x84)
		xm::fatal("PlayerSelect_Run: A7 $%06lX is below the original's frame", sp);
	slot1 = AT68(BigSprite, xm::frame_alloc(sp - (fp - 0x84)));
	slot2 = slot1 + 1;
#define SLOT1	slot1
#define SLOT2	slot2
#endif
	register short tiles;
	register SelectPos *table;
	register Actor *o;

#ifndef XMEN2_MPW
	xm::frame_alloc(2 + 24);
#endif
	g_P1Slot = SLOT1;
	g_P2Slot = SLOT2;
	if (g_ScnCerebroBaniMaza <= g_SceneIndex)
		table = (SelectPos *)CODEPTR(Tbl_PlayerSelectSlots7);
	else {
		table = (SelectPos *)CODEPTR(Tbl_PlayerSelectSlots6);
		if (g_P1CharIndex == 6) {
			do
				g_P1CharIndex = (s8)((unsigned short)Rand_Next() % 6);
			while (g_P1CharIndex == g_P2CharIndex);
		}
		if (g_P2CharIndex == 6) {
			do
				g_P2CharIndex = (s8)((unsigned short)Rand_Next() % 6);
			while (g_P1CharIndex == g_P2CharIndex);
		}
	}
	g_SelectCount = table->x;
	table++;
	g_SelectPos = table;
	SLOT1->player = 1;
	SLOT1->charIndex = g_P1CharIndex;
	SLOT1->savedChar = -1;
	SLOT1->frameSpr = 7;
	SLOT1->busy = 0;
	SLOT1->hidden = 0;
	SLOT1->confirmed = 0;
	SLOT1->big = 0;
	SLOT1->changing = 0;
	PlayerSelect_PlaceCursor(SLOT1);
	SLOT2->player = 2;
	SLOT2->charIndex = g_P2CharIndex;
	SLOT2->savedChar = -1;
	SLOT2->frameSpr = 8;
	SLOT2->busy = 0;
	SLOT2->hidden = 0;
	SLOT2->confirmed = 0;
	SLOT2->big = 0;
	SLOT2->changing = 0;
	PlayerSelect_PlaceCursor(SLOT2);
	tiles = PlayerSelect_AllocCharBuffers();
	Scaler_InitParams();
	SLOT1->params = g_ScaleParamsPtr;
	SLOT1->tiles = tiles;
	SLOT1->tileBase = g_VramTileCursor;
	SLOT1->pal = 1;
	g_VramTileCursor += tiles;
	SLOT2->params = g_ScaleParamsPtr;
	SLOT2->tiles = tiles;
	SLOT2->tileBase = g_VramTileCursor;
	SLOT2->pal = 1;
	g_VramTileCursor += tiles;
	g_TextState = 7;
	o = (Actor *)(*CLASS(Tbl_BoxInputActor_vtable)->method[2])(0, CLASS(Tbl_BoxInputActor_vtable), 0, 0,
		SLOT1);	/* New */
	o = (Actor *)(*CLASS(Tbl_BoxInputActor_vtable)->method[2])(0, CLASS(Tbl_BoxInputActor_vtable), 0, 0,
		SLOT2);
	o = (Actor *)(*CLASS(Tbl_ScalingActor_vtable)->method[2])(0, CLASS(Tbl_ScalingActor_vtable), 0, 0,
		SLOT1);
	o = (Actor *)(*CLASS(Tbl_ScalingActor_vtable)->method[2])(0, CLASS(Tbl_ScalingActor_vtable), 0, 0,
		SLOT2);
	if (g_P1Lives || g_P1CharIndex != -1) {
		g_TextCol = 5;
		g_TextRow = 0x16;
		Cerebro_PrintText(STR(Str_PressStart_065DAA, "PRESS START"), 2);
	}
	if (g_P2Lives || g_P2CharIndex != -1) {
		g_TextCol = 0x18;
		g_TextRow = 0x16;
		Cerebro_PrintText(STR(Str_PressStart, "PRESS START"), 3);
	}
	g_InLevelLoop = 1;
	ClassList_InitAll();
	do {
		do {
			Spr_BeginFrame();
			NEXT_SPRITE();
			g_SprWritePtr->y = 0x1C;
			g_SprWritePtr->size = 0;
			g_SprWritePtr->attr = 0;
			g_SprWritePtr->x = 0x1C;
			(*g_SceneFrameHook)();
			g_LevelFrame++;
			g_FrameTick = 0;
			if (!g_LevelFreeze)
				Timer_RunAll();
			(*g_DrawHook)();
			PlayerSelect_DrawFrames();
			if (g_P1Slot->charIndex != -1) {
				register char *s = STR(Str_Lives_065D96, "LIVES ");
				register unsigned char c;
				register short sx = 0x42;

				y = 0xC6;
				if (g_P1Lives > 9)
					sx -= 4;
				while ((c = *s)) {
					NEXT_SPRITE();
					g_SprWritePtr->y = y + 0x80;
					g_SprWritePtr->size = 0;
					g_SprWritePtr->attr = c - 0x20 + 0x66C0;
					g_SprWritePtr->x = sx + 0x80;
					s++;
					sx += 8;
				}
				if (g_P1Lives > 9) {
					c = g_P1Lives / 10;
					if (c > 9)
						c = 9;
					NEXT_SPRITE();
					g_SprWritePtr->y = y + 0x80;
					g_SprWritePtr->size = 0;
					g_SprWritePtr->attr = c + '0' - 0x20 + 0x66C0;
					g_SprWritePtr->x = sx + 0x80;
					sx += 8;
				}
				NEXT_SPRITE();
				g_SprWritePtr->y = y + 0x80;
				g_SprWritePtr->size = 0;
				g_SprWritePtr->attr = g_P1Lives % 10 + '0' - 0x20 + 0x66C0;
				g_SprWritePtr->x = sx + 0x80;
			}
			if (g_P2Slot->charIndex != -1) {
				register char *s = STR(Str_Lives, "LIVES ");
				register short sx = 0xCA;
				register char c;

				y = 0xC6;
				if (g_P2Lives > 9)
					sx -= 4;
				while ((c = *s)) {
					NEXT_SPRITE();
					g_SprWritePtr->y = y + 0x80;
					g_SprWritePtr->size = 0;
					g_SprWritePtr->attr = c - 0x20 + 0x6680;
					g_SprWritePtr->x = sx + 0x80;
					s++;
					sx += 8;
				}
				if (g_P2Lives > 9) {
					c = g_P2Lives / 10;
					if (c > 9)
						c = 9;
					NEXT_SPRITE();
					g_SprWritePtr->y = y + 0x80;
					g_SprWritePtr->size = 0;
					g_SprWritePtr->attr = c + '0' - 0x20 + 0x6680;
					g_SprWritePtr->x = sx + 0x80;
					sx += 8;
				}
				NEXT_SPRITE();
				g_SprWritePtr->y = y + 0x80;
				g_SprWritePtr->size = 0;
				g_SprWritePtr->attr = g_P2Lives % 10 + '0' - 0x20 + 0x6680;
				g_SprWritePtr->x = sx + 0x80;
			}
			Spr_EndFrame();
			g_DmaFlushReq = 1;
			PlayerSelect_CheckDone();
			if (g_TwoPlayerStep && !g_LevelFreeze)
				while (!g_FrameTick)
					IRQ_WAIT();
			VDP_WaitVBlank();
		} while (g_LevelFreeze || g_SceneExitTimer <= 0);
		g_SceneExitTimer -= (char)(g_TwoPlayerStep ? 2 : 1);
	} while (g_SceneExitTimer > 0);
	ClassList_CallSlot62();
	g_InLevelLoop = 0;
	if (g_P1CharIndex != g_P1Slot->charIndex) {
		if (g_P1CharIndex == -1)
			g_P1Health = 0x700;
		g_P1CharIndex = g_P1Slot->charIndex;
	}
	g_Player1.flags = 0xC000;
	switch (g_P1CharIndex) {
	case 0:
		g_Player1.cls = CLASS(Tbl_Beast_vtable);
		break;
	case 1:
		g_Player1.cls = CLASS(Tbl_Cyclops_vtable);
		break;
	case 2:
		g_Player1.cls = CLASS(Tbl_Gambit_vtable);
		break;
	case 3:
		g_Player1.cls = CLASS(Tbl_Nightcrawler_vtable);
		break;
	case 4:
		g_Player1.cls = CLASS(Tbl_Psylocke_vtable);
		break;
	case 5:
		g_Player1.cls = CLASS(Tbl_Wolverine_vtable);
		break;
	case 6:
		g_Player1.cls = CLASS(Tbl_Magneto_vtable);
		break;
	default:
		g_Player1.cls = 0;
		g_Player1.flags = 0;
		g_P1CharIndex = -1;
		break;
	}
	if (g_P2CharIndex != g_P2Slot->charIndex) {
		if (g_P2CharIndex == -1)
			g_P2Health = 0x700;
		g_P2CharIndex = g_P2Slot->charIndex;
	}
	g_Player2.flags = 0xC000;
	switch (g_P2CharIndex) {
	case 0:
		g_Player2.cls = CLASS(Tbl_Beast_vtable);
		break;
	case 1:
		g_Player2.cls = CLASS(Tbl_Cyclops_vtable);
		break;
	case 2:
		g_Player2.cls = CLASS(Tbl_Gambit_vtable);
		break;
	case 3:
		g_Player2.cls = CLASS(Tbl_Nightcrawler_vtable);
		break;
	case 4:
		g_Player2.cls = CLASS(Tbl_Psylocke_vtable);
		break;
	case 5:
		g_Player2.cls = CLASS(Tbl_Wolverine_vtable);
		break;
	case 6:
		g_Player2.cls = CLASS(Tbl_Magneto_vtable);
		break;
	default:
		g_Player2.cls = 0;
		g_Player2.flags = 0;
		g_P2CharIndex = -1;
		break;
	}
#ifndef XMEN2_MPW
	xm::frame_release(a7);
#endif
}
