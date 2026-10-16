/* VDP_ResetDmaQueue - empty the V-blank DMA queue (g_DmaQueueCount = 0; Sys_InitHardware). */
#include "xmen2.h"

void VDP_ResetDmaQueue(void)
{
	g_DmaQueueCount = 0;
}
