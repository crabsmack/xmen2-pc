/* BBoard_GetSize - the size of a BBOARD header (class slot +$06), 8 bytes (with one entry; the
 * real size is BBoard_CalcSize's). */
#include "xmen2.h"

long BBoard_GetSize(void)
{
	return sizeof(BBoard);
}
