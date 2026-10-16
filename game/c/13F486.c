/* Debris_SetSprDef - set the sprite-definition slot a debris pool draws its pieces with.
 *
 * pool: the pool.  def: the g_SpriteDefs slot (-1: the pool is neither moved nor drawn). */
#include "xmen2.h"

void Debris_SetSprDef(DebrisPool *pool, short def)
{
	pool->sprDef = def;
}
