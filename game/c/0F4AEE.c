/* SentinelsB2_LoadDigits - the Sentinels escape's countdown digits: the sprite set
 * Art_SentinelsB2Spr (palette 1, SprDef_Add), whose first VRAM tile is the digit 0 (g_DigitTile,
 * SentinelsB2_FrameHook). */
#include "xmen2.h"

#define g_DigitTile	(*(s16 *)g_SceneVar3B06)	/* scene variable: VRAM tile of the digit 0 */

void SentinelsB2_LoadDigits(void)
{
	register short n;
	register SprInst *spr;

	n = SprDef_Add(CODEPTR(Art_SentinelsB2Spr), 1);
	spr = &g_SpriteDefs[n];
	g_DigitTile = spr->tileBase;
}
