/* AvalonA4_AnimatePalettes - the Throne Room's two palette animations, one tick per call (two in
 * two-player mode, g_TwoPlayerStep: the delays count down twice).
 *
 * Pulse, every 5 frames: colours $2A-$2D of g_PalBase run a 6-step magenta pulse ($828 $A2A
 * $C2C $E4E $C2C $A2A); after each run the next byte of Tbl_AvalonA4PalPulse (a step count,
 * then one byte per run; g_AvalonA4PalStep, back to the first run after the count) says how:
 * nonzero = all four colours together, 0 = with a white ($EEE) highlight running across them.
 * Glow, every 4 frames: colours $24-$27 cycle a 15-step green table (brighter on steps 2, 4
 * and 10). Both mark g_PalBaseDirty.
 * The jump tables of the three switches are the fragments AvalonA4_AnimatePalettes_part1-6
 * (part1/part3/part5 the cases, part2 the highlight switch, part4/part6 the step counts).
 * Returns nothing (no caller reads d0). */
#include "xmen2.h"

#define g_A4PulseDelay	(*(s16 *)g_SceneVar3B32)	/* scene variable: frames before the next pulse step */
#define g_A4PulseStep	(*(s16 *)g_SceneVar3B3A)	/* scene variable: step of the 6-step pulse */
#define g_A4GlowDelay	(*(s16 *)g_SceneVar3B34)	/* scene variable: frames before the next glow step */
#define g_A4GlowStep	(*(s16 *)g_SceneVar3B3C)	/* scene variable: step of the 15-step glow */

