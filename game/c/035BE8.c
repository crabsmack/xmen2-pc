/* MagnetoBoss_RubbleTimer - timer callback of the rubble MagnetoBoss_SpawnRubble throws: move,
 * animate and draw the 32 pieces of the pool; the timer ends when no piece is on screen.
 *
 * p: the rubble pool (32 MagnetoRubble).  t: this timer (Timer_Create(.., 0, pool, 1)).
 * Each active piece, once per step (2 in two-player steps): position += speed, speed +=
 * acceleration (ax added unsigned), the animation frame (0-2) steps every frameDelay frames.
 * Below its floorY it bounces once (y back to the floor minus vy, vy = -vy / 2, floor $7D00).
 * On screen (screen x -$13..$153 and y < $F0; there is no top limit) it is drawn as one
 * hardware sprite on odd g_FrameCounter frames in one-player mode (Tbl_MagnetoBossRubbleSprites
 * entry frameBase + frame: tile offset from the rubble sprite set's tiles, high priority, and
 * size); off screen it is deactivated. */
#include "xmen2.h"

void MagnetoBoss_RubbleTimer(register MagnetoRubble *p, Timer *t)
{
	short attr, size;
	short tileBase = g_SpriteDefs[g_MagnetoBossSprDef].tileBase | 0x8000;
	short steps = g_TwoPlayerStep ? 2 : 1;
	char onScreen;
	register MagnetoRubbleSprite *spr;
	register long n;
	short i;
	register long sx, sy;

	onScreen = 0;
	for (n = 32; n; n--, p++) {
		if (p->active & 1) {
			i = steps;
			do {
				p->x += p->vx;
				p->y += p->vy;
				p->vx += p->ax;
				p->vy += p->ay;
				if (!--p->frameTimer) {
					p->frameTimer = p->frameDelay;
					p->frame++;
					if (p->frame > 2)
						p->frame = 0;
				}
			} while (--i);
			sy = p->y >> 8;
			if (sy > p->floorY) {
				p->y = (p->floorY << 8) - p->vy;
				p->vy = -(p->vy >> 1);
				p->floorY = 0x7D00;
			}
			sx = (p->x >> 8) - g_CameraX;
			sy = (p->y >> 8) - g_CameraY;
			if (sx < 0x154 && sx > -0x14 && sy < 0xF0) {
				onScreen = 1;
				if (!g_TwoPlayerStep && (g_FrameCounter & 1)) {
					spr = (MagnetoRubbleSprite *)CODEPTR(Tbl_MagnetoBossRubbleSprites);
					spr += p->frameBase + p->frame;
					size = spr->size;
					attr = spr->tile + tileBase;
					g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
					g_SprWritePtr->y = sy + 0x80;
					g_SprWritePtr->size = size;
					g_SprWritePtr->attr = attr;
					g_SprWritePtr->x = sx + 0x80;
				}
			} else
				p->active = 0;
		}
	}
	if (!onScreen)
		Timer_Free(t);
}
