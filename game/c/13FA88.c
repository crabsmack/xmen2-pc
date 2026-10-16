/* Spark_GetSize - the size of a SPARK object (class slot +$06), $9C bytes.
 *
 * Never runs in the corpus. */
#include "xmen2.h"

long Spark_GetSize(void)
{
	return sizeof(Spark);
}
