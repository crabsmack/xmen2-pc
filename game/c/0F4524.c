/* SentinelsB_SegIsMode3 - a Sentinels floor-segment type hook (type 27): 1 for message 3, else 0.
 *
 * msg: the hook's message (the other arguments are unused). */
#include "xmen2.h"

short SentinelsB_SegIsMode3(short msg)
{
	if (msg == 3)
		return 1;
	return 0;
}
