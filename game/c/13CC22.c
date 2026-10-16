/* Ninja_Reset - class slot +$6A of NINJA: only calls the empty Ninja_NopStatic.
 *
 * self: the object (unused). Never runs in the corpus. */
#include "xmen2.h"

void Ninja_Reset(Ninja *self)
{
	Ninja_NopStatic();
}
