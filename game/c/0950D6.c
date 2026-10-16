/* EvilDrop_ActRelease - EVILDROP AI-script action (Tbl_EvilDropAiScript): release the clone: its
 * spawn record at the capsule's pixel position goes to XClone_SpawnByType, then the capsule kills
 * itself (class slot +$52).
 *
 * self: the capsule. Never runs in the corpus.
 * XClone_SpawnByType is called with the record only (the original called it undeclared): its
 * player argument is whatever lies above the record on the stack, this function's saved a4,
 * which is the capsule (BioBalls_Update runs the state with self in a4). It is read only for a
 * clone type 0, which EVILMAKER never drops (types 1-7). */
#include "xmen2.h"

void EvilDrop_ActRelease(EvilDrop *self)
{
	Placement rec;

	rec = self->clone;
	rec.x = (short)(self->x >> 8);
	rec.y = (short)(self->y >> 8);
#ifdef XMEN2_MPW
	((long (*)())XClone_SpawnByType)(&rec);
#else
	XClone_SpawnByType(&rec, (XPlayer *)(void *)self);
#endif
	(*self->cls->method[19])(self);			/* +$52 Kill */
}
