/* Platform1_SteerByPad - a ridden PLATFORM1 follows its rider's d-pad (held buttons of the rider's
 * pad): its speed becomes +-|speed| by the type of its floor segment: $21 Left -, Right +
 * (Platform1_SteerByPad_LeftRight); $23 Down -, Up + (loc_106FF0); $22 Left +, Right -
 * (loc_107044); others Down +, Up - (Platform1_SteerByPad_UpDown). Nothing held: unchanged.
 *
 * self: the platform.  rider: the player standing on it. */
#include "xmen2.h"

void Platform1_SteerByPad(register Platform1 *self, register XPlayer *rider)
{
	register short speed = ABS(self->speed);

	switch (self->seg->type) {
	case 0x21:	/* Platform1_SteerByPad_LeftRight */
		if (!rider->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft)
			self->vx = -speed;
		else if (!rider->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight)
			self->vx = speed;
		break;
	case 0x23:
		if (!rider->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown)
			self->vx = -speed;
		else if (!rider->pad ? g_Pad1Held & g_PadMaskUp : g_Pad2Held & g_PadMaskUp)
			self->vx = speed;
		break;
	case 0x22:
		if (!rider->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft)
			self->vx = speed;
		else if (!rider->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight)
			self->vx = -speed;
		break;
	case 0x24:
	default:	/* Platform1_SteerByPad_UpDown */
		if (!rider->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown)
			self->vx = speed;
		else if (!rider->pad ? g_Pad1Held & g_PadMaskUp : g_Pad2Held & g_PadMaskUp)
			self->vx = -speed;
		break;
	}
}
