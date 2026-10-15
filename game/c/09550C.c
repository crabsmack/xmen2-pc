/* XClone_Kill - XCLONE dies (class slot +$52): kill its BIOBALLS object, count it, unlock the
 * arena it locked, end the wave at 6 kills, then the parent's Kill.
 *
 * self: the clone.
 * A Magneto clone (type 4, Clone_CharTypeOf) is not counted in g_CloneWaveKills. */
#include "xmen2.h"

void XClone_Kill(XClone *self)
{
	short type;

	if (self->child) {
		(*self->child->cls->method[19])(self->child);	/* Kill */
		self->child = 0;
	}
	type = Clone_CharTypeOf(self);
	if (type != 4)
		g_CloneWaveKills++;
	if (self->lockedArena)
		XClone_UnlockArena(self);
	if (g_CloneWaveKills >= 6)
		XClone_EndWave();
	(*CLASS(Tbl_XClone_vtable)->parent->method[19])(self);	/* BIONPC Kill */
}
