/* DrawList_Add - put a draw call for an object into the draw list, in sort-key order (lowest first:
 * DrawList_DrawAll runs them in that order, so lower keys get the first, frontmost sprites).
 *
 * draw: func(obj) (the object's class Draw, usually).  obj: the object (0: none).
 * prio: the sprite's priority bit (spr.flags & 4): without it the key gets $400 (drawn later).
 * layer: its draw layer (key bits 2-9).  front: (flags6 & 8) other objects' key low bits 0
 * instead of 3; players get 1 / 2 (and node flags 8).  flag4: kept as node flags 4.
 * The node flags get $8000 (in use), $400 with prio, $10 with front. Returns the node's index
 * (0-29: the object keeps it in drawNode), -1 when the list is full. */
#include "xmen2.h"

#define p	((DrawNode *)obj)	/* obj, reused to walk the list */

short DrawList_Add(AnyFn draw, register void *obj, register short prio, char layer,
	register short front, char flag4)
{
	register DrawNode *n;
	register long low;
	register short index;
	DrawNode *prev;

	n = DrawList_AllocNode();
	if (n) {
#ifdef XMEN2_MPW
		n->draw = (DrawFn)(draw ? draw : 0);
#else
		n->draw = (DrawFn)draw;
#endif
		n->flags = 0x8000 | (prio ? 0x400 : 0) | (front ? 0x10 : 0) | (flag4 ? 4 : 0);
		if (obj) {
			n->object = obj;
			if ((XPlayer *)obj == &g_Player1) {
				n->flags |= 8;
				low = 1;
			} else if ((XPlayer *)obj == &g_Player2) {
				n->flags |= 8;
				low = 2;
			} else
				low = front ? 0 : 3;
		} else {
			low = front ? 0 : 3;
			n->object = 0;
		}
		{
			register unsigned short key = (prio ? 0 : 0x400) | (unsigned char)layer << 2 | low & 3;

			n->key = key;
			/* the walk reuses obj (p), as the original did: it decides the registers */
			obj = g_DrawListHead;
			if (p) {
				while (p && key > p->key) {
					prev = p;
					obj = p->next;
				}
				if (!p) {
					n->prev = prev;
					n->next = prev->next;
					prev->next = n;
					if (n->next)
						n->next->prev = n;
					g_DrawListTail = n;
					if (!prev->prev)
						g_DrawListHead = prev;
				} else {
					n->next = p;
					n->prev = p->prev;
					p->prev = n;
					if (n->prev)
						n->prev->next = n;
					if (!n->prev)
						g_DrawListHead = n;
				}
			} else {
				g_DrawListHead = g_DrawListTail = n;
				n->next = n->prev = 0;
			}
		}
		index = (short)(n - g_DrawNodes);
	} else
		index = -1;
	return index;
}
