/* Seg_ForEachOfObj - call func(obj, segment) for every floor segment an object owns.
 *
 * obj: the owner (any Actor class), its first segment segIndex; each segment's unk_13 links
 * the next one ($FF ends). func: e.g. Seg_SetLayerFromObj. last is never read (the
 * original's). */
#include "xmen2.h"

void Seg_ForEachOfObj(void *obj, SegObjFn func)
{
	register Actor *o = (Actor *)obj;
	register Seg *s;
	short i;
	short n;
	short last;

	i = o->segIndex;
	while (i != 0xFF) {
		last = n = i;
		s = &g_Segs[n];
		i = s->unk_13;
		func(o, s);
	}
}
