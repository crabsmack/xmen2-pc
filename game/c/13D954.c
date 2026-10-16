/* ExplodeFx_BurstStart - turn a killed object's memory into a Burst effect: all cleared but its
 * class pointer (its index kept for ExplodeFx_Free), flags $8010 (| 1 high priority), the
 * explosion sound ($38, then 6 frames before it can play again), its update timer
 * (ExplodeFx_BurstUpdate, every frame) and its draw-list entry (ExplodeFx_BurstDraw).
 *
 * fx: the object.  layer: its draw layer.  unk43: its +$43 (unused).
 * prio: its sprite's priority bit (non-zero: high priority, also in the draw-list key).
 * flag: its flags6 bit 3 (passed on to DrawList_Add as 8). */
#include "xmen2.h"

void ExplodeFx_BurstStart(register ExplodeFx *fx, unsigned char layer, char unk43,
	register char prio, char flag)
{
	register char index = ((Actor *)fx)->index;

	Mem_Set(&fx->flags, 0, 0xBE);
	fx->index = index;
	fx->flags = 0x8010 | (prio ? 1 : 0);
	Snd_Stop(0x38);
	Snd_Play(0x38);
	fx->soundDelay = 6;
	if (fx->timer = Timer_Create((TimerFn)ExplodeFx_BurstUpdate, 1, fx, 1))
		Timer_Start(fx->timer);
	fx->drawNode = DrawList_Add((AnyFn)ExplodeFx_BurstDraw, fx, prio ? 4 : 0, layer, flag ? 8 : 0,
		0);
}
