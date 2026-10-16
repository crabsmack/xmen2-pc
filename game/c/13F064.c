/* Pop_Reset - class slot +$6A of POP: only calls the empty Pop_NopStatic.
 *
 * self: the object (unused). Never runs in the corpus. */
#include "xmen2.h"

void Pop_Reset(Pop *self)
{
	Pop_NopStatic();
}
