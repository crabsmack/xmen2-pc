/* VDP_SetShadowHighlight - VDP register 12: H40 (320-pixel) mode with shadow/highlight on ($8C89)
 * or off ($8C81). SpaceA1_RunScene turns it on for the Space Elevator and off after.
 *
 * on: nonzero for shadow/highlight.
 * Leaves the command word in d0 (no caller reads it). */
#include "xmen2.h"

void VDP_SetShadowHighlight(char on)
{
	register short mode;

	if (on)
		mode = 0x89;
	else
		mode = 0x81;
	VDP_CTRL = mode + 0x8C00;
}
