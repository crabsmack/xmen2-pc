/* Gambit_CardCollide - hit-test a flying card: an enemy body at its point takes the hit
 * (Gambit_CardHitEnemy); else solid map tiles ($7800, Tile_TestAt) there: the card backs out
 * along its path in quarter steps until clear and explodes there.
 *
 * card: the card. Returns 1 when it hit something (the caller frees it), 0 otherwise. */
#include "xmen2.h"

char Gambit_CardCollide(register GambitCard *card)
{
	register long x, y;
	register short px, py;

	x = card->x;
	y = card->y;
	px = (short)(x >> 8);
	py = (short)(y >> 8);
	if (ObjList_ForEachAtPoint(px, py, g_HitListEnemyBody, FN_DATA(Gambit_CardHitEnemy), 0, (long)ADDR68(card)))
		return 1;
	if ((char)Tile_TestAt(0, px, py, !g_SolidMapIsBg, 0x7800, 1)) {
		register short dx, dy;

		dx = card->vx >> 2;
		dy = card->vy >> 2;
		do {
			x -= dx;
			y -= dy;
		} while ((char)Tile_TestAt(0, x >> 8, y >> 8, !g_SolidMapIsBg, 0x7800, 0));
		Explosion_Spawn(x, y, 1);
		return 1;
	}
	return 0;
}
