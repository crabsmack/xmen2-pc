/* Synchronizer_Destroy - SYNCHRONIZER slot +$1E: clear its board flag (ClearFlag, slot +$1A),
 * free its pool slot (Free, slot +$22) and the timer running now (g_TimerCurrent: its own, as it
 * is only called from its states).
 *
 * self: the synchronizer. */
#include "xmen2.h"

void Synchronizer_Destroy(register Synchronizer *self)
{
	(*self->cls->method[5])(self);		/* +$1A ClearFlag */
	(*self->cls->method[7])(self);		/* +$22 Free */
	Timer_Free((Timer *)g_TimerCurrent);
}
