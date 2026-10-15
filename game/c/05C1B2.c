/* Tusk_PlayerOnPlatform - is player p present and alive and standing on TUSK's arena floor?
 *
 * self: TUSK.  p: g_Player1 or g_Player2.
 * Returns 1 or 0 (a char). */
#include "xmen2.h"

char Tusk_PlayerOnPlatform(Tusk *self, XPlayer *p)
{
	return IS_ALIVE(p) && p->seg == self->arenaFloor;
}
