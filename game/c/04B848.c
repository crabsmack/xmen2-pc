/* Apocalypse_StateBombsDone - APOCALYPSE state: once no bomb is live, the next part of the
 * fight in 60 frames. After the 3rd, 7th and 12th destroyed hittable bomb it switches script
 * (Tbl_Apocalypse_ScriptB: the 10 steps of Apocalypse_ScriptB and Apocalypse_ScriptC;
 * Tbl_ApocalypsePhase7Script; Tbl_ApocalypsePhase12Script) from its first step; otherwise it
 * goes on with the current one.
 *
 * self: the boss. */
#include "xmen2.h"

#define g_ApocBombCount		(*(s16 *)g_SceneVar3B08)	/* scene variable: live bombs */
#define g_ApocBombsDestroyed	(*(s16 *)g_SceneVar3B0C)	/* scene variable: hittable bombs destroyed (max 12) */

void Apocalypse_StateBombsDone(register Apocalypse *self)
{
	if (!g_ApocBombCount) {
		if (g_ApocBombsDestroyed == 3) {
			AiScript_Set(&self->ai, CODEPTR(Tbl_Apocalypse_ScriptB));
			AI_FIRST_STEP(self, self->ai, 60);
		} else if (g_ApocBombsDestroyed == 7) {
			AiScript_Set(&self->ai, CODEPTR(Tbl_ApocalypsePhase7Script));
			AI_FIRST_STEP(self, self->ai, 60);
		} else if (g_ApocBombsDestroyed == 12) {
			AiScript_Set(&self->ai, CODEPTR(Tbl_ApocalypsePhase12Script));
			AI_FIRST_STEP(self, self->ai, 60);
		} else {
			AI_NEXT_STEP(self, self->ai, 60);
		}
	}
}
