/* BigBio_SpitUpdateAll - BIGBIO's spit projectiles, once per frame (the Timer BigBio_ClassInit
 * starts): move each, drop it once it leaves the screen sideways or hits a player, bounce it off
 * the top and bottom, draw it.
 *
 * A spit flies at $900 a frame in x (twice in two-player mode), $120 after its first bounce; it
 * goes when it is 20 px past the camera's left edge (flying left) or 340 px right of it (flying
 * right), or when it is inside the second box of a live player not blinking: that player takes
 * damage 1 (TakeDamage type 4). It bounces at y -5 and 217 (mirrored), each bounce turning its
 * tile frame (0-2) on. Drawn as one hardware sprite of the Art_BigBioSpr tiles, palette 1. */
#include "xmen2.h"

void BigBio_SpitUpdateAll(void)
{
	short left, top, right;
	char bounced, dead;
	SprInst *def;
	short i;
	short attr;
	long speed;
	register BigBioSpit *spit;
	register XPlayer *p;
	short bottom, px, py;
	register long n;

	if (!g_BigBioSpitCount)
		return;
	def = &g_SpriteDefs[g_BigBioSprDef];
	spit = g_BigBioSpitPool;
	for (i = 0; i < g_BigBioSpitCount; i++, spit++) {
		bounced = 0;
		dead = 0;
		speed = (short)(spit->bounces ? 0x120 : 0x900);
		if (spit->vx < 0)
			speed = -speed;
		spit->x += speed;
		spit->y += spit->vy;
		if (g_TwoPlayerStep) {
			spit->x += speed;
			spit->y += spit->vy;
		}
		py = spit->y >> 8;
		px = spit->x >> 8;
		if (spit->vx < 0) {
			if (px < g_CameraX - 20)
				dead = 1;
		} else {
			if (px > g_CameraX + 340)
				dead = 1;
		}
		for (n = 0; n < 2; ++n) {
			if (IS_ALIVE(n == 0 ? (p = &g_Player1) : (p = &g_Player2))) {
				left = p->box2Left;
				top = p->box2Top;
				right = p->box2Right;
				bottom = p->box2Bottom;
				if (!p->hitCooldown && left <= px && right >= px && top <= py && py <= bottom) {
					(*p->cls->method[8])(p, 0, 4, 1, px, py);	/* TakeDamage */
					dead = 1;
				}
			}
		}
		if (dead) {
			BigBio_SpitFree(spit);
			spit--;
			i--;
			continue;
		}
		if (spit->vy < 0) {
			if (py < -5) {
				py = -10 - py;
				spit->y = py << 8;
				bounced = 1;
			}
		} else {
			if (py > 217) {
				py = 434 - py;
				spit->y = py << 8;
				bounced = 1;
			}
		}
		if (bounced) {
			spit->vy = -spit->vy;
			spit->bounces++;
			if (spit->frame != 2)
				spit->frame++;
		}
		attr = (def->tileBase + (spit->frame << 2)) | 0x2000;
		g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
		g_SprWritePtr->y = py - g_CameraY + 0x80;
		g_SprWritePtr->size = 5;
		g_SprWritePtr->attr = attr | 0x8000;
		g_SprWritePtr->x = px - g_CameraX + 0x80;
	}
}
