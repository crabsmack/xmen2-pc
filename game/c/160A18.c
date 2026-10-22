/* Cam_FollowPlayer - CAMERA slot +$1E: one-player mode: keep the live player in a dead-zone box.
 *
 * self: the camera; left, top, right, bottom: the box (screen pixels).
 * track = Cam_TrackPlayer, ease = Cam_EaseOffset, look limits $40 / $E0; the target is player 1
 * if alive, else player 2 if alive, else none; targetX/Y = its pixel position. */
#include "xmen2.h"

void Cam_FollowPlayer(Camera *self, short left, short top, short right, short bottom)
{
	register Camera *c = self;

	c->track = Cam_TrackPlayer;
	c->ease = Cam_EaseOffset;
	c->deadLeft = left;
	c->deadTop = top;
	c->deadRight = right;
	c->deadBottom = bottom;
	c->lookMinScrY = 0x40;
	c->lookMaxScrY = 0xE0;
#if defined(XMEN2_NATIVE)
	if (c->target = IS_ALIVE(&g_Player1) ? (Actor *)&g_Player1
			: IS_ALIVE(&g_Player2) ? (Actor *)&g_Player2
			: g_opt.players > 2 ? (Actor *)P34_FirstAlive() : 0) {	/* phase 7: or a player 3-4 */
#else
	if (c->target = IS_ALIVE(&g_Player1) ? (Actor *)&g_Player1
			: IS_ALIVE(&g_Player2) ? (Actor *)&g_Player2 : 0) {
#endif
		c->targetX = c->target->x >> 8;
		c->targetY = c->target->y >> 8;
	}
}
