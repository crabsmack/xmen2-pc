/* SpaceA1_CamUpdate - the Space Elevator camera's update (CAMERA +$04, SpaceA1_InitCamera): the
 * camera's tracking, easing and shake (those it has); while the lift moves, plane B scrolls by its
 * speed (the shaft wall), and at the top of the shaft (going up, past $20000) or the bottom (going
 * down, below 0) it stops there; at the top the scene ends in 30 frames (Scene_EndAfter), at the
 * bottom with sound $35 and every live player's health 0. Then the apply (SpaceA1_CamApply); the
 * camera is active while a player lives or plane B follows an object (g_BgLockToObj).
 *
 * cam: the camera. */
#include "xmen2.h"

void SpaceA1_CamUpdate(register Camera *cam)
{
	register CamFn f;

	(*cam->track)(cam);
	if ((f = cam->ease) != 0)
		(*f)(cam);
	if ((f = cam->shake) != 0)
		(*f)(cam);
	if (!g_Elevator->stopped) {
		g_BgScrollY -= g_Elevator->speed >> 8;
		if (g_Elevator->up) {
			if (g_Elevator->pos > 0x20000) {
				g_Elevator->stopped = 1;
				g_Elevator->speed = 0;
				g_Elevator->accel = 0;
				Scene_EndAfter(30);
			}
		} else if (g_Elevator->pos < 0) {
			g_Elevator->stopped = 1;
			g_Elevator->speed = 0;
			g_Elevator->accel = 0;
			Snd_Stop(0x35);
			Snd_Play(0x35);
			if (IS_ALIVE(&g_Player1))
				g_Player1.health = 0;
			if (IS_ALIVE(&g_Player2))
				g_Player2.health = 0;
#if defined(XMEN2_NATIVE)
			P34_KillAll();	/* phase 7: players 3-4 fall with the lift too */
#endif
			Scene_EndAfter(30);
		}
	}
	(*cam->apply)(cam);
	g_CamActive = IS_ALIVE(&g_Player1) || IS_ALIVE(&g_Player2) || g_BgLockToObj;
#if defined(XMEN2_NATIVE)
	if (g_opt.players > 2 && P34_AliveCount())
		g_CamActive = 1;
#endif
}
