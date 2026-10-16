/* BaniMazaA1_FrameHook - the Moving Blocks' per-frame hook (g_SceneFrameHook), once per frame
 * (twice in two-player step mode): count the frame and the switch cooldown down, every 8th frame
 * pulse colour 15 of palette line 0 through $E0 $C0 $A0 $80 $A0 $C0 $E0 $C0 (uploaded unless a
 * fade runs), and step the block animations (BaniMazaA1_AnimateBlocks).
 * The colour cases are the fragment BaniMazaA1_FrameHook_PulseColor; _tail is the loop's end. */
#include "xmen2.h"

void BaniMazaA1_FrameHook(void)
{
	register short n = g_TwoPlayerStep ? 2 : 1;

	do {
		g_FrameCounter++;
		if (g_BmA1SwitchTimer)
			g_BmA1SwitchTimer--;
		if (!(g_FrameCounter & 7)) {
			switch ((g_FrameCounter >> 3) & 7) {	/* BaniMazaA1_FrameHook_PulseColor */
			case 0:
				g_PalBase[15] = 0xE0;
				break;
			case 1:
				g_PalBase[15] = 0xC0;
				break;
			case 2:
				g_PalBase[15] = 0xA0;
				break;
			case 3:
				g_PalBase[15] = 0x80;
				break;
			case 4:
				g_PalBase[15] = 0xA0;
				break;
			case 5:
				g_PalBase[15] = 0xC0;
				break;
			case 6:
				g_PalBase[15] = 0xE0;
				break;
			case 7:
				g_PalBase[15] = 0xC0;
				break;
			}
			g_PalBaseDirty = !g_PalFadeActive;
		}
		BaniMazaA1_AnimateBlocks();	/* BaniMazaA1_FrameHook_tail */
	} while (--n);
}
