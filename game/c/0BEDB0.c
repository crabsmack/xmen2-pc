/* SporeMine_ClassInit - per-level class setup of SPOREMINE (class slot +$5E; the parent's isn't
 * called): a fresh spore pool (SporeMine_InitPool), then the spores' sprite definition
 * (Art_SporeMineSpr, frame 3) added to the level's set: the slot goes to g_SporeMineSprDef.
 *
 * Its arguments (0 and the class's index in the level's class list) are not read.
 * (The empty do/while (0) makes MPW C keep the pending argument pop before returning, as the
 * ROM does.) */
#include "xmen2.h"

void SporeMine_ClassInit(void)
{
	SporeMine_InitPool();
	g_SporeMineSprDef = SprDef_Add(CODEPTR(Art_SporeMineSpr), 3);
	do {
	} while (0);
}
