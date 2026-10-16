/* Pop_ClassInit - per-level class setup of POP (class slot +$5E): only calls the empty
 * Pop_NopStatic. The parent's isn't called. */
#include "xmen2.h"

void Pop_ClassInit(void)
{
	Pop_NopStatic();
}
