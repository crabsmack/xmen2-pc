/* Deathbird_ClassInit - per-level class setup of DEATHBIRD (class slot +$5E; the parent's isn't
 * called): a cleared record for its second sprite (a SprInst from Mem_StackAlloc, kept in
 * g_SceneVar3B18), and its two sprite definitions: the body (Art_DeathbirdSpr, g_DeathbirdSprDef)
 * and the burst projectiles (Art_DeathbirdSpr3, g_SceneVar3B04), frame 1 each.
 *
 * Called with 0 and the class's index in the level's class list; both are ignored. */
#include "xmen2.h"

#define g_DeathbirdSpr2		(*(PTR(SprInst) *)g_SceneVar3B18)	/* scene variable: DEATHBIRD's second sprite */
#define g_DeathbirdShotSprDef	(*(s16 *)g_SceneVar3B04)	/* scene variable: sprite definition of the burst projectiles */

void Deathbird_ClassInit(void)
{
	if (g_DeathbirdSpr2 = (SprInst *)Mem_StackAlloc(sizeof(SprInst), STR(Str_Empty_111E42, "")))
		Mem_Set(g_DeathbirdSpr2, 0, sizeof(SprInst));
	g_DeathbirdSprDef = SprDef_Add(CODEPTR(Art_DeathbirdSpr), 1);
	g_DeathbirdShotSprDef = SprDef_Add(CODEPTR(Art_DeathbirdSpr3), 1);
}
