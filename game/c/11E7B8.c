/* BioBalls_Reset - BIOBALLS class slot +$6A (per-class reset hook): only calls the empty
 * BioBalls_EmptyStub. Never runs in the corpus.
 *
 * self: the object (0 from a ClassInit; unused). */
#include "xmen2.h"

void BioBalls_Reset(void)
{
	BioBalls_EmptyStub();
}
