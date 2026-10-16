/* SentinelsD1_LoadResources - the Sentinels base's sprite sets: no second ninja set
 * (g_NinjaSprDef2 = -1), Art_Spr_0DBFC0 (palette 0) as g_ControlPanelSprDef2, Art_MaintRobotSpr
 * (palette 1) as g_MaintRobotSprDef, Art_SentinelsD1Spr (palette 0) as g_ControlPanelSprDef
 * (SprDef_Add). */
#include "xmen2.h"

void SentinelsD1_LoadResources(void)
{
	g_NinjaSprDef2 = -1;
	g_ControlPanelSprDef2 = SprDef_Add(CODEPTR(Art_Spr_0DBFC0), 0);
	g_MaintRobotSprDef = SprDef_Add(CODEPTR(Art_MaintRobotSpr), 1);
	g_ControlPanelSprDef = SprDef_Add(CODEPTR(Art_SentinelsD1Spr), 0);
}
