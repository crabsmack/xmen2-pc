/* BioBalls_IsDone - has a BIOBALLS object finished (formation set, path or motion done: its done
 * flag)? No object counts as done.
 *
 * self: the object or 0. */
#include "xmen2.h"

char BioBalls_IsDone(BioBalls *self)
{
	return self ? self->done : 1;
}
