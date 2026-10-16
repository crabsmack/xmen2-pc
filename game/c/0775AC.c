/* CloneA1_LoadLevel - load Factory Floor 1 (CloneA1): its level data through Level_Load (FG map and
 * tiles Art_CloneA1Nem, BG map and tiles Art_CloneA1Nem3, FG floor segments Lvl_CloneA1Segs, no BG
 * segments, flags 1 0 1 1, class-init argument $14, no music (-1), grid shifts 8 and 8, class
 * list Tbl_CloneA1ClassList (a class list despite its label: a word count, then CloneA1's class
 * descriptors)); then plane B without wrap, scrolling with the camera in y only, the HUD on,
 * one-player stepping. */
#include "xmen2.h"

void CloneA1_LoadLevel(void)
{
	Level_Load(CODEPTR(Art_CloneA1Nem), CODEPTR(Art_CloneA1Nem), CODEPTR(Art_CloneA1Nem3),
		CODEPTR(Art_CloneA1Nem3), CODEPTR(Lvl_CloneA1Segs), 0, 1, 0, 1, 1, 0x14, -1, 8, 8, 0,
		CODEPTR(Tbl_CloneA1ClassList));
	g_BgWrapY = 0;
	g_BgScrollFactorY = 0x100;
	g_BgScrollFactorX = 0;
	g_HudOn = 1;
	g_TwoPlayerStep = 0;
}
