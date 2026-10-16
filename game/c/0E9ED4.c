/* Hologram_StartDyingScript - HOLOGRAM: start the dying script (Tbl_Hologram_ScriptDying:
 * Hologram_ScriptDying, 3 steps SeqA_Pose1, SeqB_Pose3, ExplodeWhenReady) from its first step
 * at once.
 *
 * self: the hologram (Hologram_BeginDying makes this its state and default state when the
 * core shields are all down). */
#include "xmen2.h"

void Hologram_StartDyingScript(register Hologram *self)
{
	AiScript_Set(&self->ai, CODEPTR(Tbl_Hologram_ScriptDying));
	AI_FIRST_STEP(self, self->ai, 0);
}
