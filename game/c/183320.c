/* Beast_ShockwaveStep - move one stomp shockwave 10 px along the floor, hit what it meets, draw
 * it; does it end here?
 *
 * px, py:      the wave's position (pixels; Beast waveLeftX or waveRightX, waveY).
 * spr:         the waves' sprite (Beast waveSpr), h-flipped for the left wave.
 * right:       1 the right wave, 0 the left one.
 * priority:    the sprite priority bit (Beast's own): set on the wave and its explosion.
 * hitsEnemies: the wave hits the enemy bodies with hit type 10 (right) / 9 (left), knocking
 *              them its way; else the players' bodies, type 4 (Beast_ShockwaveHitCb).
 * The hit point is 15 px above py at the new x (Beast_ShockwaveHitCb); the wall test
 * (Tile_TestAt, solid map tiles $7800) 30 px ahead of the old x. A wall there: an explosion
 * there, and the wave ends (returns 1). It also ends past 380 px (right) or -60 px (left) of
 * the screen; returns 0 otherwise. */
#include "xmen2.h"

char Beast_ShockwaveStep(register s16 *px, s16 *py, register SprInst *spr, char right,
	unsigned char priority, char hitsEnemies)
{
	ListNode *list = hitsEnemies ? g_HitListEnemyBody : g_HitListPlayerBody;
	register short y = *py + -15;

	if (priority)
		spr->flags |= 4;
	if (right) {
		short type = hitsEnemies ? 10 : 4;
		short ahead = *px + 30;

		*px += 10;
		ObjList_ForEachAtPoint(*px, y, list, FN_DATA(Beast_ShockwaveHitCb), 0, type);
		spr->flags &= ~0x10;
		Spr_Draw(spr, *px - g_CameraX, *py - g_CameraY);
		if ((char)Tile_TestAt(0, ahead, y, !g_SolidMapIsBg, 0x7800, 1)) {
			Explosion_Spawn(ahead << 8, y << 8, priority);
			return 1;
		}
		if (*px - g_CameraX > 0x17C)
			return 1;
	} else {
		short type = hitsEnemies ? 9 : 4;
		short ahead = *px - 30;

		*px -= 10;
		ObjList_ForEachAtPoint(*px, y, list, FN_DATA(Beast_ShockwaveHitCb), 0, type);
		spr->flags |= 0x10;
		Spr_Draw(spr, *px - g_CameraX, *py - g_CameraY);
		if ((char)Tile_TestAt(0, ahead, y, !g_SolidMapIsBg, 0x7800, 1)) {
			Explosion_Spawn(ahead << 8, y << 8, priority);
			return 1;
		}
		if (*px - g_CameraX < -0x3C)
			return 1;
	}
	return 0;
}
