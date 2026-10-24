/* SentinelsA1_RunScene - the scene "SentinelsA1 CoolantCore" (Scene_RegisterAll): load the level,
 * set it up (SentinelsA1_InstallHooks), clear the scene's object pointer, then run the level
 * loop (Level_Run) until the scene ends. */
#include "xmen2.h"

#define g_A1SceneObj	(*(PTR(Actor) *)&g_SceneVar3B20)	/* scene variable: an object pointer (other scenes' camera focus, boss); nothing in the Coolant Core reads it */

void SentinelsA1_RunScene(void)
{
	SentinelsA1_CallEmptyStub();
	SentinelsA1_LoadLevel();
	SentinelsA1_InstallHooks();
	SentinelsA1_CallEmptyStub();
	g_A1SceneObj = 0;
	Level_Run();
}
