/* BanimazaDoor_ClassInit - per-level class setup of BANIMAZADOOR (class slot +$5E; the parent's
 * isn't called): seed the random generator with 99999, its empty helper, its sprite definition
 * (Art_BanimazaDoorSpr, g_BanimazaDoorSprDef), the shard buffer every door shares ($120 bytes:
 * 16 DoorShards, g_BanimazaDoorBuf) and BanimazaDoor_PushCallback as the hook of floor-segment
 * type $1C.
 *
 * self: passed on to BanimazaDoor_NopStatic (the class's index is the second argument, ignored). */
#include "xmen2.h"

void BanimazaDoor_ClassInit(BanimazaDoor *self)
{
	Rand_Seed(99999);
	BanimazaDoor_NopStatic(self);
	g_BanimazaDoorSprDef = SprDef_Add(CODEPTR(Art_BanimazaDoorSpr), 0);
	do {
		g_BanimazaDoorBuf = Mem_StackAlloc(0x120, STR(Str_Empty_03A2F2, ""));
		g_SegTypeHooks[0x1C] = BanimazaDoor_PushCallback;
	} while (0);
}
