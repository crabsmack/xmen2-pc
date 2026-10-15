/* HornetBullet_GetSize - the size of a HORNETBULLET object (class slot +$06), $8C bytes (an ACTOR
 * with no fields of its own). Never runs in the corpus. */
#include "xmen2.h"

long HornetBullet_GetSize(void)
{
	return sizeof(HornetBullet);
}
