/* Mem_StackFree - give back a block of Mem_StackAlloc: the top block is popped (its header becomes
 * the heap top); any other is only tagged "FREE ME" and stays allocated.
 *
 * block: what Mem_StackAlloc returned. */
#include "xmen2.h"

void Mem_StackFree(void *block)
{
	register MemBlock *hdr = (MemBlock *)block;

	hdr--;
	if (hdr->next == g_HeapTop)
		g_HeapTop = hdr;
	else
		hdr->tag = STR(Str_FreeMe, "FREE ME");
}
