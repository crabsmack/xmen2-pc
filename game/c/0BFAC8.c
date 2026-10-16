/* SwampBubbles_ClassInit - per-level class setup of SWAMPBUBBLES (class slot +$5E; the parent's
 * isn't called): only calls the empty SwampBubbles_Nop6A.
 *
 * self: 0, passed on to SwampBubbles_Nop6A. The class's index in the level's class list follows;
 * it is ignored.
 * (The empty do/while (0) makes MPW C keep the pending argument pop before returning, as the
 * ROM does.) */
#include "xmen2.h"

void SwampBubbles_ClassInit(SwampBubbles *self)
{
	SwampBubbles_Nop6A(self);
	do {
	} while (0);
}
