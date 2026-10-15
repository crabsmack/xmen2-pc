/* MagnetoBoss_ClassInit - per-level class setup of MAGNETOBOSS (class slot +$5E; the parent's
 * isn't called): register the rubble sprite set (Art_MagnetoBossSpr, palette 0), and give the
 * class a $800-byte block for its rubble pool (32 MagnetoRubble records), kept in g_ClassData.
 *
 * self:       0 (unused).
 * classIndex: the class's index in the level's class list (also kept in g_MagnetoRubbleClass,
 *             where MagnetoBoss_SpawnRubble finds the pool).
 * (The empty do/while (0), a macro that expands to nothing as in Barbarian_Init, makes MPW C
 * keep the pending argument pops before returning, as the ROM does.) */
#include "xmen2.h"

void MagnetoBoss_ClassInit(MagnetoBoss *self, short classIndex)
{
	g_MagnetoBossSprDef = SprDef_Add(CODEPTR(Art_MagnetoBossSpr), 0);
	g_MagnetoRubbleClass = classIndex;
	g_ClassData[classIndex] = Mem_StackAlloc(0x800, STR(Str_Empty_0360A4, ""));
	do {
	} while (0);
}
