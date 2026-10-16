/* Music_Play - switch the music to track `track` and remember it (g_MusicTrack): every sound
 * off, sound command $A (unk_Snd_1FE5B4), then play it. Track -1 is only recorded.
 *
 * track: the sound id of the music, or -1 for none. */
#include "xmen2.h"

void Music_Play(register short track)
{
	g_MusicTrack = track;
	if (g_MusicTrack != -1) {
		Snd_StopAll();
		unk_Snd_1FE5B4(0xA);
		g_MusicTrack = track;
		Snd_Play((short)track);
	}
}
