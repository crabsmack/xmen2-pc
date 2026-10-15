/* Beast_StartRoll - from the crouch, C with Left/Right (Beast_StateCrouch): face that way and leap
 * into a forward roll, if there is room (Beast_RollPathClear); else crouch on.
 *
 * self: the player.
 * The roll: sound $92, frame $96, off the floor (Obj_Detach), gravity $1E, vy -$20C, vx $600 the
 * way he faces (maximum $600, no acceleration), countdown $26; Beast_StateRoll from the next
 * frame. The animation is stopped first either way.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Beast_StartRoll(register Beast *self)
{
	self->anim = 0;
	if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft)
		self->spr.flags |= 0x10;
	else if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight)
		self->spr.flags &= ~0x10;
	if (!Beast_RollPathClear(self)) {
		SET_STATE(self, (StateFn)Beast_StateCrouch);
		return self->flags & 8;
	}
	Snd_Stop(0x92);
	Snd_Play(0x92);
	(*self->cls->method[20])(self, 0x96);		/* SetFrame */
	Obj_Detach(self);
	self->ay = 0x1E;
	self->vy = -0x20C;
	self->vx = self->spr.flags & 0x10 ? -0x600 : 0x600;
	self->ax = 0;
	self->maxVx = 0x600;
	self->countdown = 0x26;
	SET_STATE(self, (StateFn)Beast_StateRoll);
	return self->flags & 8;
}
