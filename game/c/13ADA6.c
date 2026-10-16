/* Mine_Explode - mine handler: the explosion. An Explosion at the mine, a hit on each player near
 * it, then the mine is gone (no handler: Mine_PoolUpdate removes it) and a small burst effect
 * plays (a new ACTOR turned into ExplodeFx_BurstSmall).
 *
 * m: the mine.
 * A player alive and not in hit cooldown whose second box meets the 120 x 10 px box from 40 px
 * above the mine (a spore's is 80 px high: SporeMine_RecExplode) takes TakeDamage (class slot
 * +$26) from no attacker, hit type 4, 1 damage, at the mine's position. The hit type it computes
 * first (10 when the mine is left of the player, else 9) is never used: kept. */
#include "xmen2.h"

void Mine_Explode(register MineRec *m)
{
	long x, y;
	register short i;
	register XPlayer *p;
	register Actor *fx;

	x = m->x << 8;
	y = m->y << 8;
	Explosion_Spawn(x, y, 0);
	for (i = 0; i < 2; ++i) {
		if (IS_ALIVE(i == 0 ? (p = &g_Player1) : (p = &g_Player2))) {
			short px = x >> 8;
			register short py = y >> 8;
			register short left = px - 0x3C;
			register short top = py - 0x28;
			short right = left + 0x78;
			short bottom = top + 0xA;

			if (IS_ALIVE(p) && !p->hitCooldown && left <= p->box2Right && right >= p->box2Left
			    && top <= p->box2Bottom && bottom >= p->box2Top) {
				short type = x < p->x ? 10 : 9;

				if (0)
					type = type;
				(*p->cls->method[8])(p, 0, 4, 1, px, py);	/* +$26 TakeDamage */
			}
		}
	}
	m->handler = 0;
	if (fx = (Actor *)(*CLASS(Tbl_Actor_vtable)->method[2])(0, CLASS(Tbl_Actor_vtable), x, y, 0))	/* New */
		ExplodeFx_BurstSmall(fx);
}
