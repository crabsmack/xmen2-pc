/* SpaceB1_Seg28Handler - the Maintenance Shaft's floor-segment type 28 hook (SpaceB1_LoadLevel):
 * whatever the message, 1 when the object is a player in a climbing state (Player_ClimbState, its
 * low byte tested), else 0.
 *
 * msg: the hook's message (unused).  seg: the segment (unused).  obj: the object.
 * MPW: the call's result is tested inside the condition materialized into d0, so d0 is saved
 * around the call (move.l d0,-(a7) / move.l (a7)+,d0). */
#include "xmen2.h"

short SpaceB1_Seg28Handler(short msg, Seg *seg, Actor *obj)
{
	if ((char)Player_ClimbState(obj))
		return 1;
	return 0;
}
