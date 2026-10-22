/* Music_SetPlayerParams - tell the sound driver who is playing: parameter $A = the number of
 * players in the game (0-2), $B and $C = player 1's and player 2's character voice
 * (Player_MusicVoice: 1-7, 0 = not playing). The stage music follows them. */
#include "xmen2.h"

void Music_SetPlayerParams(void)
{
	register short voice1 = Player_MusicVoice(&g_Player1);
	register short voice2 = Player_MusicVoice(&g_Player2);

#if defined(XMEN2_NATIVE)
	/* phase 7 R9: players 3-4 count too (at most 2: the second sound machine plays their parts) */
	Snd_SetParam(0xA, (char)P34_MusicCount((voice1 ? 1 : 0) + (voice2 ? 1 : 0)));
#else
	Snd_SetParam(0xA, (char)((voice1 ? 1 : 0) + (voice2 ? 1 : 0)));	/* players in the game */
#endif
	Snd_SetParam(0xB, voice1);
	Snd_SetParam(0xC, voice2);
}
