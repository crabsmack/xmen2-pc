/* DrawList_Init - empty the draw list: all 30 nodes of g_DrawNodes cleared and free (key $5FF),
 * no head or tail, the allocation cursor at the first node.
 *
 * Returns 0. */
#include "xmen2.h"

long DrawList_Init(void)
{
	register long i;
	register DrawNode *n;
	register DrawNode *nodes;

	g_DrawNodeCursor = nodes = g_DrawNodes;
	n = nodes;
	for (i = 0; i < 30; i++, n++) {
		n->next = n->prev = 0;
		n->draw = 0;
		n->object = 0;
		n->key = 0x5FF;
		n->flags = 0;
	}
	g_DrawListHead = g_DrawListTail = 0;
	g_DrawListZero = 0;
	return 0;
}
