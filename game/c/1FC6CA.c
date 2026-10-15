/* Scaler_Step - run the scaler for this frame: start a ready job's pass, or go on with one that
 * yielded.
 *
 * g_ScalerReady negative: a job is ready (Scaler_Start): Scaler_BeginPass; positive: a pass
 * yielded: Scaler_Resume; 0: nothing to do. Their result (0 done, 1 yielded) is the new
 * g_ScalerReady. Hand-written 68000: it keeps its caller's registers around the pass, which
 * runs in all of them. */
#include "xmen2.h"

void Scaler_Step(void)
{
	if ((signed char)g_ScalerReady < 0)
		g_ScalerReady = (u8)Scaler_BeginPass();
	else if (g_ScalerReady != 0)
		g_ScalerReady = (u8)Scaler_Resume();
}
