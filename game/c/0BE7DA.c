/* Launcher_GetSize - the size of a LAUNCHER object (class slot +$06), $B8 bytes.
 *
 * Never runs in the corpus. */
#include "xmen2.h"

long Launcher_GetSize(void)
{
	return sizeof(Launcher);
}
