/* Deathbird_UpdateBurst - DEATHBIRD's burst timer (Timer_Create in Deathbird_Init; started by
 * Deathbird_StateThrowBurst / _StateDiveLow, stopped by _StateFlyOff / _StateHurt): move the 8
 * burst projectiles, draw each one on screen as a hardware sprite turned the way it flies, and
 * hit the players it touches.
 *
 * self: the boss (the timer's argument).
 * A projectile is an 8x8 sprite (size 0) of the burst sprite set (g_SceneVar3B04), priority and
 * palette 1 ($A000), frame 0-7 by its direction (0 right-up, 1 up-right, 2 up-left, 3 left-up,
 * 4 left-down, 5 down-left, 6 down-right, 7 right-down), drawn when within 32 px of the screen.
 * A live player not flashing from a hit (hitCooldown 0) whose second box holds the projectile's
 * position is hit through DEATHBIRD's Attack (class slot +$22) with its hitType and attack.
 * Nothing without a burst record. */
#include "xmen2.h"

#define g_DeathbirdShotSprDef	(*(s16 *)g_SceneVar3B04)	/* scene variable: sprite definition of the burst projectiles */

void Deathbird_UpdateBurst(register Deathbird *self)
{
	short i;
	short sx;
	short sy;
	short vx;
	short frame;
	short attr;
	BurstShot *p;
	DeathbirdBurst *b;
	register short vy;
	register short x;
	register short y;
	register short j;
	register XPlayer *pl;

	attr = g_SpriteDefs[g_DeathbirdShotSprDef].tileBase | 0xA000;
	if (b = self->burst) {
		for (i = 0; i < 8; i++) {
			p = &b->shot[i];
			vx = p->vx += p->ax;
			vy = p->vy += p->ay;
			p->x += vx;
			p->y += vy;
			if (vx > 0) {
				if (vy < 0)
					frame = vx > -vy ? 0 : 1;
				else
					frame = vy < vx ? 7 : 6;
			} else {
				if (vy < 0)
					frame = vy > vx ? 3 : 2;
				else
					frame = vx < -vy ? 4 : 5;
			}
			x = p->x >> 8;
			y = p->y >> 8;
			sx = x - g_CameraX;
			sy = y - g_CameraY;
			if (sx > VIEW_L(-32) && sx < VIEW_R(320) && sy > -32 && sy < 224) {
				g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
				g_SprWritePtr->y = sy + 0x80;
				g_SprWritePtr->size = 0;
				g_SprWritePtr->attr = attr + frame;
				g_SprWritePtr->x = sx + 0x80;
			}
			for (j = 0; j < 2; ++j) {
				if (IS_ALIVE(j == 0 ? (pl = &g_Player1) : (pl = &g_Player2)) && !pl->hitCooldown
				    && x >= pl->box2Left && x <= pl->box2Right
				    && y >= pl->box2Top && y <= pl->box2Bottom)
					(*self->cls->method[7])(self, pl, self->hitType, self->attack, x, y);	/* Attack */
			}
		}
	}
}
