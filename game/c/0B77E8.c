/* ChildShip_StateDie - CHILDSHIP's last state: it blows up (ExplodeFx_Burst, kind $101).
 *
 * self: the object. */
#include "xmen2.h"

void ChildShip_StateDie(ChildShip *self)
{
	ExplodeFx_Burst(self, 0x101);
}
