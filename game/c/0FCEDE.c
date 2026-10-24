/* SiberiaB1_DebrisUpdate - the ice shard timer (every frame, SiberiaB1_InstallTileHooks): move
 * each shard in use (twice in two-player step mode; gravity in vy); one below the floor line
 * (g_IceFloorY) bounces: mirrored above it, vy reversed at half speed, vx doubled, next frame
 * set; a shard past its fourth set or bouncing slower than $30 is freed. The others animate
 * (frame 0-2, every 4-7 frames) and age, and are drawn when on screen: one 16x16 hardware
 * sprite (size 5) of the shard sprite definition, frame (set + frame) * 4, high priority,
 * h-flipped while age bit 3 is set, 9 px lower than its position.
 * The screen position is unsigned: the left and top tests never fail. The flip bit is taken
 * from age as a signed byte (the ROM's `moveq #8 / and.b`). */
#include "xmen2.h"

#define g_IceFloorY	(*(s16 *)g_SceneVar3B0C)	/* scene variable: floor line the shards bounce on, pixels (SiberiaB1_IceTileHit) */
#define g_IceShardSprDef	(*(s16 *)g_SceneVar3B0E)	/* scene variable: sprite-definition slot of the ice shards */
#define g_IceShardCount	(*(s16 *)g_SceneVar3B10)	/* scene variable: ice shards in use */
#define g_IceShards	(*(PTR(IceShard) *)g_SceneVar3B24)	/* scene variable: the ice shard pool (48) */

#define SHARD_DX	0	/* a shard is drawn this far right of its position */
#define SHARD_DY	(-9)	/* and this far below */

void SiberiaB1_DebrisUpdate(void)
{
	register IceShard *p = g_IceShards;
	register short i;
	register short attr;
	register unsigned short sx;
	register unsigned short sy;
	register SprInst *def;

	for (i = 0; i < g_IceShardCount; i++, p++) {
		p->x += p->vx;
		p->y += p->vy;
		p->vy += p->ay;
		if (g_TwoPlayerStep) {
			p->x += p->vx;
			p->y += p->vy;
			p->vy += p->ay;
		}
		sx = p->x >> 8;
		sy = p->y >> 8;
		if (sy > g_IceFloorY) {
			p->y = (g_IceFloorY - (sy - g_IceFloorY)) << 8;
			sy = p->y >> 8;
			p->vy = -(p->vy - (p->vy >> 1));
			p->vx += p->vx;
			p->frameSet += 3;
			if (p->frameSet > 9 || ABS(p->vy) < 0x30) {
				SiberiaB1_DebrisFree(p);
				i--;
				p--;
				continue;
			}
		}
		if (p->frameDelay-- <= 0) {
			++p->frame;
			if (p->frame > 2)
				p->frame = 0;
			p->frameDelay = ((unsigned short)Rand_Next() & 3) + 4;
		}
		++p->age;
		sx -= g_CameraX - SHARD_DX;
		sy -= g_CameraY - SHARD_DY;
		if (sx > -20 && sx < 0x154 && sy > -10 && sy < 0xFE) {
			def = &g_SpriteDefs[g_IceShardSprDef];
			attr = (p->frameSet + p->frame) << 2;
			attr = def->tileBase + (attr | 0 | ((signed char)p->age & 8) << 8 | 0x8000);
			g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
			g_SprWritePtr->y = sy + 0x80;
			g_SprWritePtr->size = 5;
			g_SprWritePtr->attr = attr;
			g_SprWritePtr->x = sx + 0x80;
		}
	}
}
