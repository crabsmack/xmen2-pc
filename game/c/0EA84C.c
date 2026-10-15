/* Hologram_Reset - class slot +$6A of HOLOGRAM: does nothing (calls the empty
 * Hologram_EmptyStub). Hologram_ClassInit calls it too.
 *
 * self: the object (0 from Hologram_ClassInit); unused. */
#include "xmen2.h"

void Hologram_Reset(Hologram *self)
{
	Hologram_EmptyStub();
}
