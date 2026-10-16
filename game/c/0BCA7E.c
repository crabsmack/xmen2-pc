/* SavageBio_ActDie - SAVAGEBIGBIO script action: die. Stop and hide; the BIOBALLS companion
 * takes its place and runs its death script (SavageBio_Script5), and the boss waits for it
 * (SavageBio_StateDeathWait). Without a companion: end the scene in $F0 frames, let the camera
 * go, and blow up along Tbl_ExplodeFxSpiralPath from the body's top.
 *
 * self: the boss (a step of its death script).
 * Returns self->flags & 8 with a companion; nothing in particular without. */
#include "xmen2.h"

short SavageBio_ActDie(SavageBigBio *self)
{
	BioBalls *balls = self->balls;

	STOP_MOVING(self);
	self->anim = 0;
	(*self->cls->method[20])(self, 0xFF);		/* SetFrame: hide */
	if (balls) {
		balls->x = self->x;
		balls->y = self->y;
		if (!(self->spr.flags & 8))
			balls->spr.flags &= ~8;
		else
			balls->spr.flags |= 8;
		if (!(self->spr.flags & 0x10))
			balls->spr.flags &= ~0x10;
		else
			balls->spr.flags |= 0x10;
		BioBalls_SetFormation(balls, CODEPTR(Tbl_SavageBioBallsFormation));
		AiScript_Set(&balls->ai, CODEPTR(Tbl_SavageBio_Script5));
		AI_FIRST_STEP(balls, balls->ai, 0);
		SET_STATE(self, SavageBio_StateDeathWait);
		return self->flags & 8;
	}
	Scene_EndAfter(0xF0);
	*(PTR(Actor) *)g_SceneVar3B20 = 0;		/* the object the SavageA4 camera follows */
	ExplodeFx_Chain(self, CODEPTR(Tbl_ExplodeFxSpiralPath), 0, 0, self->boxTop - (self->y >> 8));
	NO_RETURN_VALUE
}
