/* Timer_Stop - stop a timer without freeing it: Timer_RunAll neither counts it down nor fires it.
 *
 * t: the timer; null is ignored. */
#include "xmen2.h"

void Timer_Stop(register Timer *t)
{
	if (t)
		t->state = TIMER_STOPPED;
}