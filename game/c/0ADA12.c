/* TrapDoor_ClassInit - per-level class setup of TRAPDOOR (class slot +$5E): only calls the
 * empty TrapDoor_NopStatic. The parent's isn't called. */
#include "xmen2.h"

void TrapDoor_ClassInit(void)
{
	TrapDoor_NopStatic();
}
