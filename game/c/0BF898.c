/* SavageJungle_InitPalPhases - the jungle's palette phases for this level: the list at the
 * offset in word +$12 of the BG map blob (g_LevelBgMap) starts with a count; when it is 1 the
 * phase words after it become g_JunglePalScript and phase 0 starts now (phase and timer 0),
 * else there are no phases (g_JunglePalScript 0). */
#include "xmen2.h"

void SavageJungle_InitPalPhases(void)
{
	register s16 *script;
	register s16 *list;
	register short n;

	list = (s16 *)((u8 *)g_LevelBgMap + ((u16 *)g_LevelBgMap)[9]);
	script = list + 1;
	n = *list;
	if (n != 1)
		g_JunglePalScript = 0;
	else {
		g_JunglePalScript = script;
		g_JunglePalPhase = 0;
		g_JunglePalTimer = 0;
	}
}
