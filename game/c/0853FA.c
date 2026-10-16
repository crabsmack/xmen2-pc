/* Teleporter_NoPause - the Start-press hook while a teleport runs (g_PauseHandler, set by
 * Teleporter_StateTeleport, back to Pause_Check in Teleporter_StateWaitFade): nothing, so the
 * game can't be paused during the fade. */
#include "xmen2.h"

void Teleporter_NoPause(void)
{
}
