/* Obj_AllocPool - allocate the object pool of a level.
 *
 * count: the level's objects (Level_Load).
 * With the two players, count + 2 objects: g_ObjTable gets a pointer per object (Mem_StackAlloc),
 * slots 0 and 1 the players g_Player1/g_Player2, the others $C2-byte objects in g_ObjStorage
 * (also allocated here; g_ObjCount = count + 2). The level objects start unused (flags 0) and
 * know their slot (index). */
#include "xmen2.h"

void Obj_AllocPool(register short count)
{
	register u8 *mem;
	register Actor *o;

	count += 2;
#if defined(XMEN2_NATIVE)
	/* phase 7: players 3 and 4 get the two slots after the pool's (past g_ObjCount: no level loop sees
	   them; a platform finds a rider by its slot) */
	g_ObjTable = (PTR(Actor) *)Mem_StackAlloc(count + (g_opt.players > 2 ? 2 : 0) << 2, STR(Str_ObjPoolTagPtrs, ""));
	if (g_opt.players > 2) {
		register short i;

		g_ObjTable[count] = (Actor *)&g_Player3;
		g_ObjTable[count + 1] = (Actor *)&g_Player4;
		g_P34ObjIndex[0] = count;
		g_P34ObjIndex[1] = count + 1;
		for (i = 0; i < 256; i++)
			g_P34Owner[i] = 0;	/* no object is a player 3-4's yet */
	}
#else
	g_ObjTable = (PTR(Actor) *)Mem_StackAlloc(count << 2, STR(Str_ObjPoolTagPtrs, ""));
#endif
	g_ObjStorage = Mem_StackAlloc(count * 0xC2, STR(Str_ObjPoolTags, ""));
	g_ObjCount = count;
	g_ObjTable[0] = (Actor *)&g_Player1;
	g_ObjTable[1] = (Actor *)&g_Player2;
	mem = (u8 *)g_ObjStorage;
	for (count = 2; count < g_ObjCount; count++, mem += 0xC2)
		g_ObjTable[count] = (Actor *)mem;
	count = 2;
	o = g_ObjTable[2];
	while (count < g_ObjCount) {
		o->flags = 0;
		o->index = (u8)count;
		o = g_ObjTable[++count];
	}
}
