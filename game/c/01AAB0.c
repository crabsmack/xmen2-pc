/* Tower_ClassInit - per-level class setup of TOWER (class slot +$5E): only calls the empty
 * Tower_NopStatic. The parent's isn't called. */
#include "xmen2.h"

void Tower_ClassInit(void)
{
	Tower_NopStatic();
}
