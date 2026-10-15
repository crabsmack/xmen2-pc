/* XClone_MakerBufSize - the size of the clone-maker buffer: the largest of 0, $2A * 2 and the
 * EVILBEAM object size (its class slot +$06, GetSize).
 *
 * Returns the size (XClone_StartMakerMode, XClone_UnlockArena). */
#include "xmen2.h"

short XClone_MakerBufSize(void)
{
	short size;
	register short n;

	size = 0;
	n = 0x2A;
	n <<= 1;
	size = MAX(size, n);
	n = (*CLASS(Tbl_EvilBeam_vtable)->method[0])();	/* GetSize */
	size = MAX(size, n);
	return size;
}
