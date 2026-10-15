/* ApocBomb_Explode - a hittable APOCBOMB is destroyed (ApocBomb_TakeDamage): an explosion, a
 * power-up ($D2), the screen shakes (Tbl_ApocBombQuake); one more destroyed bomb (up to 12): the
 * 12th starts the room's finale (unk_BaniMazaA2_04766A), the others step the room palette
 * (BaniMazaA2_SetPaletteStep).
 *
 * self: the bomb. */
#include "xmen2.h"

#define g_ApocBombsDestroyed	(*(s16 *)g_SceneVar3B0C)	/* scene variable: hittable bombs destroyed (max 12) */

void ApocBomb_Explode(ApocBomb *self)
{
	register long x = self->x;
	register long y = self->y;

	Explosion_Spawn(x, y, 0);
	SegmentPlusPowerUp_Spawn(x, y, 0xD2);
	g_CamShake = CODEPTR(Tbl_ApocBombQuake);
	if (g_ApocBombsDestroyed < 12)
		++g_ApocBombsDestroyed;
	if (g_ApocBombsDestroyed == 12)
		unk_BaniMazaA2_04766A();
	else
		BaniMazaA2_SetPaletteStep();
}
