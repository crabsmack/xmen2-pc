/* Pause_RestoreLink - give the PAUSE text's second sprite back the link Pause_LinkSprites saved. */
#include "xmen2.h"

#define g_PauseSavedLink	g_TextWinRight	/* reused: the text sprite's link before the pause */

void Pause_RestoreLink(void)
{
	g_SpriteTable[1].link = (s8)g_PauseSavedLink;
}
