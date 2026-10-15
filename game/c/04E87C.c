/* ApocBomb_TakeDamage - APOCBOMB TakeDamage (class slot +$26; the parent's isn't called): any hit
 * destroys it. Hidden (frame $FF), one live bomb less; the hittable bomb (type $10) explodes
 * (ApocBomb_Explode) and leaves a chain of explosions from the arena centre (256, 256) along
 * the path Art_ApocBombNem2 (ExplodeFx_ChainRepeat), the others just burst (ExplodeFx_BurstSmall).
 *
 * self: the bomb.  attacker, type, damage, hx, hy (the hit): unused. */
#include "xmen2.h"

#define g_ApocBombCount	(*(s16 *)g_SceneVar3B08)	/* scene variable: live bombs */

void ApocBomb_TakeDamage(register ApocBomb *self, Actor *attacker, char type, char damage, short hx, short hy)
{
	(*self->cls->method[20])(self, 0xFF);		/* +$56 SetFrame: hide */
	g_ApocBombCount--;
	if (self->type == 0x10) {
		register void *path = CODEPTR(Art_ApocBombNem2);

		ApocBomb_Explode(self);
		self->x = 0x10000;
		self->y = 0x10000;
		ExplodeFx_ChainRepeat(self, path, 0, -16, -16, 0);
	} else
		ExplodeFx_BurstSmall(self);
}
