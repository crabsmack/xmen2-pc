/* Cerebro_RunTextScript - run the Cerebro screen's text script (g_SceneVar3B14; Tbl_FinalTextScript
 * has the format): 4-byte ops {op, 0, two argument bytes or a word}, until op 0:
 *   1 (Cerebro_TextOpSetCursor): text cursor = window origin + the signed x, y bytes;
 *   2: wait the word's number of frames (Cerebro_FrameHook, VDP_WaitVBlank each); when the next op
 *      is 3 or the end, a button ends the wait (text state 2);
 *   3: text state 2, clear the window (Cerebro_ClearTextWindow), cursor as op 1;
 *   4 (Cerebro_TextOpFadeOut): fade the palette out (Tbl_FadeBlack) with the word as step delay,
 *      text state 3, no palette upload pending;
 *   5, 6 (Cerebro_TextOpPlaySound): Snd_Play / Snd_Stop of the word;
 *   7 and up (Cerebro_TextOpPrint): print the text at the word's offset from the word, with style
 *      op - 7 (Cerebro_PrintText).
 * First the sprites are cleared (an empty sprite list flushed), the demo-skip flag cleared and the
 * text state set to 2; at the end the text state is -1. */
#include "xmen2.h"

#define g_TextState	(*(s16 *)g_SceneVar3B02)	/* scene variable: Cerebro text state */
#define g_TextCol	(*(s16 *)g_SceneVar3B04)	/* scene variable: Cerebro text cursor column */
#define g_TextRow	(*(s16 *)g_SceneVar3B06)	/* scene variable: Cerebro text cursor row */
#define g_CerebroScript	(*(PTR(char) *)g_SceneVar3B14)	/* scene variable: the text script */

/* The script's next word, the pointer moved past it. */
#ifdef XMEN2_MPW
#define NEXT_WORD(p)	(*((s16 *)(p))++)
#else
#define NEXT_WORD(p)	((p) += 2, *(s16 *)((p) - 2))
#endif

void Cerebro_RunTextScript(void)
{
	register char *p;
	register unsigned char op;
	register short n;
	register short style;
	register char *text;

	g_DemoSkipped = 0;
	g_TextState = 2;
	Spr_BeginFrame();
	g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
	g_SprWritePtr->y = 0x1C;
	g_SprWritePtr->size = 0;
	g_SprWritePtr->attr = 0;
	g_SprWritePtr->x = 0x1C;
	Spr_EndFrame();
	g_DmaFlushReq = 1;
	p = g_CerebroScript;
	while (*p) {
		op = *p;
		p += 2;
		switch (op) {
		case 1:
			g_TextCol = *p++ + g_TextWinLeft;
			g_TextRow = *p++ + g_TextWinTop;
			break;
		case 2:
			n = NEXT_WORD(p);
			if (*p == 3 || !*p)
				g_TextState = 2;
			do {
				Cerebro_FrameHook();
				VDP_WaitVBlank();
				--n;
				if (!n)
					break;
			} while (g_TextState == 2);
			break;
		case 3:
			g_TextState = 2;
			Cerebro_ClearTextWindow();
			g_TextCol = *p++ + g_TextWinLeft;
			g_TextRow = *p++ + g_TextWinTop;
			break;
		case 4:
			Pal_StartFade(7, -1, CODEPTR(Tbl_FadeBlack));
			g_PalFadeDelay = NEXT_WORD(p);
			g_TextState = 3;
			g_PalBaseDirty = 0;
			break;
		case 5:
			Snd_Play(*(s16 *)p);
			p += 2;
			break;
		case 6:
			Snd_Stop(*(s16 *)p);
			p += 2;
			break;
		default:
			style = op - 7;
			text = p;
			text += NEXT_WORD(p);
			Cerebro_PrintText(text, style);
			break;
		}
	}
	g_TextState = -1;
}
