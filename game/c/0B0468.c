/* SavageA3_LoadResources - the Perimeter's extra sprite definition: Art_CloneA1Spr (frame 3) into
 * the sprite-definition table (SprDef_Add); its slot goes to g_BioFlameSprDef. */
#include "xmen2.h"

void SavageA3_LoadResources(void)
{
	g_BioFlameSprDef = SprDef_Add(CODEPTR(Art_CloneA1Spr), 3);
}
