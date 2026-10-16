/* Mem_StackAlloc - take size bytes from the top of the heap (a stack: Mem_StackFree gives the top
 * block back), behind an 8-byte header {the new heap top, tag}.
 *
 * size: bytes (an odd size is rounded up).  tag: a C string naming the block.
 * Returns the block (after its header). The stack pointer is read first and never used. */
#include "xmen2.h"

void *Mem_StackAlloc(register long size, char *tag)
{
	register MemBlock *hdr;
	register short hdrSize;
	register char *block;

	hdr = (MemBlock *)Sys_GetStackPtr();
	hdr = g_HeapTop;
	hdrSize = 8;
	block = (char *)hdr + (hdrSize + (hdrSize & 1));
	hdr->next = g_HeapTop = block + (size + (size & 1));
	hdr->tag = tag;
	return block;
}
