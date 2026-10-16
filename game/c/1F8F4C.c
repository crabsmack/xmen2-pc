/* DrawList_AllocNode - a free node of the draw list (flags 0), searched from the allocation cursor
 * round the 30 nodes; the cursor moves past the node found.
 *
 * Returns the node, or 0 when all 30 are in use. */
#include "xmen2.h"

DrawNode *DrawList_AllocNode(void)
{
	register DrawNode *n = g_DrawNodeCursor;
	register DrawNode *last = &g_DrawNodes[29];

	while (n->flags != 0) {
		n++;
		if (n > last)
			n = g_DrawNodes;
		if (n == g_DrawNodeCursor)
			return 0;
	}
	g_DrawNodeCursor = n + 1;
	if (last < g_DrawNodeCursor)
		g_DrawNodeCursor = g_DrawNodes;
	return n;
}
