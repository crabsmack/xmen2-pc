/* BroodBoss_Update - BROODBOSS Update (class slot +$4A): the parent's (BROODBOSSPARTS) Update, then,
 * unless it is frozen (flags bit 4): the body's y offset g_BroodBossOffsetY moves 2 px a frame
 * down to $2B while g_BroodBossFlags bit 4 is set (then clears it), else back up to $33; the
 * health bar gets health x 2; and during the floor shock (flag bit 1) each live player that is
 * not flashing, in the view (x) and whose box reaches the floor line (y $62D-$631) is attacked
 * (type 4, its attack damage) at the middle of its box, on that line.
 *
 * self: the boss. */
#include "xmen2.h"

#define g_BroodBossFlags	(*(s16 *)g_SceneVar3B0C)	/* scene variable: 1 fight on, 2 floor shock, $10 lower the body, $40 wall smashed */

void BroodBoss_Update(BroodBoss *self)
{
	register short x;
	register short y;

	(*CLASS(Tbl_BroodBoss_vtable)->parent->method[17])(self);
	if (!(self->flags & 0x10)) {
		(void)(g_BroodBossFlags & 0x40);
		if (g_BroodBossFlags & 0x10) {
			g_BroodBossOffsetY -= 0x200;
			if ((g_BroodBossOffsetY >> 8) < 0x2B) {
				g_BroodBossFlags &= ~0x10;
				g_BroodBossOffsetY = 0x2B00;
			}
		} else if ((g_BroodBossOffsetY >> 8) != 0x33) {
			g_BroodBossOffsetY += 0x200;
			if ((g_BroodBossOffsetY >> 8) > 0x33)
				g_BroodBossOffsetY = 0x3300;
		}
		if (self->health > 0)
			g_BroodBossHealthBar = self->health << 1;
		if (g_BroodBossFlags & 2) {
			if (IS_ALIVE(&g_Player1) && !g_Player1.hitCooldown
			    && g_CameraX <= g_Player1.box2Right && g_CameraX + 0x140 >= g_Player1.box2Left
			    && g_Player1.box2Bottom >= 0x62D && g_Player1.box2Top <= 0x631) {
				x = g_Player1.box2Left + ((g_Player1.box2Right - g_Player1.box2Left) >> 1);
				y = 0x62D;
				(*self->cls->method[7])(self, &g_Player1, 4, self->attack, x, y);	/* +$22 Attack */
			}
			if (IS_ALIVE(&g_Player2) && !g_Player2.hitCooldown
			    && g_CameraX <= g_Player2.box2Right && g_CameraX + 0x140 >= g_Player2.box2Left
			    && g_Player2.box2Bottom >= 0x62D && g_Player2.box2Top <= 0x631) {
				x = g_Player2.box2Left + ((g_Player2.box2Right - g_Player2.box2Left) >> 1);
				y = 0x62D;
				(*self->cls->method[7])(self, &g_Player2, 4, self->attack, x, y);	/* +$22 Attack */
			}
		}
	}
}
