/* BioBalls_Reset - BIOBALLS class slot +$6A (per-class reset hook): only calls the empty
 * BioBalls_EmptyStub. Never runs in the corpus.
 *
 * Its argument (the object, 0 from a ClassInit) is not read. */
#include "xmen2.h"

void BioBalls_Reset(void)
{
	BioBalls_EmptyStub();
}
