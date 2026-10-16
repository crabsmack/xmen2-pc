/* AvalonA3_InitSprites - set up the Exodus arena's four effect sprites: scene sprites 1-4 for
 * Art_AvalonA3Spr (frame 0, palette 0) at VRAM tiles g_FgTilesEnd + 0, $10, $20, $30, uploaded
 * now, with their frame delays staggered (0, 1, 2, 3) so they animate out of step
 * (AvalonA3_FrameHook).
 * MPW: the label at the end is where the ROM pops the four calls' arguments (after the delays). */
#include "xmen2.h"

void AvalonA3_InitSprites(void)
{
	register short pal = 0;

	Spr_Init(&g_SceneSprites[1], 1, CODEPTR(Art_AvalonA3Spr), 0, pal, g_FgTilesEnd, 1);
	Spr_Init(&g_SceneSprites[2], 1, CODEPTR(Art_AvalonA3Spr), 0, pal, g_FgTilesEnd + 0x10, 1);
	Spr_Init(&g_SceneSprites[3], 1, CODEPTR(Art_AvalonA3Spr), 0, pal, g_FgTilesEnd + 0x20, 1);
	Spr_Init(&g_SceneSprites[4], 1, CODEPTR(Art_AvalonA3Spr), 0, pal, g_FgTilesEnd + 0x30, 1);
	g_AvalonA3SprDelays[6] = 0;	/* sprite 1 */
	g_AvalonA3SprDelays[4] = 1;	/* sprite 2 */
	g_AvalonA3SprDelays[2] = 2;	/* sprite 3 */
	g_AvalonA3SprDelays[0] = 3;	/* sprite 4 */
end:	;
}
