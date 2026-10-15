/* BroodBoss_Defeat - BROODBOSS defeated (BroodBoss_StateHurt at 0 health): unhittable (flags6 bit 1,
 * out of the enemy body hit list), kill every live BROODBOSSDAMAGE hurt zone, end the fight
 * (g_BroodBossFlags bit 0, no floor segment type $17 hook).
 *
 * self: the boss. */
#include "xmen2.h"

#define g_BroodBossFlags	(*(s16 *)g_SceneVar3B0C)	/* scene variable: 1 fight on, 2 floor shock, $10 lower the body, $40 wall smashed */

void BroodBoss_Defeat(BroodBoss *self)
{
	register short i = 0;
	register Actor *o;

	self->flags6 |= 2;
	List_Unlink(self->bodyNode, &g_HitListEnemyBody);
	self->bodyNode = 0;
	for (i = 0, o = g_ObjTable[0]; i < g_ObjCount; o = g_ObjTable[++i]) {
		if (o->flags & 0x8000 && o->cls == CLASS(Tbl_BroodBossDamage_vtable))
			(*o->cls->method[19])(o);		/* +$52 Kill */
	}
	g_BroodBossFlags &= ~1;
	g_SegTypeHooks[0x17] = 0;
}
