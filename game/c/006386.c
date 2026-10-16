/* HUD_UpdateHealthBars - draw the players' HUD meters (the level loop calls it while g_HudDirty):
 * when the number of players in play changed since the HUD was drawn, the whole HUD
 * (HUD_Redraw) instead.
 *
 * For each player in play: the health meter, (health - 1) / $100 + 1 bars (at most 9; 0 for no
 * health; blinking with frame counter bit 4 at 2 bars or fewer): its 128-byte strip of
 * Art_HUDTiles3 to VRAM $FF80 (player 1) / $FC00 (player 2). With the special meter
 * (g_P1HasMeter / g_P2HasMeter): charge / 4 cells, at most 8; full, it blinks (frame counter
 * bit 3) and keeps the HUD dirty; its strip of Art_HUDTiles2 to $FF00 / $FC80. */
#include "xmen2.h"

void HUD_UpdateHealthBars(void)
{
	u8 *healthTiles;
	u8 *meterTiles;
	u8 *strip;
	register XPlayer *p1, *p2;
	register unsigned short bars;
	register short health;

	healthTiles = (u8 *)CODEPTR(Art_HUDTiles3);
	meterTiles = (u8 *)CODEPTR(Art_HUDTiles2);
	g_HudDirty = 0;
	if (g_HudPlayersAlive != g_PlayersAlive) {
		g_HudPlayersAlive = g_PlayersAlive;
		HUD_Redraw();
		return;
	}
	if (IS_ALIVE(&g_Player1)) {
		p1 = &g_Player1;
		if ((health = p1->health) <= 0)
			bars = 0;
		else {
			bars = (health - 1 >> 8) + 1;
			if (bars > 9)
				bars = 9;
			else if (bars <= 2 && (g_FrameCounter & 0x10))
				bars = 0;
		}
		strip = healthTiles + ((long)bars << 7);
		VDP_QueueDma(strip, 0xFF80, 0x80);
		if (g_P1HasMeter) {
			bars = p1->charge >> 2;
			bars = MIN(bars, 8);
			if (bars == 8) {
				if (g_FrameCounter & 8)
					++bars;
				g_HudDirty = 1;
			}
			strip = meterTiles + ((long)bars << 7);
			VDP_QueueDma(strip, 0xFF00, 0x80);
		}
	}
	if (IS_ALIVE(&g_Player2)) {
		p2 = &g_Player2;
		if ((health = p2->health) <= 0)
			bars = 0;
		else {
			bars = (health - 1 >> 8) + 1;
			if (bars > 9)
				bars = 9;
			else if (bars <= 2 && (g_FrameCounter & 0x10))
				bars = 0;
		}
		strip = healthTiles + ((long)bars << 7);
		VDP_QueueDma(strip, 0xFC00, 0x80);
		if (g_P2HasMeter) {
			bars = p2->charge >> 2;
			bars = MIN(bars, 8);
			if (bars == 8) {
				if (g_FrameCounter & 8)
					++bars;
				g_HudDirty = 1;
			}
			strip = meterTiles + ((long)bars << 7);
			VDP_QueueDma(strip, 0xFC80, 0x80);
		}
	}
}
