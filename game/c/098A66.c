/* EvilPsylocke_ActDash - EVILPSYLOCKE AI-script action: dash the way it faces at speed $760 (left
 * when h-flipped), slowed by 1/32 of it each frame, with sound $C0 and Psylocke's dash animation
 * (Tbl_PsylockeAnim_2); EvilPsylocke_StateDashEnd at its end.
 *
 * self: the clone.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short EvilPsylocke_ActDash(EvilPsylocke *self)
{
	self->vx = self->spr.flags & 0x10 ? -0x760 : 0x760;
	self->ax = -(self->vx >> 5);
	Snd_Stop(0xC0);
	Snd_Play(0xC0);
	Obj_SetAnim(self, CODEPTR(Tbl_PsylockeAnim_2));
	SET_STATE_WAIT(self, (StateFn)EvilPsylocke_StateDashEnd, 0xFF);
	return self->flags & 8;
}
