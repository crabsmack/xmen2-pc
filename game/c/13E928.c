/* ExplodeFx_ChainStart - turn a killed object's memory into a chain explosion: all cleared but its
 * class pointer (its index kept for ExplodeFx_Free), flags $8010 (| 1 high priority), the
 * explosion sound ($38, then 6 frames before it can play again), its update timer
 * (ExplodeFx_ChainUpdate, every frame), its draw-list entry (ExplodeFx_ChainDraw), and the chain:
 * origin (x, y), the path from its first step at once, the repeats, and its flags (damaging, the
 * sprite's h-flip and v-flip taken before the clear).
 *
 * fx: the object.  layer, unk43, prio, flag: as ExplodeFx_BurstStart.
 * x, y: origin (pixels).  damaging: the explosions hurt both sides (ExplodeFx_ChainHit).
 * path: the path script.  repeats: low byte runs after the first (negative: forever), high
 * byte frames before each. */
#include "xmen2.h"

void ExplodeFx_ChainStart(register ExplodeFxChain *fx, unsigned char layer, char unk43,
	register char prio, char flag, short x, short y, char damaging, FxChainStep *path,
	register short repeats)
{
	register char flips;
	register char index;

	flips = 0;
	if (((Actor *)fx)->spr.flags & 0x10)
		flips |= 2;
	if (((Actor *)fx)->spr.flags & 8)
		flips |= 4;
	index = ((Actor *)fx)->index;
	Mem_Set(&fx->flags, 0, 0xBE);
	fx->index = index;
	fx->flags = 0x8010 | (prio ? 1 : 0);
	Snd_Stop(0x38);
	Snd_Play(0x38);
	fx->soundDelay = 6;
	if (fx->timer = Timer_Create((TimerFn)ExplodeFx_ChainUpdate, 1, fx, 1))
		Timer_Start(fx->timer);
	fx->drawNode = DrawList_Add((AnyFn)ExplodeFx_ChainDraw, fx, prio ? 4 : 0, layer, flag ? 8 : 0,
		0);
	fx->step = path;
	fx->path = fx->step;
	fx->x = x;
	fx->y = y;
	fx->delay = 0;
	fx->repeats = repeats & 0xFF;
	fx->repeatDelay = (repeats & 0xFF00) >> 8;
	fx->chainFlags = 0;
	if (damaging)
		fx->chainFlags |= 1;
	fx->chainFlags |= flips;
}
