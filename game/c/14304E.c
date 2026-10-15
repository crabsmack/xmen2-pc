/* TechWallGun_UpdateBullets - the wall guns' bullet timer (TechWallGun_ClassInit), every frame:
 * update each bullet in use (TechWallGun_UpdateBullet).
 *
 * recs: the 16 bullets (g_TechWallGunRecs, the timer's argument). */
#include "xmen2.h"

void TechWallGun_UpdateBullets(register void *recs)
{
	register WallGunBullet *b = (WallGunBullet *)recs;
	short n = 16;

	while (n) {
		if (b->flags)
			TechWallGun_UpdateBullet(b);
		--n;
		b++;
	}
	if (0)		/* (recs keeps its own register, as in the ROM) */
		recs = 0;
}
