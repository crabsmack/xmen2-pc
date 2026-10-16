/* BioBullet_GetSize - the size of a BIOBULLET object (class slot +$06), $8C bytes (an ACTOR
 * with no fields of its own). */
#include "xmen2.h"

long BioBullet_GetSize(void)
{
	return sizeof(BioBullet);
}
