/* Apocalypse_ClassInit - per-level class setup of APOCALYPSE (class slot +$5E; the parent's isn't
 * called): its three empty helpers, the scene's default colours (Pal_SceneDefaultLine1) into
 * palette line 3, its sprite definition (Art_ApocalypseSpr, g_ApocalypseSprDef), the record of
 * its second sprite (a SprInst from Mem_StackAlloc) and no live bombs.
 *
 * Called with 0 and the class's index in the level's class list; both are ignored. */
#include "xmen2.h"

#define g_ApocBombCount		(*(s16 *)g_SceneVar3B08)	/* scene variable: live bombs */
#define g_ApocalypseSpr2	(*(PTR(SprInst) *)g_SceneVar3B1C)	/* scene variable: APOCALYPSE's second sprite */

void Apocalypse_ClassInit(void)
{
	register s16 *src;
	register s16 *dst;
	register long i;

	Apocalypse_CallNopStatics();
	src = (s16 *)CODEPTR(Pal_SceneDefaultLine1);
	dst = &g_PalBase[0x30];
	for (i = 0; i < 16; i++)
		*dst++ = *src++;
	g_ApocalypseSprDef = SprDef_Add(CODEPTR(Art_ApocalypseSpr), 3);
	g_ApocalypseSpr2 = (SprInst *)Mem_StackAlloc(sizeof(SprInst), STR(Str_Empty_04C582, ""));
	g_ApocBombCount = 0;
}
