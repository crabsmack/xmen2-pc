/* Sys_ReturnTrue - returns 1 (reached only through its jump-table entry: a hook that always says
 * yes). */
#include "xmen2.h"

short Sys_ReturnTrue(void)
{
	return 1;
}
