/* Timer_InitAll - mark all 30 timer slots free and empty the list of timers in use. */
#include "xmen2.h"

/* The original's file (Timer_InitAll .. Timer_RunAll) defined g_Timers: MPW C keeps the address
   of an array defined in the compile unit in a register (lea g_Timers(a5),a3), copied here. */
#ifdef XMEN2_MPW
Timer g_Timers[30];
#endif

void Timer_InitAll(void)
{
	register Timer *t = g_Timers;
	short i;

	for (i = 0; i < 30; i++) {
		t->state = TIMER_FREE;
		t++;
	}
	g_TimerList = 0;
}
