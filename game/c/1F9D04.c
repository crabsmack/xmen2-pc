/* Timer_Start - start a timer: it fires on the next Timer_RunAll pass, then every period frames.
 *
 * t: the timer (Timer_Create); null is ignored. */
#include "xmen2.h"

void Timer_Start(register Timer *t)
{
	if (t)
		t->state = TIMER_START_PENDING;
}