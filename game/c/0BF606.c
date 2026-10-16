/* SpikeBall_StateCycle - SPIKEBALL's only state: spikes out (frame 1) during the first onTime
 * frames of each cycle of mask + 1 frames, in (frame 0) for the rest.
 *
 * self: the spike ball. The cycle position is (g_FrameCounter - phase) & mask.
 * Returns nothing in particular. */
#include "xmen2.h"

short SpikeBall_StateCycle(SpikeBall *self)
{
	register unsigned long t = (g_FrameCounter - self->phase) & self->mask;

	(*self->cls->method[20])(self, t < self->onTime ? 1 : 0);	/* +$56 SetFrame */
	NO_RETURN_VALUE
}
