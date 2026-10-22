/* Teleporter_StateArrive - TELEPORTER state: the players are there: the CloneA2 camera's alternate
 * mode when the receiver's param 4 is set, the camera zone of the pad's channel
 * (CloneA2_SetCameraZone); each live player gets 10 frames of hit cooldown, its class slot +$86
 * (XPLAYER DropThrough) and its StateControl (class slot +$A6) as the state; the screen fades
 * back in (Pal_FadeInWait, waits); then Teleporter_StateWaitFade.
 *
 * self: the pad. Returns flags & 8. */
#include "xmen2.h"

short Teleporter_StateArrive(register Teleporter *self)
{
	register Placement *rcv = self->receiver;
	short zone;
	long i;
	register XPlayer *p;

	if (rcv->param[4])
		CloneA2_StartCamAltMode();
	zone = self->channel;
	CloneA2_SetCameraZone(zone);
	for (i = 0; i < GROUP_N; ++i) {
		if (IS_ALIVE(GROUP_P(i, p))) {
			p->hitCooldown = 10;
			(*p->cls->method[32])(p);			/* +$86 DropThrough */
			p->timer = 0;
			p->state = (StateFn)p->cls->method[40];		/* StateControl */
		}
	}
	Pal_FadeInWait(1);
	SET_STATE(self, Teleporter_StateWaitFade);
	return self->flags & 8;
}
