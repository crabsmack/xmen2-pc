/* Game_Init - one-time game setup (Sys_InitGameVideo): the sound driver started with its four
 * data blocks (Snd_Init), no scenes registered yet, no character chosen for either player. */
#include "xmen2.h"

void Game_Init(void)
{
	Snd_Init(ADDR68(CODEPTR(Snd_1618BC)), ADDR68(CODEPTR(Snd_163BF2)), ADDR68(CODEPTR(Snd_1641BE)),
		ADDR68(CODEPTR(Snd_17B8AA)));
	g_SceneCount = 0;
	g_P1CharIndex = -1;
	g_P2CharIndex = -1;
}
