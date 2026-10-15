/* Nightcrawler_SetTeleportVelocity - NIGHTCRAWLER the teleport's speed from teleportDir: $A00 in a
 * straight line, $700 on a diagonal; the camera is carried along in x.
 *
 * self: the player.
 * Maximum speed x the speed. Left: vx -speed, Right: vx speed; unless the camera is panning
 * (Cam_IsPanning) its following speed becomes vx and its accel its maximum, the same way.
 * Up: vy -speed, Down: vy speed. */
#include "xmen2.h"

void Nightcrawler_SetTeleportVelocity(register Nightcrawler *self)
{
	register char diagonal;
	register long speedX, speedY;

	diagonal = self->teleportDir & 0xC && self->teleportDir & 3;
	speedX = diagonal ? (short)0x700 : (short)0xA00;
	speedY = diagonal ? (short)0x700 : (short)0xA00;
	self->maxVx = speedX;
	if (self->teleportDir & 4) {
		self->vx = -speedX;
		if (!Cam_IsPanning(g_Camera)) {
			g_Camera->speedX = self->vx;
			g_Camera->accelX = -g_Camera->maxAccelX;
		}
	} else if (self->teleportDir & 8) {
		self->vx = speedX;
		if (!Cam_IsPanning(g_Camera)) {
			g_Camera->speedX = self->vx;
			g_Camera->accelX = g_Camera->maxAccelX;
		}
	}
	if (self->teleportDir & 1)
		self->vy = -speedY;
	else if (self->teleportDir & 2)
		self->vy = speedY;
}
