/* DrawList_Remove - take node index out of the draw list and free it.
 *
 * index: a node index from DrawList_Add (0-29; others, or a node not in use, are ignored).
 * Returns g_DrawListZero (always 0). */
#include "xmen2.h"

/* unlink a node from its neighbours */
#define UNLINK(n) \
	if ((n)->next) (n)->next->prev = (n)->prev; \
	if ((n)->prev) (n)->prev->next = (n)->next

/* The original's file defined g_DrawNodes: MPW C keeps the address of an array defined in the
   compile unit in a register (lea g_DrawNodes(a5),a3, indexed .w; Pal_SaveBase). */
#ifdef XMEN2_MPW
DrawNode g_DrawNodes[30];
#endif

char DrawList_Remove(register short index)
{
	register DrawNode *n;

	if (index < 0 || index > 29)
		return g_DrawListZero;
	n = &g_DrawNodes[index];
	if (!(n->flags & 0x8000))
		return g_DrawListZero;
	n->flags = 0;
	if (!n->prev) {
		if (n->next) {
			UNLINK(n);
			g_DrawListHead = n->next;
		} else
			g_DrawListHead = g_DrawListTail = 0;
	} else if (!n->next) {
		UNLINK(n);
		g_DrawListTail = n->prev;
	} else {
		UNLINK(n);
	}
	return g_DrawListZero;
}
