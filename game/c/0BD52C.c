/* Barbarian_SegHook1A - floor-segment hook of type $1A, BARBARIAN's platforms
 * (g_SegTypeHooks[$1A], installed by Barbarian_ClassInit): solid only in mode 3.
 *
 * mode: what the segment code asks (3: is it solid for standing on).
 * Returns 1 when mode is 3, else 0. */
#include "xmen2.h"

short Barbarian_SegHook1A(short mode)
{
	if (mode == 3)
		return 1;
	return 0;
}
