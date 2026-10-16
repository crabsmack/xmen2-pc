/* Player_CalcMaxTiles - the most VRAM tiles any player character's sprite set needs (Spr_Init
 * with no frame gives a definition's largest frame): g_PlayerTiles, each player's VRAM area
 * (Scene_PostLoadVram).
 *
 * The sets: Art_PlayerSpr plus Art_PlayerSpr2 (twice), the single definitions, some with extra
 * tiles for effects ($10, $D, $34 for Cyclops' beam). Returns the largest. */
#include "xmen2.h"

short Player_CalcMaxTiles(void)
{
	SprInst tmp;
	register short base, max;
	register short t;

	max = 0;
	base = 0;
	base = Spr_Init(&tmp, 1, CODEPTR(Art_PlayerSpr), -1, 1, 0, 0);
	t = Spr_Init(&tmp, 1, CODEPTR(Art_PlayerSpr2), -1, 1, 0, 0);
	t += base;
	max = MAX(max, t);
	t = Spr_Init(&tmp, 1, CODEPTR(Art_PlayerSpr2), -1, 1, 0, 0);
	t += base;
	max = MAX(max, t);
	t = Spr_Init(&tmp, 1, CODEPTR(Art_EvilWolverineSpr), -1, 1, 0, 0);
	max = MAX(max, t);
	t = Spr_Init(&tmp, 1, CODEPTR(Art_PlayerSpr3), -1, 1, 0, 0);
	max = MAX(max, t);
	t = Spr_Init(&tmp, 1, CODEPTR(Art_EvilPsylockeSpr), -1, 1, 0, 0);
	t += 0x10;
	max = MAX(max, t);
	t = Spr_Init(&tmp, 1, CODEPTR(Art_PlayerSpr6), -1, 1, 0, 0);
	t += 0x10;
	max = MAX(max, t);
	t = Spr_Init(&tmp, 1, CODEPTR(Art_PlayerSpr7), -1, 1, 0, 0);
	t += 0x10;
	max = MAX(max, t);
	t = Spr_Init(&tmp, 1, CODEPTR(Art_EvilNightcrawlerSpr), -1, 1, 0, 0);
	max = MAX(max, t);
	t = Spr_Init(&tmp, 1, CODEPTR(Art_PlayerSpr4), -1, 1, 0, 0);
	max = MAX(max, t);
	t = Spr_Init(&tmp, 1, CODEPTR(Art_PlayerSpr5), -1, 1, 0, 0);
	max = MAX(max, t);
	t = Spr_Init(&tmp, 1, CODEPTR(Art_MagnetoBossSpr2), -1, 1, 0, 0);
	max = MAX(max, t);
	t = Spr_Init(&tmp, 1, CODEPTR(Art_EvilMagnetoSpr), -1, 1, 0, 0);
	max = MAX(max, t);
	t = Spr_Init(&tmp, 1, CODEPTR(Art_MagnetoBossSpr3), -1, 1, 0, 0);
	max = MAX(max, t);
	t = Spr_Init(&tmp, 1, CODEPTR(Art_EvilGambitSpr), -1, 1, 0, 0);
	t += 0xD;
	max = MAX(max, t);
	t = Spr_Init(&tmp, 1, CODEPTR(Art_PlayerSpr8), -1, 1, 0, 0);
	t += 0xD;
	max = MAX(max, t);
	t = Spr_Init(&tmp, 1, CODEPTR(Art_PlayerSpr9), -1, 1, 0, 0);
	t += 0xD;
	max = MAX(max, t);
	t = Spr_Init(&tmp, 1, CODEPTR(Art_CyclopsSpr), -1, 1, 0, 0);
	t += 0x34;
	max = MAX(max, t);
	return max;
}
