/* BrainRocket_ClassInit - per-level class setup of BRAINROCKET (class slot +$5E; the parent's isn't
 * called): nothing but a call of the empty BrainRocket_EmptyStub.
 *
 * Its arguments (0, the class's index in the level's class list) are ignored. Never runs in the
 * corpus. */
#include "xmen2.h"

void BrainRocket_ClassInit(void)
{
	BrainRocket_EmptyStub();
}
