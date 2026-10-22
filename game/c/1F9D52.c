/* Timer_RunAll - the timers' part of each frame (Level_FrameLoop). First pass over the list:
 * count running timers down; a timer at 0 is reloaded with its period and fires:
 * func(arg, timer), with g_TimerCurrent = the timer. Second pass: unlink the timers freed
 * (Timer_Free) and mark their slots free; timers started (Timer_Start) run from the next frame. */
#include "xmen2.h"

void Timer_RunAll(void)
{
	register Timer *t;
	register void *arg;
	TimerFn func;

	for (t = g_TimerList; t; t = t->next) {
		if (t->state > 0)
			--t->state;
		if (t->state == 0) {
			func = t->func;
			arg = t->arg;
			t->state = t->period;
			g_TimerCurrent = t;
#if defined(XMEN2_NATIVE)
			plat_spr_owner(t);	/* phase 6 R4: the sprites it writes are this timer's */
			{	/* phase 7: a player 3-4's timer (its frame) runs in its context */
				register int k = P34_Index(arg);

				if (k)
					P34_Enter(k);
				(*func)(arg, t);
				if (k)
					P34_Leave(k);
			}
#else
			(*func)(arg, t);
#endif
		}
	}
	for (t = g_TimerList; t; t = t->next) {
		if (t->state == TIMER_FREE_PENDING) {
			if (t->prev)
				t->prev->next = t->next;
			if (t->next)
				t->next->prev = t->prev;
			if (t == g_TimerList)
				g_TimerList = t->next;
			t->state = TIMER_FREE;
		} else if (t->state == TIMER_START_PENDING) {
			t->state = 0;
		}
	}
}
