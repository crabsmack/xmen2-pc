/* DrawList_DrawAll - run the draw list: draw(object) of every node that has one, in key order
 * (the scene draw hooks call it each frame). */
#include "xmen2.h"

void DrawList_DrawAll(void)
{
	register DrawNode *n;

	for (n = g_DrawListHead; n; n = n->next)
		if (n->draw)
			(*n->draw)(n->object);
}
