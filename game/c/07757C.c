/* CloneA1_LoadResources - load the CloneA1 sprite definitions: the maintenance robot
 * (Art_MaintRobotSpr, palette 1, g_MaintRobotSprDef) and the bio-flame (Art_CloneA1Spr,
 * palette 3, g_BioFlameSprDef). */
#include "xmen2.h"

void CloneA1_LoadResources(void)
{
	g_MaintRobotSprDef = SprDef_Add(CODEPTR(Art_MaintRobotSpr), 1);
	g_BioFlameSprDef = SprDef_Add(CODEPTR(Art_CloneA1Spr), 3);
}
