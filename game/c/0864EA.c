/* BroodBossDamage_Update - BROODBOSSDAMAGE Update (class slot +$4A): the parent's (BROODBOSSPARTS)
 * Update, then, unless frozen (flags bit 4) or without rectangles: each live player that is not
 * flashing and whose second box touches one of the rectangles (placed at the object's pixel
 * position) takes damage once: TakeDamage (class slot +$26) from no attacker, hit type 5, 1 point,
 * at (0, 0).
 *
 * self: the object. */
#include "xmen2.h"

void BroodBossDamage_Update(register BroodBossDamage *self)
{
	register DamageRect *r;
	short ox;
	short oy;
	unsigned short i;

	(*CLASS(Tbl_BroodBossDamage_vtable)->parent->method[17])(self);	/* BROODBOSSPARTS Update */
	if (self->flags & 0x10)
		return;
	if (!self->rects)
		return;
	r = self->rects;
	ox = self->x >> 8;
	oy = self->y >> 8;
	if (IS_ALIVE(&g_Player1) && !g_Player1.hitCooldown) {
		for (i = 0; i < self->rectCount; i++, r++) {
			if (g_Player1.box2Left <= ox + r->right && g_Player1.box2Right >= ox + r->left
			    && g_Player1.box2Top <= oy + r->bottom && g_Player1.box2Bottom >= oy + r->top) {
				(*g_Player1.cls->method[8])(&g_Player1, 0, 5, 1, 0, 0);	/* +$26 TakeDamage */
				break;
			}
		}
	}
	r = self->rects;
	if (IS_ALIVE(&g_Player2) && !g_Player2.hitCooldown) {
		for (i = 0; i < self->rectCount; i++, r++) {
			if (g_Player2.box2Left <= ox + r->right && g_Player2.box2Right >= ox + r->left
			    && g_Player2.box2Top <= oy + r->bottom && g_Player2.box2Bottom >= oy + r->top) {
				(*g_Player2.cls->method[8])(&g_Player2, 0, 5, 1, 0, 0);	/* +$26 TakeDamage */
				break;
			}
		}
	}
#if defined(XMEN2_NATIVE)
	P34_Each([&](XPlayer *q) {	/* players 3-4 in play too (R11) */
		r = self->rects;
		if (IS_ALIVE(q) && !q->hitCooldown) {
			for (i = 0; i < self->rectCount; i++, r++) {
				if (q->box2Left <= ox + r->right && q->box2Right >= ox + r->left
				    && q->box2Top <= oy + r->bottom && q->box2Bottom >= oy + r->top) {
					(*q->cls->method[8])(q, 0, 5, 1, 0, 0);	/* +$26 TakeDamage */
					break;
				}
			}
		}
	});
#endif
}
