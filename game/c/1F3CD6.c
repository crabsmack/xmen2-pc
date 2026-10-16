/* XClimber_GetSize - the size of an XCLIMBER object (class slot +$06), $C2 bytes: XPLAYER's $BC,
 * the segment it climbs and its climb state. */
#include "xmen2.h"

long XClimber_GetSize(void)
{
	return sizeof(XClimber);
}
