/* BaniMazaA1_AnimateBlocks - step the Moving Blocks' three block animations (scene sprites 0-3,
 * 4-7, 8-11): every 6 frames each group shows the next of its 8 frames (BaniMazaA1_UploadBlockTiles),
 * counted 7 down to 0, or played the other way (7 - frame) while its rolling-disc band runs
 * reversed (g_DiscBandReverse). */
#include "xmen2.h"

void BaniMazaA1_AnimateBlocks(void)
{
	register u8 *rev = g_DiscBandReverse;

	if (!g_BmA1BlockTimer0) {
		g_BmA1BlockTimer0 = 6;
		if (!g_BmA1BlockFrame0)
			g_BmA1BlockFrame0 = 8;
		--g_BmA1BlockFrame0;
		BaniMazaA1_UploadBlockTiles(&g_SceneSprites[0], rev[0] ? 7 - g_BmA1BlockFrame0 : g_BmA1BlockFrame0);
	}
	--g_BmA1BlockTimer0;
	if (!g_BmA1BlockTimer1) {
		g_BmA1BlockTimer1 = 6;
		if (!g_BmA1BlockFrame1)
			g_BmA1BlockFrame1 = 8;
		--g_BmA1BlockFrame1;
		BaniMazaA1_UploadBlockTiles(&g_SceneSprites[4], rev[1] ? 7 - g_BmA1BlockFrame1 : g_BmA1BlockFrame1);
	}
	--g_BmA1BlockTimer1;
	if (!g_BmA1BlockTimer2) {
		g_BmA1BlockTimer2 = 6;
		if (!g_BmA1BlockFrame2)
			g_BmA1BlockFrame2 = 8;
		--g_BmA1BlockFrame2;
		BaniMazaA1_UploadBlockTiles(&g_SceneSprites[8], rev[2] ? 7 - g_BmA1BlockFrame2 : g_BmA1BlockFrame2);
	}
	--g_BmA1BlockTimer2;
}
