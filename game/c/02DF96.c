/* ExodusBlast_GetSize - the size of an EXODUSBLAST object (class slot +$06), $9A bytes.
 * Never runs in the corpus. */
#include "xmen2.h"

long ExodusBlast_GetSize(void)
{
	return sizeof(ExodusBlast);
}
