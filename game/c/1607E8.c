/* Cam_Init - CAMERA slot +$0E: set up a new camera.
 *
 * self: the camera. The default steps (Cam_DefaultUpdate, Cam_EaseOffset, Cam_ShakeStep,
 * Cam_ApplyPos; no look step, no pan), the tuning (catchUp 3, easeStep 2, spring accel steps $10
 * and maxima $150, look speeds 5 and limits $40 / $E0, climbing screen y $56, wall margin $60,
 * ledge look-ahead $A), then the follow mode by the players alive: FollowBoth (slot +$1A) with
 * ($96, $40, $AA, $C0) for two, FollowPlayer (+$1E) with ($60, $5A, $E0, $C0) otherwise. The
 * camera starts on its target ($A0, $C0 above-left of it), or where the scroll is; it is applied
 * at once and the post-update hook runs. */
#include "xmen2.h"

void Cam_Init(Camera *self)
{
	register Camera *c = self;
	register Actor *t;
	register CamFn hook;
	short alive;

	c->update = Cam_DefaultUpdate;
	c->ease = Cam_EaseOffset;
	c->shake = Cam_ShakeStep;
	c->lookFn = 0;
	c->apply = Cam_ApplyPos;
	c->savedTrack = 0;
	c->offsetX = 0;
	c->offsetY = 0;
	c->catchUp = 3;
	c->easeStep = 2;
	c->accelX = 0;
	c->accelY = 0;
	c->accelStepX = 0x10;
	c->accelStepY = 0x10;
	c->speedX = 0;
	c->speedY = 0;
	c->maxAccelX = 0x150;
	c->maxAccelY = 0x150;
	c->look = 0;
	c->lookReturn = 5;
	c->lookStep = 5;
	c->lookMinScrY = 0x40;
	c->lookMaxScrY = 0xE0;
	c->unread_48 = 0;
	c->camFlags = 0;
	c->climbScrY = 0x56;
	c->wallMargin = 0x60;
	c->ledgeDist = 0xA;
	c->panX = 0;
	c->panY = 0;
	t = 0;
	alive = 0;
	if (IS_ALIVE(&g_Player1))
		alive++;
	if (IS_ALIVE(&g_Player2))
		alive++;
	switch (alive) {
	case 0:
	case 1:
		(*c->cls->method[6])(c, 0x60, 0x5A, 0xE0, 0xC0);	/* FollowPlayer */
		break;
	case 2:
		(*c->cls->method[5])(c, 0x96, 0x40, 0xAA, 0xC0);	/* FollowBoth */
		break;
	}
	if (t = c->target) {
		c->targetX = t->x >> 8;
		c->targetY = t->y >> 8;
		c->x = c->targetX - 0xA0;
		c->y = (t->y >> 8) - 0xC0;
	} else {
		c->targetX = g_CameraX + 0xA0;
		c->targetY = g_CameraY + 0x70;
		c->x = g_CameraX;
		c->y = g_CameraY;
	}
	(*c->apply)(c);
	c->prevY = c->y;
	c->prevX = c->x;
	if (hook = c->postHook)
		hook(c);
}
