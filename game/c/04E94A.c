/* ApocBomb_Spawn - drop an APOCBOMB (Apocalypse_StateDropBombs): a new one at (256, -20) px with
 * the round's speed.
 *
 * speed: its orbit speed.  param: low nibble = kind 0-3 (0 never bounces; 1 rises once after
 * landing, height step -100; 2 and 3 bounce, steps 100 and 200; other values as 0), high nibble
 * = type: 0 harmless (palette 2), $10 hittable (palette 1, frame 9, in the enemy body hit list).
 * The cases are ApocBomb_Spawn_part1 (0 and default), _part2 (1), _part3 (2 and 3) and _part4
 * (the type).
 *
 * Inline here, separate blocks in the disassembly: ApocBomb_Spawn_part2, ApocBomb_Spawn_part3, ApocBomb_Spawn_part4.
 */
#include "xmen2.h"

void ApocBomb_Spawn(short speed, register short param)
{
	register long x = 0x10000;
	register long y = -0x1400;
	register ApocBomb *b;

	b = (ApocBomb *)(*CLASS(Tbl_ApocBomb_vtable)->method[2])(0,	/* New */
		CLASS(Tbl_ApocBomb_vtable), x, y, 0);
	if (b) {
		register short kind = param & 0xF;
		register short type = param & 0xF0;

		b->speed = speed;
		b->kind = kind;
		b->type = type;
		switch (kind) {
		default:
		case 0:
			b->heightStep = 0;
			break;
		case 1:
			b->heightStep = -100;
			break;
		case 2:
			b->bombFlags |= 2;
			b->heightStep = 100;
			break;
		case 3:
			b->bombFlags |= 2;
			b->heightStep = 200;
			break;
		}
		if (type == 0)
			b->spr.flags = b->spr.flags & ~3 | 2;
		else if (type == 0x10) {
			b->flags6 &= ~2;
			b->bodyNode = HitList_Add(b, &g_HitListEnemyBody);
			b->spr.flags = b->spr.flags & ~3 | 1;
			(*b->cls->method[20])(b, 9);		/* +$56 SetFrame */
		}
	}
}
