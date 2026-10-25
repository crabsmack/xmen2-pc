/* Exodus_CheckFloorHit - when EXODUS's swoop blast has hit the floor segment it was sent to
 * break (breakSeg): break it.
 *
 * Segment A (left): it gets flag $40, A's link to the next segment and B's link to the previous
 * one are cut ($FF), the players standing on it fall (Exodus_DropPlayerFromSeg) and the left
 * floor tiles are drawn broken (Exodus_BreakFloorTiles(1)). Segment B the same the other way.
 * Then breakSeg = 0. Nothing without an EXODUS (g_ExodusObj) or a segment to break. */
#include "xmen2.h"

#define EXODUS	((Exodus *)g_ExodusObj)

void Exodus_CheckFloorHit(void)
{
	register unsigned char seg;
	register Seg *a;
	register Seg *b;

	if (g_ExodusObj && (seg = EXODUS->breakSeg)) {
		a = &g_Segs[EXODUS->segA];
		b = &g_Segs[EXODUS->segB];
		if (seg == EXODUS->segA) {
			a->flags |= 0x40;
			a->next = 0xFF;
			b->prev = 0xFF;
			Exodus_DropPlayerFromSeg(&g_Player1, a);
			Exodus_DropPlayerFromSeg(&g_Player2, a);
#if defined(XMEN2_NATIVE)
			P34_Each([&](XPlayer *q) {	/* players 3-4 in play too (R11) */
				Exodus_DropPlayerFromSeg(q, a);
			});
#endif
			Exodus_BreakFloorTiles(1);
		} else if (seg == EXODUS->segB) {
			b->flags |= 0x40;
			b->prev = 0xFF;
			a->next = 0xFF;
			Exodus_DropPlayerFromSeg(&g_Player1, b);
			Exodus_DropPlayerFromSeg(&g_Player2, b);
#if defined(XMEN2_NATIVE)
			P34_Each([&](XPlayer *q) {	/* players 3-4 in play too (R11) */
				Exodus_DropPlayerFromSeg(q, b);
			});
#endif
			Exodus_BreakFloorTiles(0);
		}
		EXODUS->breakSeg = 0;
	}
}
