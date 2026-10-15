/* XClone_TakeDamage - XCLONE is hit (class slot +$26): count the hit, the parent's TakeDamage, then
 * note a kill by the clone's own character, throw debris (sound $D8) and show the hurt frame.
 *
 * self:     the clone.
 * attacker: the object that hit it.
 * type, damage: the hit's type and damage (passed on).
 * x, y:     where it was hit (pixels); debris only when both are > 0: damage * 2 pieces
 *           (6 to 16), landing on the arena floor.
 * A hit that kills the clone (or finds it already stopped: flags bit 4) by the same character
 * (Clone_CharTypeOf) sets bit 8 + type of the clone flags (XClone_TileHookReward). */
#include "xmen2.h"

#define g_CloneFlags	(*(s16 *)g_SceneVar3B08)	/* scene variable: 1 arena locked, 2 reward given, $100 << type killed by its own character */
#define g_CloneDebris	(*(PTR(DebrisPool) *)g_SceneVar3B1C)	/* scene variable: the clone-maker debris pool */

void XClone_TakeDamage(XClone *self, Actor *attacker, char type, char damage, short x, short y)
{
	short floorY;
	short myType;
	short pieces;

	floorY = self->arenaFloor;
	myType = Clone_CharTypeOf(self);
	self->recentHits += 60;
	(*CLASS(Tbl_XClone_vtable)->parent->method[8])(self, attacker, type, damage, x, y);	/* BIONPC TakeDamage */
	if ((self->flags & 0x10 || self->health <= 0) && myType == Clone_CharTypeOf(attacker))
		g_CloneFlags |= 1 << (myType + 8);
	if (x > 0 && y > 0) {
		pieces = damage << 1;
		pieces = MAX(pieces, 6);
		pieces = MIN(pieces, 16);
		Debris_Spawn(x, y, g_CloneDebris, pieces);
		Snd_Stop(0xD8);
		Snd_Play(0xD8);
		Debris_SetFloorY(g_CloneDebris, floorY);
	}
	if (!(self->flags & 0x10))
		(*self->cls->method[20])(self, self->charRec->hurtFrame);	/* SetFrame */
}
