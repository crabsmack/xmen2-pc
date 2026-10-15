/* SiberiaB1_LoadResources - load the Blizzard's sprite definitions: the bio-flame
 * (Art_CloneA1Spr, palette 3, g_BioFlameSprDef), the ice shards (Art_SiberiaB1Spr2, palette 0)
 * and the ninja's second set (Art_NinjaSpr2, palette 1, g_NinjaSprDef2). */
#include "xmen2.h"

#define g_IceShardSprDef	(*(s16 *)g_SceneVar3B0E)	/* scene variable: sprite-definition slot of the ice shards */

void SiberiaB1_LoadResources(void)
{
	g_BioFlameSprDef = SprDef_Add(CODEPTR(Art_CloneA1Spr), 3);
	g_IceShardSprDef = SprDef_Add(CODEPTR(Art_SiberiaB1Spr2), 0);
	g_NinjaSprDef2 = SprDef_Add(CODEPTR(Art_NinjaSpr2), 1);
}
