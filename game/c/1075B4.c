/* SpaceB1_LoadLevel - load the Maintenance Shaft (SpaceB1): its level data through Level_Load (FG
 * map and tiles Lvl_SpaceB1Fg, BG map Lvl_SpaceB1BgMap, BG tiles Lvl_SavageC1Bg, FG floor segments
 * Lvl_SpaceB1FgSegs, no BG segments, flags 1 0 1 1, class-init argument $F, music 2, grid shifts 8
 * and 8, class list Tbl_SpaceB1ClassList), then palette colour 0 $A84, the HUD on, the floor-segment
 * type 28 hook SpaceB1_Seg28Handler, plane B not wrapping in y, scroll factors $4D, plane A y base
 * $1960, plane B y base $7A0, x bases 0, and the start camera offsets ($82, y $BE). */
#include "xmen2.h"

void SpaceB1_LoadLevel(void)
{
	Level_Load(CODEPTR(Lvl_SpaceB1Fg), CODEPTR(Lvl_SpaceB1Fg), CODEPTR(Lvl_SpaceB1BgMap),
		CODEPTR(Lvl_SavageC1Bg), CODEPTR(Lvl_SpaceB1FgSegs), 0, 1, 0, 1, 1, 0xF, 2, 8, 8, 0,
		CODEPTR(Tbl_SpaceB1ClassList));
	g_PalBase[0] = 0xA84;
	g_HudOn = 1;
	g_SegTypeHooks[28] = SpaceB1_Seg28Handler;
	g_BgWrapY = 0;
	g_BgScrollFactorY = 0x4D;
	g_BgScrollFactorX = 0x4D;
	g_FgScrollBaseY = 0x1960;
	g_BgScrollBaseY = 0x7A0;
	g_FgScrollBaseX = 0;
	g_BgScrollBaseX = 0;
	g_LevelLoadWord3898 = 0x82;
	g_StartCamOffsetY = 0xBE;
}
