/* HUD_Redraw - redraw both HUD panels: a player in play gets its full panel (Art_HUDTiles, the
 * second one $200 bytes on), one out of play the short placeholder (Art_HUDTiles4); then the
 * health bars (HUD_UpdateHealthBars) and a wait for V-blank (the DMA queue goes out). VRAM $FD00
 * player 1, $FA00 player 2. */
#include "xmen2.h"

void HUD_Redraw(void)
{
	register u8 *panel = (u8 *)CODEPTR(Art_HUDTiles);
	register u8 *empty = (u8 *)CODEPTR(Art_HUDTiles4);

	if (IS_ALIVE(&g_Player1))
		VDP_QueueDma(panel, 0xFD00, 0x200);
	else
		VDP_QueueDma(empty, 0xFD00, 0x140);
	if (IS_ALIVE(&g_Player2)) {
		panel += 0x200;
		VDP_QueueDma(panel, 0xFA00, 0x200);
	} else
		VDP_QueueDma(empty, 0xFA00, 0x140);
	HUD_UpdateHealthBars();
	VDP_WaitVBlank();
}
