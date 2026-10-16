/* PlayerSelect_CharFrameNum - the select-screen frame of a slot's character (<Char>_SelectFrame;
 * cases PlayerSelect_CharFrameNum_Beast .. _Magneto).
 *
 * slot: the player's select slot. Returns $FF with no character (PlayerSelect_CharFrameNum_None). */
#include "xmen2.h"

short PlayerSelect_CharFrameNum(BigSprite *slot)
{
	register short frame;

	switch (slot->charIndex) {
	case 0:
		frame = Beast_SelectFrame();
		break;
	case 1:
		frame = Cyclops_SelectFrame();
		break;
	case 2:
		frame = Gambit_SelectFrame();
		break;
	case 3:
		frame = Nightcrawler_SelectFrame();
		break;
	case 4:
		frame = (short)Psylocke_SelectFrame();
		break;
	case 5:
		frame = Wolverine_SelectFrame();
		break;
	case 6:
		frame = Magneto_SelectFrame();
		break;
	default:
		frame = 0xFF;
		break;
	}
	return frame;
}
