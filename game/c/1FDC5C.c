/* VDP_WaitVBlank - wait for the start of the next vertical blank, when the display is on.
 *
 * Clears g_FrameUpdated first, so the next V-int runs the once-per-frame work again
 * (VDP_FrameUpdate). With the display enabled (VDP register 1 bit 6) it polls the VDP
 * status: until VBLANK (bit 3) is clear, then until it is set. This entry is the game's
 * frame boundary: the phase-4 sync point. */
#include "xmen2.h"

void VDP_WaitVBlank(void)
{
	g_FrameUpdated = 0;
	if (g_VdpReg1 & 0x40) {
		while ((VDP_CTRL & 8) != 0)
			;
		while ((VDP_CTRL & 8) == 0)
			;
	}
}