/* SavageBio_StateFireTip - SAVAGEBIGBIO state: fire a BRAINROCKET from the tip of the tentacle,
 * the way it faces, then go on with the script in 10 frames.
 *
 * self: the boss (after SavageBio_ActFireRocket with the tentacle out; never runs in the
 * corpus).
 * The tip is the tentacle's last path point moved by the wave step SavageBigBio_Draw shows
 * there this frame (Tbl_SavageBigBioTentacleWave, one step per 2 frames), mirrored when
 * h-flipped as Draw mirrors it (a $20 px sprite). The rocket starts $13 px further out and
 * $40 px above it ($52 below when hanging from the ceiling) and flies at angle 0 (right) or
 * 180. Returns nothing in particular. */
#include "xmen2.h"

short SavageBio_StateFireTip(SavageBigBio *self)
{
	TentacleWave *wave;
	PathPt *tip;
	short w, x, y;
	Actor *rocket;

	wave = (TentacleWave *)CODEPTR(Tbl_SavageBigBioTentacleWave);
	y = (g_FrameCounter >> 1 & 0x1F) + (self->tentacleLen - 1);	/* y holds the wave step first */
	if (y >= 0x20)
		y -= 0x20;
	wave += y;
	tip = self->tentacle + (self->tentacleLen - 1);
	if (!(self->spr.flags & 0x10)) {
		x = tip->x + wave->dx + 0x13;
		y = tip->y + -7 + wave->dy + 0x10;
	} else {
		w = 0x20;
		x = -(tip->x + w + wave->dx + 0x13);
		y = tip->y + -7 + wave->dy + 0x10;
	}
	x += !(self->spr.flags & 0x10) ? 0 : 0;
	y += !(self->spr.flags & 8) ? -0x49 : 0x49;
	rocket = (Actor *)(*CLASS(Tbl_BrainRocket_vtable)->method[2])(0,		/* New */
		CLASS(Tbl_BrainRocket_vtable), self->x + (x << 8), self->y + (y << 8), 0);
	if (rocket)
		Rocket_SetAngleA(rocket, self, !(self->spr.flags & 0x10) ? 0 : 0xB4);
	AI_NEXT_STEP(self, self->ai, 10);
	NO_RETURN_VALUE
}
