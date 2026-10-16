/* CloneA5_LoadResources - load the boss arena's sprite definitions: the little leg (Art_CloneA5Spr2,
 * frame 2), the wall and clone debris (Art_BroodBossWallSpr, 1), the claw (Art_BroodClawSpr, 2,
 * g_BroodClawSprDef) and the boss debris (Art_CloneA5Spr, 2); the two debris pools get theirs
 * (Debris_SetSprDef). Run again by Spr_ReloadDefs (CloneA5_FrameHook). */
#include "xmen2.h"

#define g_LittleLegSprDef	(*(s16 *)g_SceneVar3B04)	/* scene variable (CloneA5): sprite-definition slot of Art_CloneA5Spr2, the leg */
#define g_WallSprDef		(*(s16 *)g_SceneVar3B08)	/* scene variable (CloneA5): sprite-definition slot of Art_BroodBossWallSpr, the wall and the clone debris */
#define g_BroodBossDebris	(*(PTR(DebrisPool) *)g_SceneVar3B14)	/* scene variable: the boss's explosion debris pool */
#define g_CloneDebris		(*(PTR(DebrisPool) *)g_SceneVar3B1C)	/* scene variable: the clone-maker debris pool */

void CloneA5_LoadResources(void)
{
	register short debrisDef;

	g_LittleLegSprDef = SprDef_Add(CODEPTR(Art_CloneA5Spr2), 2);
	g_WallSprDef = SprDef_Add(CODEPTR(Art_BroodBossWallSpr), 1);
	g_BroodClawSprDef = SprDef_Add(CODEPTR(Art_BroodClawSpr), 2);
	debrisDef = SprDef_Add(CODEPTR(Art_CloneA5Spr), 2);
	Debris_SetSprDef(g_CloneDebris, g_WallSprDef);
	Debris_SetSprDef(g_BroodBossDebris, debrisDef);
}
