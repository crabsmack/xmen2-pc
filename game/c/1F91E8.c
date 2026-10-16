/* DrawList_SetPriority - give draw-list node index a new sort key (from prio, layer and front, as
 * DrawList_Add) and move it to its place in the list.
 *
 * index: the node (0-29).  prio, layer, front: as DrawList_Add (a player's node keeps its key's
 * low bits). The node flags' $400 and $10 follow prio and front.
 * Returns g_DrawListZero (0) for a bad index or a node not in use, else 0. */
#include "xmen2.h"

/* unlink a node from its neighbours */
#define UNLINK(n) \
	if ((n)->next) (n)->next->prev = (n)->prev; \
	if ((n)->prev) (n)->prev->next = (n)->next

char DrawList_SetPriority(register short index, register short prio, char layer, register short front)
{
	register DrawNode *n;
	register DrawNode *p;
	register unsigned short low;
	DrawNode *prev;

	if (index < 0 || index > 29)
		return g_DrawListZero;
	n = &g_DrawNodes[index];
	if (!(n->flags & 0x8000))
		return g_DrawListZero;
	if (n->flags & 8)
		low = n->key & 3;
	else
		low = front ? 0 : 3;
	{
		register unsigned short key = (prio ? 0 : 0x400) | (unsigned char)layer << 2 | low & 3;
		register long diff = key - n->key;

		if (diff) {
			if (diff < 0) {
				if (n->prev) {
					p = n->prev;
					while (p && key < p->key) {
						prev = p;
						p = p->prev;
					}
					if (!p) {
						UNLINK(n);
						if (!n->next)
							g_DrawListTail = n->prev;
						n->next = prev;
						n->prev = prev->prev;
						prev->prev = n;
						if (n->prev)
							n->prev->next = n;
						g_DrawListHead = n;
					} else if (p != n->prev) {
						UNLINK(n);
						if (!n->next)
							g_DrawListTail = n->prev;
						n->prev = p;
						n->next = p->next;
						p->next = n;
						if (n->next)
							n->next->prev = n;
					}
				}
			} else if (n->next) {
				p = n->next;
				while (p && key > p->key) {
					prev = p;
					p = p->next;
				}
				if (!p) {
					UNLINK(n);
					if (!n->prev)
						g_DrawListHead = n->next;
					n->prev = prev;
					n->next = prev->next;
					prev->next = n;
					if (n->next)
						n->next->prev = n;
					g_DrawListTail = n;
				} else if (p != n->next) {
					UNLINK(n);
					if (!n->prev)
						g_DrawListHead = n->next;
					n->next = p;
					n->prev = p->prev;
					p->prev = n;
					if (n->prev)
						n->prev->next = n;
				}
			}
			n->key = key;
			if (prio)
				n->flags |= 0x400;
			else
				n->flags &= ~0x400;
			if (front)
				n->flags |= 0x10;
			else
				n->flags &= ~0x10;
		}
	}
	return 0;
}
