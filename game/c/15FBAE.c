/* Intro_WaitFrame - wait for the V-blank, then run the intro's per-frame callback if it has one.
 *
 * cb: the callback, or 0. Hand-written 68000 with no frame of its own: it reads cb as the
 * first argument of its caller's frame (8(a6) of Intro_SegaLogoFlash), which the C caller
 * passes instead. */
#include "xmen2.h"

void Intro_WaitFrame(IntroFrameFn cb)
{
	VDP_WaitVBlank();
	if (cb)
		cb();
}
