/* Synchronizer_Run - SYNCHRONIZER slot +$12 (its timer, every frame): run its state.
 *
 * self: the synchronizer. */
#include "xmen2.h"

void Synchronizer_Run(register Synchronizer *self)
{
	(*self->state)(self);
}
