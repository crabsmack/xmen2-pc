/* Obj_CollideSegs - push an object out of the floor segments it overlaps, one contact per pass.
 *
 * obj: the object (any Actor class; Actor_Update, XPlayer_Move, Beast_RollPathClear).
 * Seg_GatherNearObj lists the segments near it (up to 30 contacts, ended by flags 1). Each pass
 * (Seg_CollideObj on every contact not skipped, flags 4):
 *   - 3 or more: only touching; the highest such segment layer is kept (the layer of a
 *     free-moving object at the end, with its own segments: Seg_SetLayerFromObj);
 *   - 2, a push-out, from a segment other than the one it walks on, in its layer (or a static
 *     segment of layer 0, owner -2): pushed up (pushY > 0) while walking: blocked, skipped from
 *     now on (segment flags bit 6; a player not in hit cooldown calls g_SegBlockedHook(obj, list,
 *     best so far) and the segment lets it pass for 30 frames). Else the push |x| + |y| (bytes;
 *     the second push with flags $10) competes: event contacts (flags bit 15) for the smallest,
 *     the others for the smallest, or, both pushing down, the one pushing down more.
 * The pass's contact (an event contact only when there is no other) is resolved: the segment
 * type's hook (g_SegTypeHooks, message 0) answers 0: g_SegResolveHook moves the object (contact
 * flags 2 / $40 for a push in x / y), 1: the contact is just skipped. A landing (contact flags 8,
 * unless flags6 $2000) makes the object walk on the segment: detached, attached to the segment's
 * owner (unless -2), its layer (unless type $F, and to what it carries), its position along it.
 * An event contact ends the passes (its event byte: debug stubs). After 30 passes a player is put
 * back where the last call left it (once per call: then 5 more passes), others stop (a player
 * already put back calls g_SegBlockedHook). The skipped segments lose flags bit 6 at the end, and
 * a player's position is kept for next time (g_CollideSafeX/Y).
 * Hand-written 68000 in an MPW frame (a2 as a variable, word index sums): it can't byte-match.
 * The contact list is a local, as in the original; the hooks get pointers into it (each hook
 * only reads the contact it is given; g_SegBlockedHook is an empty function, Sys_Nop1F949E).
 * Phase 7 (R11): a player is IS_PLAYER_OBJ, slots 0-1 and players 3-4's objects: a stuck player
 * 3-4 is put back too (it fell through AvalonA3's floor), and they share g_CollideSafeX/Y with
 * players 1-2 as those two share it in the original. */
#include "xmen2.h"

/* |v| of a byte, as a byte (neg.b: -128 stays -128), widened. */
#define ABS8(v)		((s8)((v) < 0 ? -(v) : (v)))

/* Empty debug functions the original called with arguments (nothing reads them). */
#ifdef XMEN2_MPW
#define CALL_STUB(f, args)	(((void (*)())(f)) args)
#else
#define CALL_STUB(f, args)	(f())
#endif

