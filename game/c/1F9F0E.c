/* Collide_ObjVsList - one attacker against every object of a body hit list: each one it touches
 * gets hit (the attacker's Attack, class slot +$22).
 *
 * att: the attacker.  node: the first node of the body list.
 * The attacker's frame needs a hit-point list (the frame record's hitPoints; none for no frame
 * $FF). A body is tested when its box comes within $50 px of the attacker's and it is not
 * flashing from a hit (hitCooldown); Obj_FindHitPoint then finds where they touch. The walk
 * stops at once when the attacker has flags $10 (before and after hitting). Attack gets
 * (att, body, att->hitType, att->attack, x, y). */
#include "xmen2.h"

void Collide_ObjVsList(register Actor *att, ListNode *node)
{
	s16 hx;
	s16 hy;
	s8 *pts;
	register short x;
	register short y;

	{
		register short frame = att->spr.frame;
#ifdef XMEN2_MPW
		register SprFrame *fr = SPR_FRAME(att->spr.def, frame);

		if (frame == 0xFF || !fr->hitPoints)
			pts = 0;
		else
			pts = (s8 *)att->spr.def + fr->hitPoints;
#else
		SprDef *def = (SprDef *)att->spr.def;
		SprFrame *fr;

		if (!def)	/* no definition: the 68000 reads the vector table */
			def = (SprDef *)Vectors;
		fr = SPR_FRAME(def, frame);
		if (frame == 0xFF || !fr->hitPoints)
			pts = 0;
		else
			pts = (s8 *)def + fr->hitPoints;
#endif
	}
	x = att->x >> 8;
	y = att->y >> 8;
	if (!pts)
		return;
	while (node) {
		register Actor *body = (Actor *)node->owner;

		if (att->boxLeft - 0x50 < body->boxRight && att->boxRight + 0x50 > body->boxLeft
		    && att->boxTop - 0x50 < body->boxBottom && att->boxBottom + 0x50 > body->boxTop
		    && !body->hitCooldown && Obj_FindHitPoint(body, att, pts, x, y, &hx, &hy)) {
			if (att->flags & 0x10)
				return;
			(*att->cls->method[7])(att, body, att->hitType, att->attack, hx, hy);	/* Attack */
			if (att->flags & 0x10)
				return;
		}
		node = node->next;
	}
}
