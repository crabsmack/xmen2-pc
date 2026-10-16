/* BBoard_Init - BBOARD Init (class slot +$0E): count entries, all 0 (every switch off).
 *
 * self: the board.  count: entries. */
#include "xmen2.h"

void BBoard_Init(register BBoard *self, register short count)
{
	register short i;

	self->count = count;
	for (i = 0; i < count; i++)
		self->entry[i] = 0;
}