void Obj_CollideSegs(void *obj)
{
	Actor *o = (Actor *)obj;
	SegContact list[30];
	SegContact *c;
	SegContact *best;
	SegContact *bestEvent;
	Seg *s;
	SegHookFn hook;
	short passes;
	s8 layer;
	short flags;
	short d;
	short bestD;
	short bestEventD;
	short r;
	u8 res;

	Seg_GatherNearObj(o, list);
	passes = 0;
	g_CollideReset = 0;
	do {
		layer = 0;
		best = 0;
		bestEvent = 0;
		bestD = bestEventD = 2000;
		passes++;
		for (c = list; !((flags = c->flags) & 1); c++) {
			if (flags & 4)
				continue;
			s = &g_Segs[c->seg];
			r = Seg_CollideObj(s, o, c);
			if (r < 2)
				continue;
			if (r != 2) {
				if (s->layer > layer)
					layer = s->layer;
				continue;
			}
			if (!(o->flags & 0x1000) && s == o->seg)
				continue;
			if (!(s->owner == -2 && s->layer == 0) && (u8)s->layer != o->layer)
				continue;
			if (c->pushY > 0 && !(o->flags & 0x1000)) {
				c->flags |= 4;
				s->flags |= 0x40;
				if (IS_PLAYER_OBJ(o)) {
					if (o->hitCooldown == 0)
						FN_AT(SegObjFn, g_SegBlockedHook)(o, list, best);
					s->passFrames = 30;
				}
				continue;
			}
			d = ABS8(c->pushY) + ABS8(c->pushX);
			if (flags & 0x10)
				d = ABS8(c->push2Y) + ABS8(c->push2X);
			if (flags < 0) {
				if (d < bestEventD) {
					bestEvent = c;
					bestEventD = d;
				}
				continue;
			}
			if (best && c->pushY < 0 && best->pushY < 0) {
				if (c->pushY < best->pushY) {
					best = c;
					bestD = d;
				}
				continue;
			}
			if (d < bestD) {
				best = c;
				bestD = d;
			}
		}
		if (!best)
			best = bestEvent;
		c = best;
		if (passes > 30) {
			if (IS_PLAYER_OBJ(o)) {
				if (!g_CollideReset) {
					g_CollideReset = 0xFF;
					passes = 25;
					o->x = g_CollideSafeX;
					o->y = g_CollideSafeY;
					o->vx = 0;
					o->vy = 0;
					Obj_UpdateBox(o);
					continue;		/* another pass when there was a contact */
				}
				FN_AT(SegObjFn, g_SegBlockedHook)(o, list, best);
			}
			c = 0;
		}
		if (!c)
			continue;
		s = &g_Segs[c->seg];
		hook = g_SegTypeHooks[(u8)s->type];
		res = hook ? (u8)hook(0L, s, o, c) : 0;
		if (res == 0) {
			FN_AT(SegHookFn, g_SegResolveHook)(s, o, c);
			if (c->pushX)
				c->flags |= 2;
			if (c->pushY)
				c->flags |= 0x40;
		} else if (res == 1) {
			c->flags &= 7;
			c->flags |= 4;
			s->flags |= 0x40;
		}
		if ((c->flags & 8) && !(o->flags6 & 0x2000)) {
			Obj_Detach(o);
			if (s->owner != -2)
				Obj_AttachChild((u8)s->owner == 0xFF ? 0 : (Actor *)g_ObjTable[(u8)s->owner], o);
			o->flags = (o->flags & ~0x1000) | 0x80;
			o->seg = s;
			if (s->type != 0xF)
				o->layer = s->layer;
			Obj_InheritLayer(o, o);
			Seg_ComputeObjPos(s, o);
			Seg_ApplyObjPos(o);
			c->flags |= 4;
			s->flags |= 0x40;
		}
		if (c->flags < 0) {
			if (c->event) {
				if (IS_PLAYER_OBJ(o)) {
					if (c->event == 4)
						CALL_STUB(Sys_Nop1FA8C4, (0L, 2L, 0L));
					else
						CALL_STUB(Sys_Nop1FA8CC, (-1L, 0x63L));
				} else
					CALL_STUB(Sys_Nop1FA8D4, (o, 0x63L, 0L));
			}
			c = 0;
		}
		Obj_UpdateBox(o);
	} while (c);
	for (c = list; !(c->flags & 1); c++) {
		if (c->flags & 4)
			g_Segs[c->seg].flags &= ~0x40;
	}
	if (IS_PLAYER_OBJ(o)) {
		g_CollideSafeX = o->x;
		g_CollideSafeY = o->y;
	}
	if (o->flags & 0x1000) {
		o->layer = layer;
		if (o->segIndex != 0xFF)
			Seg_ForEachOfObj(o, Seg_SetLayerFromObj);
	}
}
