/* CoreShields_ClassInit - per-level class setup of CORESHIELDS (class slot +$5E, static): nothing
 * to do but call CoreShields_Reset with its first argument. The parent's isn't called.
 *
 * self: 0 (no object). */
#include "xmen2.h"

void CoreShields_ClassInit(CoreShields *self)
{
	CoreShields_Reset(self);
}
