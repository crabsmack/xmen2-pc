/* Cyclops_ResetBeams - at the end of a scene (Game_SceneLoop, Game_PlayAttractDemo): the first live
 * player who is Cyclops (player 1 before player 2), if his beam is out, ends it
 * (Cyclops_EndBeam) and the beam's V-blank flicker is reset (Cyclops_ResetBeamFlicker). */
#include "xmen2.h"

void Cyclops_ResetBeams(void)
{
	register Cyclops *p = 0;

	if (IS_ALIVE(&g_Player1) && g_Player1.cls == CLASS(Tbl_Cyclops_vtable))
		p = (Cyclops *)&g_Player1;
	else if (IS_ALIVE(&g_Player2) && g_Player2.cls == CLASS(Tbl_Cyclops_vtable))
		p = (Cyclops *)&g_Player2;
	if (p && p->beam.def) {
		Cyclops_EndBeam(p);
		Cyclops_ResetBeamFlicker();
	}
}
