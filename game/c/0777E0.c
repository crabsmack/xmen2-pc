/* LightBeam_ClassInit - LIGHTBEAM ClassInit (class slot +$5E, once per level): add the beams' sprite
 * definition (Art_CloneA1Nem_2, frame 2) to the level's sprite definitions and keep its slot and
 * its first VRAM tile (LightBeam_Init, LightBeam_Draw). */
#include "xmen2.h"

#define g_LightBeamSprDef	g_TextWinBottom	/* reused in levels: the beams' sprite-definition slot */
#define g_LightBeamTiles	(*(s16 *)g_SceneVar3B10)	/* scene variable (CloneA1): first VRAM tile of the beams' definition */

void LightBeam_ClassInit(void)
{
	LightBeam_NopStatic();
	g_LightBeamTiles = g_SpriteDefs[g_LightBeamSprDef = SprDef_Add(CODEPTR(Art_CloneA1Nem_2), 2)].tileBase;
}
