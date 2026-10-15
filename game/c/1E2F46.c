/* Cyclops_BeamHitEnemies - CYCLOPS the beam's hits (Cyclops_Draw, while the beam is active): every
 * enemy body not in hit cooldown that one of the beam's pieces overlaps is hit.
 *
 * self: the player.
 * Counts beam.frameCount; its parity picks the piece list tested (even: the first, with
 * beam.count1 pieces; odd: the second) so it follows what Cyclops_DrawBeam shows. The origin is
 * the player's position plus the level's offset (mirrored facing left); a piece is a w x h box
 * (its size code) at its offset, mirrored too. On the first overlap with an enemy's second box
 * the enemy's TakeDamage (slot +$26) gets (no attacker, the beam's hit type, its strength, the
 * centre of the overlap), and the next enemy is tested. */
#include "xmen2.h"

void Cyclops_BeamHitEnemies(Cyclops *self)
{
	short ox, oy, count, size, h, left, top, right, bottom;
	ListNode *node;
	register Actor *target;
	register BeamPiece *piece;
	register unsigned short i;
	short x, y, w;

	self->beam.frameCount++;
	if (g_HitListEnemyBody) {
		node = g_HitListEnemyBody;
		do {
			target = (Actor *)node->owner;
			if (target->hitCooldown > 0)
				continue;
			if (!(self->spr.flags & 0x10)) {
				ox = (self->x >> 8) + self->beam.level->dx;
				oy = (self->y >> 8) + self->beam.level->dy;
			} else {
				ox = (self->x >> 8) - self->beam.level->dx;
				oy = (self->y >> 8) + self->beam.level->dy;
			}
			if (!(self->beam.frameCount & 1)) {
				piece = BEAM_PIECES(self->beam.level, list1);
				count = self->beam.count1;
			} else {
				piece = BEAM_PIECES(self->beam.level, list2);
				count = self->beam.count2;
			}
			for (i = 0; i < count; i++, piece++) {
				size = piece->size & 0xF;
				w = ((size >> 2 & 3) + 1) << 3;
				h = ((size & 3) + 1) << 3;
				y = oy + piece->y;
				if (!(self->spr.flags & 0x10))
					x = ox + piece->x;
				else
					x = ox - piece->x - w;
				if (x > target->box2Right)
					continue;
				if (x + w < target->box2Left)
					continue;
				if (y > target->box2Bottom)
					continue;
				if (y + h < target->box2Top)
					continue;
				left = MAX(x, target->box2Left);
				top = MAX(y, target->box2Top);
				right = MIN(x + w, target->box2Right);
				bottom = MIN(y + h, target->box2Bottom);
				(*target->cls->method[8])(target, 0, self->hitType, self->attack,
					(left + right) >> 1, (top + bottom) >> 1);	/* TakeDamage */
				break;
			}
		} while ((node = node->next) != 0);
	}
}
