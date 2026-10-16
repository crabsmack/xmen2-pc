/* XClone_LoadResources - load the clone-maker sprite definitions (run by Spr_ReloadDefs from
 * XClone_StartMakerMode): the player sprites for EVILBEAST's throws, the debris pool's pieces,
 * the FLYBROOD and bio-flame sprites; then the GAUSSBULLET pool. */
#include "xmen2.h"

#define g_CloneDebris	(*(PTR(DebrisPool) *)g_SceneVar3B1C)	/* scene variable: the clone-maker debris pool */

void XClone_LoadResources(void)
{
	g_EvilBeastShockwaveSprDef = SprDef_Add(CODEPTR(Art_PlayerSpr), 2);
	g_CloneDebris->sprDef = SprDef_Add(CODEPTR(Art_CloneA5Spr), 2);
	g_FlyBroodSprDef = SprDef_Add(CODEPTR(Art_FlyBroodSpr), 1);
	g_BioFlameSprDef = SprDef_Add(CODEPTR(Art_CloneA1Spr), 3);
	GaussBullet_InitPool();
}
