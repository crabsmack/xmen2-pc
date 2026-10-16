/* Spawn_Nop - the spawn pass of a level without spawn records: nothing (Spawn_InitRecords makes it
 * g_SpawnPassHook, which Level_FrameLoop then runs instead of the record scan). */
#include "xmen2.h"

void Spawn_Nop(void)
{
}
