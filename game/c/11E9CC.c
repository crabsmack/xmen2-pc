/* BioBalls_Draw - BIOBALLS Draw (class slot +$1A): every ball of the ball list on screen as one
 * hardware sprite of the object's frame (16x16, size code 5).
 *
 * self: the object.
 * Nothing without a ball list or a frame ($FF). A frame past the definition's frame count is
 * brought back by $70. The frame's tiles are uploaded when they changed. The list ends at its
 * count or at a ball at (-1, -1). With ballFlags bit 0 (layered: BioBalls_MotionHelix) the balls
 * tagged in front (tag bit 0) are drawn first, then the others; else all in list order. A ball
 * is drawn when it is within (-32, 320) x (-32, 224) of the camera, with priority, the
 * sprite's palette and its first VRAM tile. */
#include "xmen2.h"

/* the ball on screen? */
#define BALL_ON_SCREEN(b)	((b)->x - g_CameraX > VIEW_L(-32) && (b)->x - g_CameraX < VIEW_R(320) \
				&& (b)->y - g_CameraY > -32 && (b)->y - g_CameraY < 224)
/* the next hardware sprite: the ball */
#define BALL_EMIT(b, attr) \
	do { \
		g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link); \
		g_SprWritePtr->y = (b)->y - g_CameraY + 0x80; \
		g_SprWritePtr->size = 5; \
		g_SprWritePtr->attr = (attr); \
		g_SprWritePtr->x = (b)->x - g_CameraX + 0x80; \
	} while (0)

void BioBalls_Draw(BioBalls *self)
{
	SprDef *def = (SprDef *)CODEPTR(Art_BioBallsSpr);
	BioBall *ball;
	short count;
	short attr;
	register unsigned short i;

	if (!self->ballList || self->spr.frame == 0xFF)
		return;
	if (self->spr.frame > def->frameCount)
		self->spr.frame -= 0x70;
	count = *self->ballList;
	ball = (BioBall *)(self->ballList + 1);
	if (self->spr.frame != self->spr.uploaded) {
		Spr_UploadFrame(&self->spr, 0);
		self->spr.uploaded = self->spr.frame;
	}
	attr = self->spr.tileBase | 0x8000;
	attr |= (self->spr.flags & 3) << 13;
	if (self->ballFlags & 1) {
		for (i = 0; i < count; i++, ball++) {
			if (ball->x < 0 && ball->y < 0)
				break;
			if (ball->tag & 1) {
				if (BALL_ON_SCREEN(ball))
					BALL_EMIT(ball, attr);
			}
		}
		ball = (BioBall *)(self->ballList + 1);
		for (i = 0; i < count; i++, ball++) {
			if (ball->x < 0 && ball->y < 0)
				break;
			if (!(ball->tag & 1) && BALL_ON_SCREEN(ball))
				BALL_EMIT(ball, attr);
		}
	} else {
		for (i = 0; i < count; i++, ball++) {
			if (ball->x < 0 && ball->y < 0)
				break;
			if (BALL_ON_SCREEN(ball))
				BALL_EMIT(ball, attr);
		}
	}
}
