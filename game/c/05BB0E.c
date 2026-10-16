/* Boulder_ClassInit - per-level class setup of BOULDER (class slot +$5E; the parent's isn't
 * called): its empty hook (Boulder_Nop6A), the sprite sets of the boulder (Art_BoulderSpr), its
 * dust (Art_BoulderSpr2) and the rock debris (Art_BoulderSpr3), all palette 2, a $258-byte block
 * for the 30 debris records (Boulder_InitDebrisPool), and the boulder rain restarted (delay 0,
 * sequence index 0: Boulder_SpawnerTick).
 *
 * self: passed on to Boulder_Nop6A (the class's index is the second argument, ignored).
 * (The rain's reset is a do/while (0) block, a macro's shape: MPW C pops the calls' arguments
 * at its head, as the ROM does.) */
#include "xmen2.h"

#define g_BoulderSeqIndex	(*(s16 *)g_SceneVar3B04)	/* scene variable: index in Tbl_BoulderSpawnDelays */
#define g_BoulderDelay		(*(s16 *)g_SceneVar3B32)	/* scene variable: frames before the next boulder */

void Boulder_ClassInit(Boulder *self)
{
	register long size;

	Boulder_Nop6A(self);
	g_BoulderSprDef = SprDef_Add(CODEPTR(Art_BoulderSpr), 2);
	g_BoulderSprDef2 = SprDef_Add(CODEPTR(Art_BoulderSpr2), 2);
	g_BoulderDebrisSprDef = SprDef_Add(CODEPTR(Art_BoulderSpr3), 2);
	size = 30 * sizeof(BoulderDebris);
	Boulder_InitDebrisPool((BoulderDebris *)Mem_StackAlloc(size, STR(Str_Empty_05BB84, "")));
	do {
		g_BoulderDelay = 0;
		g_BoulderSeqIndex = 0;
	} while (0);
}
