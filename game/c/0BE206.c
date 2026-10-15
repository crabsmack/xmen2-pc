/* Barbarian_StateToLeapEdge - BARBARIAN state: go on with Barbarian_StateLeapEdge from the next
 * update.
 *
 * self: the object. Never runs in the corpus.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateToLeapEdge(Barbarian *self)
{
	SET_STATE(self, Barbarian_StateLeapEdge);
	return self->flags & 8;
}
