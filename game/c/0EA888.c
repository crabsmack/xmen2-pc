/* Hologram_ClassInit - per-level class setup of HOLOGRAM (class slot +$5E, static): nothing to
 * do but call Hologram_Reset with its first argument. The parent's isn't called.
 *
 * self: 0 (no object). */
#include "xmen2.h"

void Hologram_ClassInit(Hologram *self)
{
	Hologram_Reset(self);
}
