/* Cam_TrackPlayer - the tracking function in one-player mode (Cam_FollowPlayer).
 *
 * self: the camera, following target (a player). With two players alive it switches to
 * FollowBoth (slot +$1A, box ($96, $40, $AA, $C0)), sets g_CamModeSwitched and runs the
 * post-update hook. Else, once, or twice in two-player-step frames (g_TwoPlayerStep), with the
 * target's pixel position (tx, ty) against last frame's (targetX/Y):
 *   x (unless camFlags 1): on a wall (Player_ClimbState 2 / 3, no y offset) eased to wallMargin
 *     from that side; near an open ledge (Seg_WouldLeaveEnd, ledgeDist) eased twice as fast to
 *     the dead zone's side behind it; else the spring (Cam_SpringAxis) toward deadLeft/deadRight,
 *     except on a ceiling (climb state 1) whose end is in view;
 *   y (unless camFlags 2): climbing (Player_ClimbStateNcWolv 1) eased to climbScrY (+$20 for
 *     Wolverine); other climb states: the spring toward rows $40..$C0; Beast/Psylocke climbing or
 *     riding a lift: eased to row $70; on the ground eased to deadBottom; in the air kept between
 *     deadTop and deadBottom.
 * Then the position is clamped to the camera limits, the spring speeds marked stopX / stopY are
 * zeroed and targetX/Y = tx/ty. The flags are crossed in places (a ledge or the x spring marks y,
 * the y spring marks x): the original's. carrier is stored, never read. */
#include "xmen2.h"

void Cam_TrackPlayer(Camera *self)
{
	register Camera *c = self;
	register Actor *t = c->target;
	short tx;
	long carrier = 0;
	short steps = g_TwoPlayerStep + 1;
	register char stopY = 0;
	char stopX = 0;
	register short ty;
	register unsigned char climb;
	register short bottom;

	if (!t)
		return;
	if (g_PlayersAlive == 2) {
		CamFn hook;

		(*c->cls->method[5])(c, 0x96, 0x40, 0xAA, 0xC0);	/* FollowBoth */
		g_CamModeSwitched = 1;
		if (hook = c->postHook)
			hook(c);
		return;
	}
	tx = t->x >> 8;
	ty = t->y >> 8;
	while (steps--) {
		short ofs2;
		short ofs;
		short top;
		short oldY;
		unsigned char climbNW;

		if (!(c->camFlags & 1)) {
			climb = (char)Player_ClimbState(t);
			if ((climb == 2 || climb == 3) && c->offsetY == 0) {
				switch (climb) {
				case 2:
					ofs = c->wallMargin;
					break;
				case 3:
					ofs = 0x140 - c->wallMargin;
					break;
				}
				c->x = Cam_EaseAxis(ofs, c->x, c->easeStep, tx, c->targetX);
				stopX = 1;
			} else if (Seg_WouldLeaveEnd(t, c->ledgeDist)) {
				ofs = !(t->spr.flags & 0x10) ? c->deadLeft : c->deadRight;
				c->x = Cam_EaseAxis(ofs, c->x, c->easeStep << 1, tx, c->targetX);
				stopY = 1;
			} else if (climb != 1 || !Player_CeilingEndInView(t)) {
				Cam_SpringAxis(tx, c->targetX, c->deadLeft, c->deadRight, c->catchUp,
					c->maxAccelX, &c->x, &c->speedX, &c->accelX, &c->accelStepX);
				stopY = 1;
			}
		}
		if (!(c->camFlags & 2)) {
			top = c->deadTop;
			bottom = c->deadBottom;
			oldY = c->y;
			if (climbNW = (char)Player_ClimbStateNcWolv(t)) {
				if (climbNW == 1) {
					ofs2 = c->climbScrY;
					if (t->cls == CLASS(Tbl_Wolverine_vtable))
						ofs2 += 0x20;
					c->y = Cam_EaseAxis(ofs2, c->y, c->easeStep, ty, c->targetY);
					stopY = 1;
				} else {
					Cam_SpringAxis(ty, c->targetY, 0x40, 0xC0, c->catchUp, c->maxAccelY,
						&c->y, &c->speedY, &c->accelY, &c->accelStepY);
					stopX = 1;
				}
			} else if ((char)Player_ClimbStateBeastPsy(t) || (carrier = ADDR68(Obj_GetLiftCarrier(t)))) {
				c->y = Cam_EaseAxis(0x70, c->y, c->easeStep, ty, c->targetY);
			} else if (!(t->flags & 0x1000)) {
				c->y = Cam_EaseAxis(bottom, c->y, c->easeStep, ty, c->targetY);
				stopY = 1;
			} else if (ty < oldY + top)
				c->y = ty - top;
			else if (ty > oldY + bottom)
				c->y = ty - bottom;
		}
	}
	{
		short cx = c->x;
		short cy = c->y;

		if (cx > CAM_HI(g_CamMinX, g_CamMaxX))
			c->x = CAM_HI(g_CamMinX, g_CamMaxX);
		if (cx < CAM_LO(g_CamMinX, g_CamMaxX))
			c->x = CAM_LO(g_CamMinX, g_CamMaxX);
		if (cy > g_CamMaxY)
			c->y = g_CamMaxY;
		if (cy < g_CamMinY)
			c->y = g_CamMinY;
	}
	if (stopX) {
		c->speedX = 0;
		c->accelX = 0;
	}
	if (stopY) {
		c->speedY = 0;
		c->accelY = 0;
	}
	c->targetX = tx;
	c->targetY = ty;
}
