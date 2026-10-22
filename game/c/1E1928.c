/* Cyclops_BeamVBlankFlicker - the beam's flicker, every V-blank (VDP_FrameUpdate): Cyclops_DrawBeam
 * writes the beam's two piece lists one after the other into the sprite list; this relinks the
 * sprite before them in VRAM so that only one list is shown, the first on even V-blanks, the
 * second on odd ones.
 *
 * Once a frame's sprite list is complete (g_VBlankFrameDone) and a new splice is pending
 * (g_BeamFlickerPending), it becomes the current one (g_BeamFlickerNew -> g_BeamFlickerCur:
 * {slot before the beam, first slot of list 1, first slot of list 2, on}); off: nothing more.
 * Then it keeps the slot's size byte and writes the size and link word of the first list's last
 * sprite (its link: past the second list, set by Cyclops_DrawBeam) to the sprite attribute table
 * (VRAM $F000 + slot * 8 + 2). While on, the slot before the beam is written there with its link
 * to the first list (even V-blanks) or to the second (odd).
 * Hand-written 68000 (no MPW frame, tag asm): this C is the same behaviour (verify). */
#include "xmen2.h"

void Cyclops_BeamVBlankFlicker(void)
{
	short last;

#if defined(XMEN2_NATIVE)
	/* phase 7: the beam in a player 3-4's own table, or past the hardware's 80 slots (the raised
	   stop): not in VRAM's attribute table, so it isn't relinked there (both lists show) */
	if (g_P34BeamTable || (g_BeamFlickerPending ? g_BeamFlickerNew[2] : g_BeamFlickerCur[2]) >= 80) {
		g_BeamFlickerOn = 0;
		return;
	}
#endif
	if (g_VBlankFrameDone && g_BeamFlickerPending) {
		g_BeamFlickerCur[0] = g_BeamFlickerNew[0];
		g_BeamFlickerCur[1] = g_BeamFlickerNew[1];
		g_BeamFlickerCur[2] = g_BeamFlickerNew[2];
		g_BeamFlickerCur[3] = g_BeamFlickerNew[3];
		g_BeamFlickerOn = g_BeamFlickerCur[3];
		if (!g_BeamFlickerOn)
			return;
		g_BeamFlickerSpr[0] = g_SpriteTable[g_BeamFlickerCur[0]].size;
		last = g_BeamFlickerNew[2] - 1;		/* the first list's last sprite (-1: the one below the table) */
		g_BeamFlickerSpr[2] = g_SpriteTable[last].size;
		VDP_CTRL32 = (unsigned long)(unsigned short)(last * 8 + 2) << 16 | 0x70000003;	/* VRAM write $F000 + slot * 8 + 2 */
		VDP_DATA = g_BeamFlickerSpr[2] << 8 | g_BeamFlickerSpr[3];
	}
	if (g_BeamFlickerOn) {
		g_BeamFlickerSpr[1] = g_BeamFlickerCur[2];
		if (!(g_VBlankCount & 1))
			g_BeamFlickerSpr[1] = g_BeamFlickerCur[1];
		VDP_CTRL32 = (unsigned long)(g_BeamFlickerCur[0] * 8 + 2) << 16 | 0x70000003;
		VDP_DATA = g_BeamFlickerSpr[0] << 8 | g_BeamFlickerSpr[1];
	}
}
