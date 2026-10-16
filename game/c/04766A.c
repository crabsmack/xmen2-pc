/* unk_BaniMazaA2_04766A - the BaniMazaA2 finale (ApocBomb_Explode, at the 12th destroyed bomb): a
 * plain ACTOR at ($100, $100) runs the explosion chain Tbl_BaniMazaA2FinaleChain 3 times
 * (ExplodeFx_ChainRepeat, offset -16, -16, harmless), the room's sprite shows frame 0 (uploaded at
 * once), colours 0-13 of line 0 become Pal_BaniMazaA2FinaleLine0, and the room's animation stops
 * (g_SceneVar3B0E = 1: BaniMazaA2_AnimSprite and the colour cycle). What the player sees is not
 * confirmed (never in the corpus). */
#include "xmen2.h"

#define g_BaniMazaA2Finale	(*(s16 *)g_SceneVar3B0E)	/* scene variable: 1 once the finale ran */

void unk_BaniMazaA2_04766A(void)
{
	register Actor *obj;
	register void *chain;
	register s16 *src;
	register long i;

	obj = (Actor *)(*CLASS(Tbl_Actor_vtable)->method[2])(0, CLASS(Tbl_Actor_vtable), 0x100, 0x100, 0);	/* New */
	if (obj) {
		chain = CODEPTR(Tbl_BaniMazaA2FinaleChain);
		ExplodeFx_ChainRepeat(obj, chain, 0, -16, -16, 3);
	}
	Spr_SetFrame(&g_SceneSprites[0], 0);
	Spr_UploadFrame(&g_SceneSprites[0], 0);
	src = (s16 *)CODEPTR(Pal_BaniMazaA2FinaleLine0);
	for (i = 0; i < 14; ++i)
		g_PalBase[i] = *src++;
	g_PalBaseDirty = 1;
	g_BaniMazaA2Finale = 1;
}
