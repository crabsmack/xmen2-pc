/* ExodusBlast_StateDie - EXODUSBLAST's last state: no kind (no longer drawn), then its Kill
 * (class slot +$52).
 *
 * self: the blast. No value returned. */
#include "xmen2.h"

void ExodusBlast_StateDie(register ExodusBlast *self)
{
	self->kind = 0;
	(*self->cls->method[19])(self);		/* +$52 Kill */
}
