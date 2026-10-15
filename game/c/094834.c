/* EvilMaker_ActKill - EVILMAKER AI script step 0: remove itself (class slot +$52 Kill).
 *
 * self: EVILMAKER. */
#include "xmen2.h"

void EvilMaker_ActKill(EvilMaker *self)
{
	(*self->cls->method[19])(self);		/* +$52 Kill */
}
