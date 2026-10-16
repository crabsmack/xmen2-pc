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

#ifdef XMEN2_MPW
	hdr = (MemBlock *)Sys_GetStackPtr();
#else
	(void)Sys_GetStackPtr();
#endif
	hdr = g_HeapTop;
	hdrSize = sizeof(MemBlock);
	block = (char *)hdr + ((hdrSize & 1) + hdrSize);
	hdr->next = g_HeapTop = block + ((size & 1) + size);
	hdr->tag = tag;
	return block;
}
