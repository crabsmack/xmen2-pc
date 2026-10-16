/* EvilMagneto_StateBlast - EVILMAGNETO state after EvilMagneto_ActBlast's wind-up: fire a magnetic
 * globe in direction shotDir (Magneto_SpawnShot as an enemy: not crouching, not in the air, the
 * clone's sprite priority; sound $83 when none could be made), then play the shot's end animation
 * (Magneto_ShotEndAnim) and go on with the AI script at its end, or at once without one.
 *
 * self: the clone (never runs in the corpus).
 * A bug: the call passes 7 arguments to Magneto_SpawnShot's 8; its last, tiles, is whatever the
 * stack holds above them on the 68000 (the low word of the caller's saved a3). The PC passes 0. */
#include "xmen2.h"

void EvilMagneto_StateBlast(EvilMagneto *self)
{
	MagneticGlob *shot;
	void *anim;

#ifdef XMEN2_MPW
	shot = ((MagneticGlob *(*)())Magneto_SpawnShot)(self->x, self->y, self->shotDir, 0, 0,
		self->spr.flags & 4 ? 1 : 0, 0);
#else
	shot = Magneto_SpawnShot(self->x, self->y, (char)self->shotDir, 0, 0,
		self->spr.flags & 4 ? 1 : 0, 0, 0);
#endif
	if (!shot) {
		Snd_Stop(0x83);
		Snd_Play(0x83);
	}
	if (anim = Magneto_ShotEndAnim(self->shotDir)) {
		Obj_SetAnim(self, anim);
		AI_NEXT_STEP(self, self->ai, 0xFF);
	} else {
		AI_NEXT_STEP(self, self->ai, 0);
	}
}
