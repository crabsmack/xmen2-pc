/* FireWall_Reset - FIREWALL's Reset (class slot +$6A): does nothing but call the empty
 * FireWall_EmptyStub. Never runs in the corpus. */
#include "xmen2.h"

void FireWall_Reset(void)
{
	FireWall_EmptyStub();
}
