/* Switch_DebounceWait - the switch search timer's callback after a toggle (Switch_FindAndToggle):
 * more than 10 frames later, free the timer and let the next switch tile hit start a search.
 *
 * s: the search record (the timer's argument). */
#include "xmen2.h"

void Switch_DebounceWait(SwitchSearch *s)
{
	if (g_FrameCounter - s->frame > 10) {
		Timer_Free((Timer *)g_TimerCurrent);
		g_SwitchSearchBusy = 0;
	}
}
