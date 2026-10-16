/* BBoard_CalcSize - BBOARD slot +$12: the bytes of a board of count entries, 6 + 2 per entry.
 *
 * cls: the class (unused).  count: entries. */
#include "xmen2.h"

long BBoard_CalcSize(Class *cls, short count)
{
	return count * 2 + 6;
}
