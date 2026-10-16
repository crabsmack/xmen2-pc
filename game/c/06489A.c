/* PlayerSelect_CharSpriteDef - the select-screen sprite definition of a slot's character
 * (<Char>_SelectSpriteDef; cases PlayerSelect_CharSpriteDef_Beast .. _Magneto).
 *
 * slot: the player's select slot. Returns 0 with no character (PlayerSelect_CharSpriteDef_None).
 *
 * Inline here, separate blocks in the disassembly: PlayerSelect_CharSpriteDef_Cyclops, PlayerSelect_CharSpriteDef_Nightcrawler, PlayerSelect_CharSpriteDef_Psylocke, PlayerSelect_CharSpriteDef_Wolverine, PlayerSelect_CharSpriteDef_Magneto.
 */
#include "xmen2.h"

void *PlayerSelect_CharSpriteDef(BigSprite *slot)
{
	register void *def;

	switch (slot->charIndex) {
	case 0:
		def = Beast_SelectSpriteDef();
		break;
	case 1:
		def = Cyclops_SelectSpriteDef();
		break;
	case 2:
		def = Gambit_SelectSpriteDef();
		break;
	case 3:
		def = Nightcrawler_SelectSpriteDef();
		break;
	case 4:
		def = (void *)Psylocke_SelectSpriteDef();
		break;
	case 5:
		def = Wolverine_SelectSpriteDef();
		break;
	case 6:
		def = Magneto_SelectSpriteDef();
		break;
	default:
		def = 0;
		break;
	}
	return def;
}
