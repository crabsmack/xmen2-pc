/* Cerebro_LoadFonts - load the Cerebro font (Art_CerebroFont, Nemesis) five times with its
 * colours remapped (Art_LoadToVramRecolor: colour 0 stays 0, $E and $F become a pair of palette
 * entries): VRAM tiles $6C0 (14, 15), $680 (12, 13), $640 (10, 11), $600 (8, 9), $5C0 (7, 7). These
 * are the text styles Cerebro_PrintText picks; the palette animation (Cerebro_AnimatePalettes)
 * cycles the entries. */
#include "xmen2.h"

void Cerebro_LoadFonts(void)
{
	Art_LoadToVramRecolor(CODEPTR(Art_CerebroFont), 0x6C0, 0, 14, 15);
	Art_LoadToVramRecolor(CODEPTR(Art_CerebroFont), 0x680, 0, 12, 13);
	Art_LoadToVramRecolor(CODEPTR(Art_CerebroFont), 0x640, 0, 10, 11);
	Art_LoadToVramRecolor(CODEPTR(Art_CerebroFont), 0x600, 0, 8, 9);
	Art_LoadToVramRecolor(CODEPTR(Art_CerebroFont), 0x5C0, 0, 7, 7);
}
