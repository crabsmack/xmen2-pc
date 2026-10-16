/* Sys_Nop1F945C - does nothing. The camera shakes call it with each shake step's x as (x, 0)
 * (Cam_ShakeStep, CloneA5_CamApply, BroodClaw_CamApplyShake): a camera nudge compiled out.
 *
 * x, y: unused. */
#include "xmen2.h"

void Sys_Nop1F945C(short x, short y)
{
}