void AvalonA4_AnimatePalettes(void)
{
	register s8 *pulse;

	g_A4PulseDelay--;
	if (g_TwoPlayerStep)
		g_A4PulseDelay--;
	if (g_A4PulseDelay <= 0) {
		pulse = CODEPTR(Tbl_AvalonA4PalPulse);
		g_A4PulseDelay = 5;
		g_PalBaseDirty = 1;
		if (g_AvalonA4PalStep == *pulse)
			g_AvalonA4PalStep = 0;
		if (*(pulse + g_AvalonA4PalStep + 1)) {
			/* AvalonA4_AnimatePalettes_part1: the four colours together */
			switch (g_A4PulseStep) {
			case 0:
				g_PalBase[0x2A] = 0x828;
				g_PalBase[0x2B] = 0x828;
				g_PalBase[0x2C] = 0x828;
				g_PalBase[0x2D] = 0x828;
				break;
			case 1:
				g_PalBase[0x2A] = 0xA2A;
				g_PalBase[0x2B] = 0xA2A;
				g_PalBase[0x2C] = 0xA2A;
				g_PalBase[0x2D] = 0xA2A;
				break;
			case 2:
				g_PalBase[0x2A] = 0xC2C;
				g_PalBase[0x2B] = 0xC2C;
				g_PalBase[0x2C] = 0xC2C;
				g_PalBase[0x2D] = 0xC2C;
				break;
			case 3:
				g_PalBase[0x2A] = 0xE4E;
				g_PalBase[0x2B] = 0xE4E;
				g_PalBase[0x2C] = 0xE4E;
				g_PalBase[0x2D] = 0xE4E;
				break;
			case 4:
				g_PalBase[0x2A] = 0xC2C;
				g_PalBase[0x2B] = 0xC2C;
				g_PalBase[0x2C] = 0xC2C;
				g_PalBase[0x2D] = 0xC2C;
				break;
			case 5:
				g_PalBase[0x2A] = 0xA2A;
				g_PalBase[0x2B] = 0xA2A;
				g_PalBase[0x2C] = 0xA2A;
				g_PalBase[0x2D] = 0xA2A;
				break;
			}
		} else {
			/* AvalonA4_AnimatePalettes_part2 (the switch) and _part3 (its cases): a white
			   highlight runs across the four colours */
			switch (g_A4PulseStep) {
			case 0:
				g_PalBase[0x2A] = 0x828;
				g_PalBase[0x2B] = 0x828;
				g_PalBase[0x2C] = 0x828;
				g_PalBase[0x2D] = 0x828;
				break;
			case 1:
				g_PalBase[0x2A] = 0xEEE;
				g_PalBase[0x2B] = 0xA2A;
				g_PalBase[0x2C] = 0xA2A;
				g_PalBase[0x2D] = 0xA2A;
				break;
			case 2:
				g_PalBase[0x2A] = 0xC2C;
				g_PalBase[0x2B] = 0xEEE;
				g_PalBase[0x2C] = 0xC2C;
				g_PalBase[0x2D] = 0xC2C;
				break;
			case 3:
				g_PalBase[0x2A] = 0xE4E;
				g_PalBase[0x2B] = 0xE4E;
				g_PalBase[0x2C] = 0xEEE;
				g_PalBase[0x2D] = 0xE4E;
				break;
			case 4:
				g_PalBase[0x2A] = 0xC2C;
				g_PalBase[0x2B] = 0xC2C;
				g_PalBase[0x2C] = 0xC2C;
				g_PalBase[0x2D] = 0xEEE;
				break;
			case 5:
				g_PalBase[0x2A] = 0xA2A;
				g_PalBase[0x2B] = 0xA2A;
				g_PalBase[0x2C] = 0xA2A;
				g_PalBase[0x2D] = 0xA2A;
				break;
			}
		}
		/* AvalonA4_AnimatePalettes_part4: next step; after the sixth, the next table byte */
		++g_A4PulseStep;
		if (g_A4PulseStep == 6) {
			g_A4PulseStep = 0;
			g_AvalonA4PalStep++;
		}
	}
	g_A4GlowDelay--;
	if (g_TwoPlayerStep)
		g_A4GlowDelay--;
	if (g_A4GlowDelay <= 0) {
		g_A4GlowDelay = 4;
		g_PalBaseDirty = 1;
		/* AvalonA4_AnimatePalettes_part5: the glow's cases */
		switch (g_A4GlowStep) {
		case 0:
			g_PalBase[0x24] = 0x82;
			g_PalBase[0x25] = 0xA4;
			g_PalBase[0x26] = 0xA8;
			g_PalBase[0x27] = 0xCA;
			break;
		case 1:
			g_PalBase[0x24] = 0x82;
			g_PalBase[0x25] = 0xA4;
			g_PalBase[0x26] = 0xA8;
			g_PalBase[0x27] = 0xCA;
			break;
		case 2:
			g_PalBase[0x24] = 0xA4;
			g_PalBase[0x25] = 0xA8;
			g_PalBase[0x26] = 0xCA;
			g_PalBase[0x27] = 0xEC;
			break;
		case 3:
			g_PalBase[0x24] = 0x82;
			g_PalBase[0x25] = 0xA4;
			g_PalBase[0x26] = 0xA8;
			g_PalBase[0x27] = 0xCA;
			break;
		case 4:
			g_PalBase[0x24] = 0xA4;
			g_PalBase[0x25] = 0xA8;
			g_PalBase[0x26] = 0xCA;
			g_PalBase[0x27] = 0xEC;
			break;
		case 5:
			g_PalBase[0x24] = 0x82;
			g_PalBase[0x25] = 0xA4;
			g_PalBase[0x26] = 0xA8;
			g_PalBase[0x27] = 0xCA;
			break;
		case 6:
			g_PalBase[0x24] = 0x82;
			g_PalBase[0x25] = 0xA4;
			g_PalBase[0x26] = 0xA8;
			g_PalBase[0x27] = 0xCA;
			break;
		case 7:
			g_PalBase[0x24] = 0x82;
			g_PalBase[0x25] = 0xA4;
			g_PalBase[0x26] = 0xA8;
			g_PalBase[0x27] = 0xCA;
			break;
		case 8:
			g_PalBase[0x24] = 0x82;
			g_PalBase[0x25] = 0xA4;
			g_PalBase[0x26] = 0xA8;
			g_PalBase[0x27] = 0xCA;
			break;
		case 9:
			g_PalBase[0x24] = 0x82;
			g_PalBase[0x25] = 0xA4;
			g_PalBase[0x26] = 0xA8;
			g_PalBase[0x27] = 0xCA;
			break;
		case 10:
			g_PalBase[0x24] = 0xA4;
			g_PalBase[0x25] = 0xA8;
			g_PalBase[0x26] = 0xCA;
			g_PalBase[0x27] = 0xEC;
			break;
		case 11:
			g_PalBase[0x24] = 0x82;
			g_PalBase[0x25] = 0xA4;
			g_PalBase[0x26] = 0xA8;
			g_PalBase[0x27] = 0xCA;
			break;
		case 12:
			g_PalBase[0x24] = 0x82;
			g_PalBase[0x25] = 0xA4;
			g_PalBase[0x26] = 0xA8;
			g_PalBase[0x27] = 0xCA;
			break;
		case 13:
			g_PalBase[0x24] = 0x82;
			g_PalBase[0x25] = 0xA4;
			g_PalBase[0x26] = 0xA8;
			g_PalBase[0x27] = 0xCA;
			break;
		case 14:
			g_PalBase[0x24] = 0x82;
			g_PalBase[0x25] = 0xA4;
			g_PalBase[0x26] = 0xA8;
			g_PalBase[0x27] = 0xCA;
			break;
		}
		/* AvalonA4_AnimatePalettes_part6 */
		++g_A4GlowStep;
		if (g_A4GlowStep == 15)
			g_A4GlowStep = 0;
	}
}
