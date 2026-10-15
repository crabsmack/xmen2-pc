/* BigBio_StateDefeated - BIGBIO defeated (after the hit blink): open the arena wall, end the scene
 * in 200 frames, a spiral chain explosion from a new plain ACTOR at its position (high priority,
 * centred on its body box top), death animation and sound $F9, then the orbs (BigBio_StateDeathOrbs)
 * once the animation has played.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BigBio_StateDefeated(register BigBio *self)
{
	register Actor *fx;

	self->flags6 &= ~0x40;
	Seg_Free(&g_Segs[self->wallSeg]);
	g_SceneExitTimer = 200;
	fx = (Actor *)(*CLASS(Tbl_Actor_vtable)->method[2])(0, CODEPTR(Tbl_Actor_vtable),	/* New */
		self->x, self->y, 0);
	fx->spr.flags |= 4;
	fx->boxTop = self->boxTop;
	ExplodeFx_Chain(fx, CODEPTR(Tbl_ExplodeFxSpiralPath), 0, 0, fx->boxTop - (fx->y >> 8));
	Obj_SetAnim(self, CODEPTR(Tbl_Anim_1206CA));
	Snd_Stop(0xF9);
	Snd_Play(0xF9);
	SET_STATE_WAIT(self, BigBio_StateDeathOrbs, 0xFF);
	return self->flags & 8;
}
