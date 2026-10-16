/* Teleporter_StateTeleport - TELEPORTER state: send the players to the receiver: each live player is
 * detached (Obj_Detach), put at the receiver's placement + its offset (params 1, 2; player 2 $10 px
 * further along x, away from the offset's side), stopped and given 60 frames of hit cooldown; the
 * camera jumps there (CloneA2_SetCameraLimitsToMap, then the camera's PanTo at speed 1000, x locked
 * when the receiver's param 4 is set); the CloneA2 palette effects stop, Start no longer pauses
 * (Teleporter_NoPause), the screen fades to black (Pal_FadeOutWait, waits); then 30 frames of
 * Teleporter_StateHold.
 *
 * self: the pad. Returns flags & 8. */
#include "xmen2.h"

#define g_CloneA2PalOff	(*(s16 *)g_SceneVar3B04)	/* scene variable: non-zero: no flicker, no background palette swaps */

short Teleporter_StateTeleport(register Teleporter *self)
{
	short rx;
	short ry;
	short dx;
	short dy;
	unsigned char lockX;
	unsigned char lockY;
	long nx;
	long ny;
	long i;
	register Placement *rcv = self->receiver;
	register XPlayer *p;

	rx = rcv->x;
	ry = rcv->y;
	dx = rcv->param[1];
	dy = rcv->param[2];
	lockX = rcv->param[4] ? 1 : 0;
	lockY = 0;
	nx = (rx + dx) << 8;
	ny = (ry + dy) << 8;
	for (i = 0; i < 2; ++i) {
		if (IS_ALIVE(i == 0 ? (p = &g_Player1) : (p = &g_Player2))) {
			Obj_Detach(p);
			p->x = nx;
			if (p == &g_Player2) {
				long off = 0x1000;

				if (dx < 0)
					off = -off;
				p->x += off;
			}
			p->y = ny;
			STOP_MOVING(p);
			p->hitCooldown = 0x3C;
		}
	}
	CloneA2_SetCameraLimitsToMap();
	(*g_Camera->cls->method[12])(g_Camera, rx, ry, 1000, 1000, lockX, lockY);	/* +$36 Cam_PanTo */
	g_CloneA2PalOff = 1;
	g_PauseHandler = FN_DATA(Teleporter_NoPause);
	Pal_FadeOutWait(1);
	self->countdown = 0x1E;
	SET_STATE(self, Teleporter_StateHold);
	return self->flags & 8;
}
