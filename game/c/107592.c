/* SpaceB1_LoadResources - the Maintenance Shaft's sprite sets: Art_SpaceB1Spr2 (palette 3) as
 * g_SparkSprDef; none for MAINTROBOT (g_MaintRobotSprDef -1). */
#include "xmen2.h"

void SpaceB1_LoadResources(void)
{
	g_SparkSprDef = SprDef_Add(CODEPTR(Art_SpaceB1Spr2), 3);
	g_MaintRobotSprDef = -1;
}
