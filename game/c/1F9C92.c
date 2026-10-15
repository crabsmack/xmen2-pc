/* Timer_Create - take a free timer slot, set it up stopped and link it at the head of the list.
 *
 * func:   called as func(arg, timer) each time the timer fires.
 * period: frames between firings.
 * arg:    passed to func (usually the owner object).
 * user:   a byte the owner keeps in the timer.
 * Returns the timer (Timer_Start starts it), or 0 when all 30 slots are taken. */
#include "xmen2.h"

Timer *Timer_Create(TimerFn func, short period, void *arg, char user)
{
	register Timer *t;
	short i;

	t = g_Timers;
	for (i = 0; i < 30; i++) {
		if (t->state == TIMER_FREE) {
			t->func = func;
			t->period = period;
			t->arg = arg;
			t->user = user;
			t->state = TIMER_STOPPED;
			if (g_TimerList)
				g_TimerList->prev = t;
			t->next = g_TimerList;
			t->prev = 0;
			g_TimerList = t;
			return t;
		}
		t++;
	}
	return 0;
}