/* SentinelsA1_OpenExitTiles - open the Coolant Core's exit in the map: FG blocks (17, 22) and
 * (18, 22) become block $ED (Map_SetBlockFG), block (18, 22) gets attribute bit $200, and it is
 * redrawn (Map_RefreshBlock). Called by Hologram_OpenExit. */
#include "xmen2.h"

void SentinelsA1_OpenExitTiles(void)
{
	register s16 *cell;

	Map_SetBlockFG(0x11, 0x16, 0xED, 0);
	Map_SetBlockFG(0x12, 0x16, 0xED, 0);
	cell = &g_FgBlockMap[g_FgRowTable[0x16] + 0x12];
	*cell |= 0x200;
	Map_RefreshBlock(0x12, 0x16);
}
