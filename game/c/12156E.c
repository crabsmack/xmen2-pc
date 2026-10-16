/* BioSentinel_ClassInit - per-level class setup of BIOSENTINEL (class slot +$5E; the parent's
 * isn't called): only its empty helper (BioSentinel_EmptyStub).
 *
 * Called with 0 and the class's index in the level's class list; both are ignored. */
#include "xmen2.h"

void BioSentinel_ClassInit(void)
{
	BioSentinel_EmptyStub();
}
