/* Mine_ClassInit - per-level class setup of MINE (class slot +$5E; the parent's isn't called): a
 * fresh mine pool (Mine_InitPool), then the mines' sprite definition (Art_MineNem, frame 3) added
 * to the level's set: the slot goes to g_MineSprDef.
 *
 * Its arguments (0 and the class's index in the level's class list) are not read.
 * (The empty do/while (0) makes MPW C keep the pending argument pop before returning, as the
 * ROM does.) */
#include "xmen2.h"

void Mine_ClassInit(void)
{
	Mine_InitPool();
	g_MineSprDef = SprDef_Add(CODEPTR(Art_MineNem), 3);
	do {
	} while (0);
}
