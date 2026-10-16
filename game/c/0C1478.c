/* Music_PlayStage - start a stage's music: every sound off, the player parameters
 * (Music_SetPlayerParams), then track `track`: remembered in g_MusicTrack, given to the driver as
 * parameter 0 and played (restarted).
 *
 * track: the sound id of the music. Callers: the RunScenes (CloneA2, SavageB1, ...),
 * SiberiaB1_FrameHook. */
#include "xmen2.h"

void Music_PlayStage(register short track)
{
	Snd_StopAll();
	Music_SetPlayerParams();
	g_MusicTrack = track;
	Snd_SetParam(0, (short)track);
	Snd_Stop(track);
	Snd_Play(track);
}
