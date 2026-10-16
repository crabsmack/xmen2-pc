/* Hologram_StartAttackScript - HOLOGRAM intro script action (its only step,
 * Tbl_HologramAiScript_0EAEFC: Hologram_ScriptIntro): sound $D0, then the attack script
 * (Tbl_Hologram_ScriptAttack: Hologram_ScriptAttack, 26 steps) from its first step at once.
 *
 * self: the hologram. */
#include "xmen2.h"

void Hologram_StartAttackScript(register Hologram *self)
{
	Snd_Stop(0xD0);
	Snd_Play(0xD0);
	AiScript_Set(&self->ai, CODEPTR(Tbl_Hologram_ScriptAttack));
	AI_FIRST_STEP(self, self->ai, 0);
}
