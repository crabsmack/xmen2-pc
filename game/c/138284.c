/* LaserCannon_OnDeath - LASERCANNON class slot +$72 (replaces XNPC's death reaction): it goes up
 * in a chain explosion along Tbl_ExplodeFxSpiralPath (ExplodeFx_Chain, harmless), from its
 * position up to its body box's top.
 *
 * self: the cannon.  damage, type: the last hit (not used). */
#include "xmen2.h"

void LaserCannon_OnDeath(LaserCannon *self, char damage, char type)
{
	ExplodeFx_Chain(self, CODEPTR(Tbl_ExplodeFxSpiralPath), 0, 0, self->boxTop - (self->y >> 8));
}
