/* Timer_SetVBlankHookCb - a timer callback: make its argument the V-blank game hook
 * (g_VBlankGameHook), then free the timer. Scenes start a sprite or tile animation this way a
 * while after the level starts: Timer_Create(Timer_SetVBlankHookCb, 60, FN_DATA(hook), 1).
 *
 * hook: the timer's argument, the hook function.
 * t:    the timer. */
#include "xmen2.h"

void Timer_SetVBlankHookCb(void *hook, Timer *t)
{
	g_VBlankGameHook = hook;
	Timer_Free(t);
}
