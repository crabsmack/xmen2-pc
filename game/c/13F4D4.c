/* Debris_CreatePool - a debris pool for n pieces from the level heap, its sprite definition added
 * and its update timer started.
 *
 * n: pieces.  def: the pieces' sprite definition (0: none, the pool isn't drawn until
 * Debris_SetSprDef gives it a slot).  frame: SprDef_Add's second argument.
 * Floor line 400, the bounce limit for 4 bounces (Debris_SetBounceLimit), every piece cleared,
 * none flying. The timer (Debris_Update, every frame) gets the pool as its argument.
 * Returns the pool. */
#include "xmen2.h"

DebrisPool *Debris_CreatePool(register short n, void *def, char frame)
{
	register short size = n * 0x14;		/* n pieces */
	register DebrisPool *pool;
	DebrisPiece *pieces;

	pool = (DebrisPool *)Mem_StackAlloc(size + 0xC, STR(Str_DebrisPoolTag, ""));
	pool->used = 0;
	pool->max = n;
	pool->floorY = 0x190;
	if (def)
		pool->sprDef = SprDef_Add(def, frame);
	else
		pool->sprDef = -1;
	if (pool->timer = Timer_Create((TimerFn)Debris_Update, 0, pool, 1))
		Timer_Start(pool->timer);
	Debris_SetBounceLimit(pool, 4);
	pieces = (DebrisPiece *)&pool[1];
	Mem_Set(pieces, 0, size);
	return pool;
}
