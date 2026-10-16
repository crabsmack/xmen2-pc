/* FloatBubbleExplode_Spawn - create a FLOATBUBBLEEXPLODE (a bubble bursting) at (x, y) (class slot
 * +$0E New, no placement).
 *
 * x, y: position (8 fraction bits; FloatBubble_StateBurst passes the bubble's). */
#include "xmen2.h"

void FloatBubbleExplode_Spawn(long x, long y)
{
	(*CLASS(Tbl_FloatBubbleExplode_vtable)->method[2])(0,	/* New */
		CLASS(Tbl_FloatBubbleExplode_vtable), x, y, 0);
}
