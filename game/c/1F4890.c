/* unk_Sys_1F4890 - the address of Tbl_A5WorldInfo, the 20-byte A5-world parameter block.
 *
 * Returns it in a0 (hand-written 68000: lea/rts). Its only caller, Sys_InitHardware, ignores
 * it; what the block was for is not known. */
#include "xmen2.h"

void *unk_Sys_1F4890(void)
{
	return (void *)Tbl_A5WorldInfo;
}
