/* FireWall_ClassInit - per-level class setup of FIREWALL (class slot +$5E; the parent's isn't
 * called): nothing but a call of the empty FireWall_EmptyStub.
 *
 * Its arguments (0, the class's index in the level's class list) are ignored. */
#include "xmen2.h"

void FireWall_ClassInit(void)
{
	FireWall_EmptyStub();
}
