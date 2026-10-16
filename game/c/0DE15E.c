/* ControlPanel_SetFlags - CONTROLPANEL class slot +$6E (its own): set bits in its switch's flag
 * word (g_SentinelsD1Flags[switch]: 1 destroyed, 4 damaged once, 8 twice).
 *
 * self: the panel.  bits: the bits to set. */
#include "xmen2.h"

void ControlPanel_SetFlags(ControlPanel *self, short bits)
{
	register s16 *flags = self->switchFlags;

	*flags |= bits;
}
