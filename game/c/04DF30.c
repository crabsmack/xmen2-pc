/* ApocBomb_Nop6A - class slot +$6A of APOCBOMB: only calls the empty ApocBomb_NopStatic.
 *
 * self: the object (unused). Never runs in the corpus. */
#include "xmen2.h"

void ApocBomb_Nop6A(ApocBomb *self)
{
	ApocBomb_NopStatic();
}
