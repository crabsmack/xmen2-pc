/* CloneA2_StartCamAltMode - put the CloneA2 camera in its alternate mode (CloneA2_CamHookAlt) for
 * $300 frames: a timer (CloneA2_CamAltTimer, every frame, its own Timer as argument) switches it
 * back when g_FloatFallSpeed, set to $300 here, reaches 0. With no free timer, nothing but the
 * countdown. */
#include "xmen2.h"

void CloneA2_StartCamAltMode(void)
{
	register Timer *t;

	t = Timer_Create(CloneA2_CamAltTimer, 1, 0, 0);
	g_FloatFallSpeed = 0x300;
	if (t) {
		t->arg = t;
		Timer_Start(t);
		CloneA2_CamHookAlt(g_Camera);
	}
}
