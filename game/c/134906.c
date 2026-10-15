/* FlyBrood_ClassInit - per-level class setup of FLYBROOD (class slot +$5E; the parent's isn't
 * called): register its sprite set (Art_FlyBroodSpr, frame 1) in g_FlyBroodSprDef, except in the
 * scene g_ScnUnused3B5C names, where FLYBROOD draws from raw art (FlyBrood_Init, _Draw).
 *
 * (g_ScnUnused3B5C is never set: 0, so that is scene 0.) Its arguments (0, the class's index
 * in the level's class list) are ignored. */
#include "xmen2.h"

void FlyBrood_ClassInit(void)
{
	FlyBrood_EmptyStub();
	if (g_ScnUnused3B5C != g_SceneIndex)
		g_FlyBroodSprDef = SprDef_Add(CODEPTR(Art_FlyBroodSpr), 1);
}
