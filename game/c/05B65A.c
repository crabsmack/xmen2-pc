/* Boulder_StateBounce - a BOULDER rolling down: each frame it looks for a floor segment of its
 * layer under its position (Seg_FindAt, 15 px).
 *   Solid floor (type 3): from now on it lands on the layer below (0 stays 0), bounces (vy =
 *     -vy / 4) and rolls off left or right at random ($150).
 *   Crumbling floor (types $13, $14): it rolls towards the floor's nearer end ($300); falling
 *     fast (vy > $200) it hits the map tile under it (Tile_TestAt, hook types $3800, hooks run:
 *     the crumbling floor's crack hook); it bounces (vy = -vy / 4, at least $100 up).
 * Each impact: the rolling animation (Tbl_BoulderAnim4), a dust cloud (spr2, Tbl_BoulderBounceAnim)
 * where it is, sound $97 and a camera shake, Tbl_BoulderQuake when its box (40 px around) is on
 * screen, Tbl_BoulderQuake2 (lighter) when not. With no layer left it stays in this state
 * waiting ($FF), else it runs again next frame.
 *
 * self: the boulder. Returns self->flags & 8. */
#include "xmen2.h"

/* Is o's body box, grown by m on each side, off the screen? */
#define OFF_SCREEN(o, m) \
	((o)->boxLeft - (m) - g_CameraX > 320 || (o)->boxRight + (m) - g_CameraX < 0 \
	 || (o)->boxTop - (m) - g_CameraY > 224 || (o)->boxBottom + (m) - g_CameraY < 0)

short Boulder_StateBounce(register Boulder *self)
{
	register Seg *seg;
	register short x;
	short left, right, toLeft;

	if (seg = Seg_FindAt(self->x >> 8, self->y >> 8, self->layer, 15)) {
		switch (seg->type) {
		case 3:
			if (seg->layer > 0)
				self->layer = seg->layer - 1;
			else if (seg->layer == 0)
				self->layer = 0;
			self->vy = -(self->vy >> 2);
			if ((unsigned short)Rand_Next() & 1)
				self->vx = 0x150;
			else
				self->vx = -0x150;
			Obj_SetAnim(self, CODEPTR(Tbl_BoulderAnim4));
			self->anim2 = (u8 *)CODEPTR(Tbl_BoulderBounceAnim);
			self->maxVx = self->x >> 8;
			self->maxVy = self->y >> 8;
			self->spr2.frame = self->anim2[7];
			Snd_Stop(0x97);
			Snd_Play(0x97);
			g_CamShake = !OFF_SCREEN(self, 40) ? CODEPTR(Tbl_BoulderQuake) : CODEPTR(Tbl_BoulderQuake2);
			break;
		case 0x13:
		case 0x14:
			x = self->x >> 8;
			left = MIN(seg->x1, seg->x2);
			right = MAX(seg->x1, seg->x2);
			toLeft = x;
			toLeft -= left;
			left = right;		/* (now the distance to the right end) */
			left -= x;
			if (toLeft < left)
				self->vx = -0x300;
			else
				self->vx = 0x300;
			if (self->vy > 0x200) {
				short tx = self->x >> 8;
				short ty = (self->y >> 8) + 4;

				Tile_TestAt((long)self, tx, ty, !g_SolidMapIsBg, 0x3800, 1);
			}
			self->vy = -(self->vy >> 2);
			if (ABS(self->vy) < 0x100)
				self->vy = -0x100;
			Obj_SetAnim(self, CODEPTR(Tbl_BoulderAnim4));
			self->anim2 = (u8 *)CODEPTR(Tbl_BoulderBounceAnim);
			self->maxVx = self->x >> 8;
			self->maxVy = self->y >> 8;
			self->spr2.frame = self->anim2[7];
			Snd_Stop(0x97);
			Snd_Play(0x97);
			g_CamShake = !OFF_SCREEN(self, 40) ? CODEPTR(Tbl_BoulderQuake) : CODEPTR(Tbl_BoulderQuake2);
			break;
		}
	}
	if (self->layer == 0) {
		SET_STATE_WAIT(self, Boulder_StateBounce, 0xFF);
		return self->flags & 8;
	} else {
		SET_STATE(self, Boulder_StateBounce);
		return self->flags & 8;
	}
}
