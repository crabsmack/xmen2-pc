/* Hologram_VBlankFlicker - the V-blank game hook of the SentinelsA1 core (Hologram_Init installs
 * it after 60 frames, Timer_SetVBlankHookCb): make the hologram flicker by relinking the
 * hardware sprite list in VRAM every V-blank, so its sprites are shown on odd V-blanks only.
 *
 * Once a frame's sprite list is complete (g_VBlankFrameDone) and the flicker is armed
 * (Hologram_Draw, Hologram_SetPose, Hologram_Init), it takes the slots Hologram_Draw left: the
 * slot before the hologram's sprites and the one after them (none: the flicker stays off and
 * stays armed), keeps the first slot's size byte and disarms. Then, while running, it writes
 * that slot's size and link word to the sprite attribute table (VRAM $F000 + slot * 8 + 2):
 * linked to the hologram's first sprite on odd V-blanks, past its last one on even ones.
 * Hand-written 68000 (no MPW frame, tag asm): this C is the same behaviour (verify). */
#include "xmen2.h"

void Hologram_VBlankFlicker(void)
{
	unsigned char link;

	if (g_VBlankFrameDone && g_HoloFlickerArmed) {
		g_HoloFlickerSlots[0] = g_HoloFlickerLinks[0];
		g_HoloFlickerSlots[1] = g_HoloFlickerLinks[1];
		g_HoloFlickerOn = g_HoloFlickerLinks[1];
		if (!g_HoloFlickerOn)
			return;
		g_HoloFlickerSpr[0] = g_SpriteTable[g_HoloFlickerSlots[0]].size;
		g_HoloFlickerArmed = 0;
	}
	if (g_HoloFlickerOn) {
		link = g_HoloFlickerSlots[1];
		if (g_VBlankCount & 1)
			link = g_HoloFlickerSlots[0] + 1;
		g_HoloFlickerSpr[1] = link;
		VDP_CTRL32 = (unsigned long)(g_HoloFlickerSlots[0] * 8 + 2) << 16 | 0x70000003;	/* VRAM write $F000 + slot * 8 + 2 */
		VDP_DATA = g_HoloFlickerSpr[0] << 8 | g_HoloFlickerSpr[1];
	}
}
