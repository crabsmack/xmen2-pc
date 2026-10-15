/* Tusk_PlayerToLayer0 - give a present, alive player low priority: sprite priority bit off
 * and draw-list priority 0 (DrawList_SetPriority with its layer and flags6 bit 3).
 *
 * p: g_Player1 or g_Player2. */
#include "xmen2.h"

void Tusk_PlayerToLayer0(XPlayer *p)
{
	if (IS_ALIVE(p)) {
		p->spr.flags &= ~4;
		DrawList_SetPriority(p->drawNode, 0, p->layer, p->flags6 & 8);
	}
}
