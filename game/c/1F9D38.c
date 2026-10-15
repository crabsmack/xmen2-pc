/* Timer_Free - free a timer: the next Timer_RunAll pass unlinks it and marks the slot free.
 *
 * t: the timer; null is ignored. The caller then forgets its pointer. */
#include "xmen2.h"

void Timer_Free(register Timer *t)
{
	if (t)
		t->state = TIMER_FREE_PENDING;
}