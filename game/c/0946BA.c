/* EvilMaker_StateWaitKills - EVILMAKER's first state: wait until two clones have been killed
 * (g_CloneMakerKills), then let the camera go (camera slot +$3A UnlockAxes), start the
 * clone-maker mode (XClone_StartMakerMode) and run its AI script from step 0.
 *
 * self: EVILMAKER.
 * Returns self->flags & 8 while waiting; no value once the script starts. Step 0 of its script
 * is EvilMaker_ActKill: the maker removes itself on the next update, before any drop. */
#include "xmen2.h"

short EvilMaker_StateWaitKills(EvilMaker *self)
{
	if (g_CloneMakerKills >= 2) {
		(*CLASS(Tbl_Camera_vtable)->method[13])(g_Camera);	/* +$3A UnlockAxes */
		XClone_StartMakerMode();
		AI_FIRST_STEP(self, self->ai, 0);
	} else {
		SET_STATE(self, EvilMaker_StateWaitKills);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
