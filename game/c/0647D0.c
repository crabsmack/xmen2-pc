/* PlayerSelect_LoadCharFrame - render the select-screen frame of a slot's character
 * (<Char>_SelectSpriteDef / _SelectFrame; cases PlayerSelect_LoadCharFrame_Beast .. _Magneto)
 * into the scaler's source bitmap (g_ScaleSrcBitmap, Spr_RenderToBitmap) and make it the slot's
 * big-sprite picture at its own size (Cerebro_BigSpriteInit). No character: nothing
 * (PlayerSelect_LoadCharFrame_part1 is the default and the rendering).
 *
 * slot: the player's select slot. */
#include "xmen2.h"

void PlayerSelect_LoadCharFrame(register BigSprite *slot)
{
	register void *def;
	register short frame;

	switch (slot->charIndex) {
	case 0:
		def = Beast_SelectSpriteDef();
		frame = Beast_SelectFrame();
		break;
	case 1:
		def = Cyclops_SelectSpriteDef();
		frame = Cyclops_SelectFrame();
		break;
	case 2:
		def = Gambit_SelectSpriteDef();
		frame = Gambit_SelectFrame();
		break;
	case 3:
		def = Nightcrawler_SelectSpriteDef();
		frame = Nightcrawler_SelectFrame();
		break;
	case 4:
		def = (void *)Psylocke_SelectSpriteDef();
		frame = (short)Psylocke_SelectFrame();
		break;
	case 5:
		def = Wolverine_SelectSpriteDef();
		frame = Wolverine_SelectFrame();
		break;
	case 6:
		def = Magneto_SelectSpriteDef();
		frame = Magneto_SelectFrame();
		break;
	default:
		def = 0;
		frame = 0xFF;
		break;
	}
	if (def) {
		Spr_RenderToBitmap((SprDef *)def, frame, g_ScaleSrcBitmap);
		Cerebro_BigSpriteInit(slot, g_ScaleSrcBitmap);
	}
}
