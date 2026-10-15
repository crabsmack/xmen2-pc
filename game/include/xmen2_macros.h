/* xmen2_macros.h - the idioms the original C used over and over, as macros (hand-written C,
 * phase 4). xmen2disasm docs/COMPILER.md "How the original C was written" has the evidence:
 * state changes were do { } while (0) macros, and a?b:c conditions are materialized in d3. */
#ifndef XMEN2_MACROS_H
#define XMEN2_MACROS_H

/* An object is present and alive: flags bits $C000 = $8000. */
#define IS_ALIVE(o)		(((o)->flags & 0xC000) == 0x8000)

/* |v| as the original wrote it (tst / sge / neg: a materialized condition). */
#define ABS(v)			((v) >= 0 ? (v) : -(v))

/* The larger / smaller of two values as the original wrote them: `a > b ? a : b` (the compare
   order shows: MPW C's cmp/sgt vs cmp/slt; XClone_Init, _TakeDamage, _LockArena,
   _MakerBufSize). Both arguments are evaluated twice. */
#define MAX(a, b)		((a) > (b) ? (a) : (b))
#define MIN(a, b)		((a) < (b) ? (a) : (b))

/* Ends a function that returns a value on some paths and falls off the end on others, as the
   original did (MPW C accepts it; d0 then holds whatever the last instruction left, and no
   caller reads it: state functions). MPW: nothing. PC: C++ needs a value: 0, flagged so the
   slot entry leaves d0 alone and verify doesn't compare it.
   RETURN_NO_VALUE; is the same as a statement before the end (MPW: return;). */
#ifdef XMEN2_MPW
#define NO_RETURN_VALUE
#define RETURN_NO_VALUE		return
#else
#define NO_RETURN_VALUE		return (xm::g_novalue = xm::g_frame, 0);
#define RETURN_NO_VALUE		return (xm::g_novalue = xm::g_frame, 0)
#endif

/* Switch an object to state f now: store it and run it (Barbarian, Exodus...). */
#define GOTO_STATE(o, f)	do { (o)->state = (f); (*(f))(o); } while (0)
/* Switch an object to state f from the next update: reset the delay, store it. */
#define SET_STATE(o, f)		do { (o)->timer = 0; (o)->state = (f); } while (0)
/* The same after n frames ($FF: Actor_Update waits, the animation's end clears it). */
#define SET_STATE_WAIT(o, f, n)	do { (o)->timer = (n); (o)->state = (f); } while (0)

/* Go on with the next step of an object's AI script (cursor cur, an AiCursor) in n frames:
   the step's jmp thunk becomes the state, wrapping to step 0 after the last; with no script,
   the object's default state. AI_FIRST_STEP restarts the script at step 0. (SmallBrood,
   Brood, BigBio and SavageBio match with these; the step address is built as AiScript.step +
   index.) Where the loop is matters: any loop in a function makes MPW C flush pending
   argument pops before each branch. AI_NEXT_STEP's do/while is inside its else: right after a
   call the pop lands between the test and the branch (SavageBio_ActAnimA, $0BB7F0), not before
   the test. AI_FIRST_STEP has no loop: after AiScript_Set the pop is never flushed
   (SavageBio_ActScript2052, $0BAEC4). Use both as statements on their own (not as the body of
   an if that has an else). The step is counted in a statement of its own, then compared: with
   the object not in a register, the compare reuses the object pointer it reloads
   (BigBio_StateGatherOrbs, $1224B0); `if (++index >= ...)` reloads it twice. */
#define AI_SCRIPT(cur)		((AiScript *)(cur).table)
#define AI_NEXT_STEP(o, cur, n) \
	if (!(cur).table) { \
		(o)->state = (o)->defaultState; \
		(o)->timer = (n); \
	} else do { \
		(cur).index++; \
		if ((cur).index >= AI_SCRIPT(cur)->count) \
			(cur).index = 0; \
		(o)->state = FN_AT(StateFn, AI_SCRIPT(cur)->step + (cur).index); \
		(o)->timer = (n); \
	} while (0)
#define AI_FIRST_STEP(o, cur, n) \
	if (!(cur).table) { \
		(o)->state = (o)->defaultState; \
		(o)->timer = (n); \
	} else { \
		(cur).index = 0; \
		(o)->state = FN_AT(StateFn, AI_SCRIPT(cur)->step + (cur).index); \
		(o)->timer = (n); \
	}

/* Stop moving: no acceleration, no speed. MPW C merges the two speed clears into one clr.l
   (184 functions have clr.w +$1E / clr.l +$16 / clr.w +$20). No loop: it doesn't move
   the pending pops of a call before it. */
#define STOP_MOVING(o)		((o)->ax = 0, (o)->vx = 0, (o)->vy = 0, (o)->ay = 0)

#endif /* XMEN2_MACROS_H */
