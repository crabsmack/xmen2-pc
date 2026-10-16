/* BroodBoss_StateShockOff - BROODBOSS state: end the floor shock (flag bit 1, sound $F5), blank the
 * two shock sprites (scene sprites 0 and 1, frame 0), roar (Tbl_BroodBossAnim), next AI-script
 * step on the next update.
 *
 * self: the boss. */
#include "xmen2.h"

#define g_BroodBossFlags	(*(s16 *)g_SceneVar3B0C)	/* scene variable: 1 fight on, 2 floor shock, $10 lower the body, $40 wall smashed */

void BroodBoss_StateShockOff(BroodBoss *self)
{
	g_BroodBossFlags &= ~2;
	Snd_Stop(0xF5);
	Spr_SetFrame(&g_SceneSprites[0], 0);
	Spr_UploadFrame(&g_SceneSprites[0], 0);
	Spr_SetFrame(&g_SceneSprites[1], 0);
	Spr_UploadFrame(&g_SceneSprites[1], 0);
	Obj_SetAnim(self, CODEPTR(Tbl_BroodBossAnim));
	AI_NEXT_STEP(self, self->ai, 0);
}
