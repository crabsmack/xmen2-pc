/* HitList_AllocNode - take a free hit-list node from the pool (one without an owner) and clear
 * its links. Returns the node, or 0 when all 50 are in use. */
#include "xmen2.h"

/* Defined in the original's file (see HitList_Init): MPW keeps its address in a register. */
#ifdef XMEN2_MPW
ListNode g_HitNodes[50];
#endif

ListNode *HitList_AllocNode(void)
{
	register ListNode *n = &g_HitNodes[0];
	short i;

	for (i = 0; i < 50; i++) {
		if (!n->owner) {
			n->next = 0;
			n->prev = 0;
			return n;
		}
		n++;
	}
	return 0;
}
