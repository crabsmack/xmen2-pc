/* XPlayer_Draw - XPLAYER class slot +$1A: draw the player and its hit spark, when its body box (40 px
 * around it) touches the screen.
 *
 * self: the player.
 * The hit spark (g_HitSparkSpr, Spr_Draw) shows at sparkX/sparkY while sparkTimer counts down
 * (2 a frame in two-player mode). Blinking (flags6 $40) draws the player on odd g_LevelFrame
 * only. Its frame's tiles are uploaded when stale (Spr_UploadFrame18); nothing when no frame
 * is uploaded; then the sprite pieces (Spr_EmitStream) at its screen position + $80, animDist
 * behind for a distance-driven animation (as ACTOR Draw).
 * (The frame's record is looked up and never used.) */
#include "xmen2.h"

void XPlayer_Draw(register XPlayer *self)
{
	SprInst *spr;
	short frame;

	spr = &self->spr;
	if (self->boxLeft - 40 - g_CameraX <= VIEW_R(320) && self->boxRight + 40 - g_CameraX >= VIEW_L(0)
	    && self->boxTop - 40 - g_CameraY <= 224 && self->boxBottom + 40 - g_CameraY >= 0) {
		if (self->sparkTimer) {
			self->sparkTimer -= g_TwoPlayerStep + 1;
			Spr_Draw(&g_HitSparkSpr, self->sparkX - g_CameraX, self->sparkY - g_CameraY);
		}
		if (self->flags6 & 0x40 && (g_LevelFrame & 1) == 0)
			return;
		if ((spr->flags & 0x20) == 0)
			Spr_UploadFrame18(spr);
		frame = spr->uploaded;
		if (frame != 0xFF) {
			SprFrame *rec = SPR_FRAME(spr->def, frame);
			short x = self->anim && (*(u8 *)self->anim & 0x30) == 0x20
				? ((self->x - self->animDist) >> 8) - g_CameraX : (self->x >> 8) - g_CameraX;
			short y = self->anim && (*(u8 *)self->anim & 0x30) == 0x30
				? ((self->y - self->animDist) >> 8) - g_CameraY : (self->y >> 8) - g_CameraY;

#if defined(XMEN2_NATIVE)
			if (WS)	/* phase 6, 16:9: pieces reach past the box test; clip them to the wide view */
				Spr_EmitStreamClip(spr, x + 0x80, y + 0x80, self->spr.tileBase);
			else
#endif
			Spr_EmitStream(spr, x + 0x80, y + 0x80, self->spr.tileBase);
		}
	}
}
