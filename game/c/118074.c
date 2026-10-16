/* Explosion_SpawnEx - start an explosion puff centred on (x, y) in the first free slot of
 * g_Explosions: five animation steps of 4 frames, a 32x32 sprite of the g_FxTileBase tiles in
 * palette 1, run by its own timer (Explosion_Update every frame), sound $38 (restarted).
 *
 * x, y:     centre (8 fraction bits).
 * priority: non-zero: the sprite has high priority (callers pass their own sprite's bit 2).
 * hurts:    non-zero: it hurts the objects in its box each frame (Explosion_HurtActor).
 * Returns the slot, or 0 when all 10 are in use or no timer is free.
 * (The ROM loads the slot array before the parameters: they are copied into register locals.) */
#include "xmen2.h"

Explosion *Explosion_SpawnEx(long px, long py, unsigned char ppriority, short hurts)
{
	register Explosion *slots = g_Explosions;
	register unsigned char priority = ppriority;
	register long y = py;
	register long x = px;
	register Explosion *e = &slots[0];
	register short i;

	for (i = 0; i < 10; i++) {
		if (!e->timer) {
			e->x = (short)(x >> 8) - 16;
			e->y = (short)(y >> 8) - 16;
			e->hurts = hurts;
			e->delay = 4;
			e->step = 0;
			e->attr = (g_FxTileBase | 0x2000) | (priority ? 0x8000 : 0);
			if ((e->timer = Timer_Create((TimerFn)Explosion_Update, 1, e, 1)) != 0) {
				Timer_Start(e->timer);
				Snd_Stop(0x38);
				Snd_Play(0x38);
				return e;
			}
			return 0;
		}
		e++;
	}
	return 0;
}
