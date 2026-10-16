/* Synchronizer_New - SYNCHRONIZER New (class slot +$0A): take a free slot of g_SyncPool and make
 * it a synchronizer of cls for the members of memberClass on switch-board entry board (its Init,
 * slot +$0E).
 *
 * cls: the class descriptor (Tbl_Synchronizer_vtable).  memberClass: the members' class.
 * board: the g_BBoard entry (the members' placement param 0).
 * The pool (4 slots of the class's GetSize bytes, all free) is made on the level stack at the
 * first call (Mem_StackAlloc). Returns the synchronizer, or 0 when all 4 are in use. */
#include "xmen2.h"

Synchronizer *Synchronizer_New(Class *cls, Class *memberClass, register unsigned char board)
{
	Class *c = cls;
	register short size = (*c->method[0])();		/* +$06 GetSize */
	register short i;
	Synchronizer *s;
	register u8 *slot;

	if (!g_SyncPool) {
		g_SyncPool = Mem_StackAlloc(size * 4, STR(Str_Empty_119892, ""));
		for (i = 0, slot = g_SyncPool; i < 4; i++, slot += size) {
			s = (Synchronizer *)slot;
			s->inUse = 0;
		}
	}
	for (i = 0, slot = g_SyncPool; i < 4; i++, slot += size) {
		s = (Synchronizer *)slot;
		if (!(s->inUse & 1)) {
			s->cls = c;
			(*s->cls->method[2])(s, memberClass, board);	/* +$0E Init */
			return s;
		}
	}
	return 0;
}
