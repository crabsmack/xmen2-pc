/* SavageA4_LoadResources - the Chamber's extra sprite definition: Art_SavageA4Spr2 (frame 3) into
 * the sprite-definition table (SprDef_Add); its slot goes to g_SavageBioSprDef (the SAVAGEBIGBIO
 * tentacle tiles: SavageBio_StateReloadArt14). */
#include "xmen2.h"

void SavageA4_LoadResources(void)
{
	g_SavageBioSprDef = SprDef_Add(CODEPTR(Art_SavageA4Spr2), 3);
}
